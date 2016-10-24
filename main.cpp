/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| Vertex Buffer Objects and interpolation                                      |
\******************************************************************************/
#include "gl_utils.h" // utility stuff discussed in previous tutorials is here
#include <glad/glad.h>
#include <GLFW/glfw3.h> // GLFW helper library

#include <cmath>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define GL_LOG_FILE "gl.log"
#include <SFML/System.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>


#include "Camera.h"
#include "src/Moveable.hpp"

#include "GameController.cpp"
#include "GLManager.cpp"

#include "src/graphics/gl/Mesh.h"
#include "src/graphics/gl/ShaderCompiler.h"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = nullptr;
static const double pi = 4 * atan(1);
GLManager myGLManager;
bool changed = false;

Camera myCam;

Mesh* myMesh;
Mesh* myGround;
I_Moveable* stone;
I_Moveable* stone2;

I_Moveable* mySelf;


ShaderCompiler myCompiler;

GLuint rendering_program[3];
GLuint currentProgram = 0;

void startup() {

    myCompiler.addShader("vertex_shader.glsl", GL_VERTEX_SHADER);
    myCompiler.addShader("fragment_shader.glsl", GL_FRAGMENT_SHADER);
    rendering_program[0] = myCompiler.compileProgram();

    myCompiler.addShader("vertex_shader2.glsl", GL_VERTEX_SHADER);
    myCompiler.addShader("fragment_shader2.glsl", GL_FRAGMENT_SHADER);
    rendering_program[1] = myCompiler.compileProgram();

    myCompiler.addShader("vertex_shader3.glsl", GL_VERTEX_SHADER);
    myCompiler.addShader("fragment_shader3.glsl", GL_FRAGMENT_SHADER);
    rendering_program[2] = myCompiler.compileProgram();

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    myCam.setLens(90, float(g_gl_width) / float(g_gl_height), 0.001, 1000);
    stone->setMaxSpeed(-1);
    stone->setSpeedComposition(1, 0, 0);
    stone2->setMaxSpeed(0.5);
    stone2->setSpeedComposition(1, 0, 0);


}

void render(GLfloat bgcolor[],
            const float& myX, const float myY, const float& myZ,
            const float& heading, const double& pitch) {


    static GLint colour_location;

    changed = false;
    if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_C)) {
        currentProgram = rendering_program[0];
        changed = true;
    }
    if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_V)) {
        currentProgram = rendering_program[1];
        changed = true;
    }

    if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_B)) {
        currentProgram = rendering_program[2];
        changed = true;
    }





    glClearBufferfv(GL_COLOR, 0, bgcolor);
    static const GLfloat one = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &one);

    glm::vec4 ColourMatrix = glm::vec4(255.0f, 0.0f, 0.0f, 127.0f);
    if (currentProgram == rendering_program[1] && changed) {
        colour_location = glGetUniformLocation(currentProgram, "colour");
        glUniform4fv(colour_location, 1, &ColourMatrix[0]);
    }

    //myCam.setPosition(myX, myY, myZ);
    //myCam.setDirection(heading, pitch);



    myCam.draw(myGround, currentProgram);
    myMesh->attachToPosition(stone);
    myCam.draw(myMesh, currentProgram);
    myMesh->attachToPosition(stone2);
    myCam.draw(myMesh, currentProgram);
}

void shutdown() {

}

void getInputs(GameController& gameController) {
    gameController.clearInputContents();
    gameController.readKeys();
}

int main () {



    assert (restart_gl_log ());
    // all the start-up code for GLFW and GLEW is called here
    assert (start_gl ());

    //glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const sf::Clock clock;
    sf::Time dt;
    double currentTime = 0;
    double previousTime;
    double deltaTime;

    bool firstTime = true;




    mySelf = new Moveable;





    GameController gameController(g_window);

    MeshGeometry myCubeMesh;
    MeshGeometry myFloorMesh;

    myCubeMesh.beCube();
    myFloorMesh.bePlane();
    myMesh = new Mesh(&myCubeMesh);
    myGround = new Mesh(&myFloorMesh);
    stone = new Moveable(0.0f, 0.0f, 0.0f, 0.0f);
    stone2 = new Moveable(-20.0f, -10.0f, 1.0f, 0.0f);
    myCam.attachTo(mySelf);




    //std::cout<<"Shading languages available: \n"<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;


    while (!glfwWindowShouldClose (g_window)) {
        if (firstTime) {
            startup();
            firstTime = false;
        }


        //updateGame(gameClock, gameController, gameStatus);
        //emitGraphics(window, gameStatus,camera);

        dt = clock.getElapsedTime();
        previousTime = currentTime;
        currentTime = dt.asSeconds();
        deltaTime = currentTime - previousTime;
        _update_fps_counter (g_window);



        //gameController.readKeys();
        getInputs(gameController);


        mySelf->setOrientationDelta(gameController.Rotation_delta, gameController.Pitch_delta, 0);

        mySelf->setMaxSpeed(5.0);
        mySelf->setSpeedComposition(0.0f, 0.0f, 0.0f);

        if (gameController.GoLeft) {
            mySelf->setSpeedDelta(0.0f, -1.0f, 0.0f);
        }

        if (gameController.GoRight) {
            mySelf->setSpeedDelta(0.0f, 1.0f, 0.0f);
        }

        if (gameController.GoForward) {
            mySelf->setSpeedDelta(1.0f, 0.0f, 0.0f);
        }

        if (gameController.GoBack) {
            mySelf->setSpeedDelta(-1.0f, 0.0f, 0.0f);
        }

        if (gameController.RequestJump) {
            mySelf->setSpeedDelta(0.0f, 0.0f, 10.0f);;
            mySelf->setPositionDelta(0.0f, 0.0f, 0.2f);
        }

        mySelf->move(deltaTime);
        stone->move(deltaTime);
        stone2->move(deltaTime);


        GLfloat bgcolor[] = {0.5f, 0.5f, 1.0f, 1.0f};

        render(bgcolor, (float)mySelf->getPosition().axis_one, (float)mySelf->getPosition().axis_two, (float)mySelf->getPosition().axis_three, (float)mySelf->getHeading(), (float)mySelf->getOrientation().axis_two);
        //render(bgcolor,f0,hisZ,pos_x,pos_y,pos_z,heading,pitch);
        glfwSwapBuffers (g_window);

    }


// close GL context and any other GLFW resources
    glfwDestroyWindow(g_window);
    glfwTerminate();
    return 0;
}
