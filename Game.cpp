#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <functional>


struct Game
{
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
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <cmath>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define GL_LOG_FILE "gl.log"
#include <SFML/System.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include "GameController.cpp"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = nullptr;
static const double pi=4*atan(1);

void CheckShaderStatus (const GLuint& shader, const std::string& message)
{

    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status!= 1)
    {
        std::cout<<message<<" failed to compile";
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string str;
        str.resize(len);
        glGetShaderInfoLog(shader, len, NULL, &str[0]);
        std::ofstream out("error.txt",std::ofstream::app);
        out<<"----------------"<<message<<" debug info: \n";
        out<<str;
        out.close();
    }

}



GLuint compile_shaders(void)
{
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    vertex_shader=glCreateShader(GL_VERTEX_SHADER);
    {
        std::ifstream shader_source_aux3("vertex_shader.glsl");
        std::stringstream shader_source_aux2;
        shader_source_aux2<<shader_source_aux3.rdbuf();
        std::string shader_source_aux(shader_source_aux2.str());
        const char* vertex_shader_source=shader_source_aux.c_str();
        //glShaderSource(vertex_shader,1,vertex_shader_source,NULL);
        glShaderSource(vertex_shader,1,&vertex_shader_source,NULL);
    }

    glCompileShader(vertex_shader);
    CheckShaderStatus(vertex_shader,"Vertex Shader");

    fragment_shader=glCreateShader(GL_FRAGMENT_SHADER);
    {
        std::ifstream shader_source_aux3("fragment_shader.glsl");
        std::stringstream shader_source_aux2;
        shader_source_aux2<<shader_source_aux3.rdbuf();
        std::string shader_source_aux(shader_source_aux2.str());
        const char* fragment_shader_source=shader_source_aux.c_str();
        //glShaderSource(vertex_shader,1,vertex_shader_source,NULL);
        glShaderSource(fragment_shader,1,&fragment_shader_source,NULL);
    }

    glCompileShader(fragment_shader);
    CheckShaderStatus(fragment_shader,"Fragment Shader");

    program=glCreateProgram();
    glAttachShader(program,vertex_shader);
    glAttachShader(program,fragment_shader);
    glLinkProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

GLuint rendering_program;
GLuint vertex_array_object;

void startup()
{
    rendering_program=compile_shaders();
    //glGenVertexArrays(1,&vertex_array_object);
    //glBindVertexArray(vertex_array_object);

    static const GLfloat vertex_positions[]=
    {
        -0.25f, -0.25f, -0.25f,
        -0.25f, +0.25f, -0.25f,
        +0.25f, -0.25f, -0.25f,
        +0.25f, +0.25f, -0.25f,
        +0.25f, -0.25f, +0.25f,
        +0.25f, +0.25f, +0.25f,
        -0.25f, -0.25f, +0.25f,
        -0.25f, +0.25f, +0.25f,
    };

    static const GLshort vertex_indices[]=
    {
        0,1,2,
        2,1,3,
        2,3,4,
        4,3,5,
        4,5,6,
        6,5,7,
        6,7,0,
        0,7,1,
        6,0,2,
        2,4,6,
        7,5,3,
        7,3,1,
    };

    GLuint buffer;
    GLuint index_buffer;

    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    std::cout<<"Finished creation and binding of buffer"<<std::endl;

    //Fill buffer
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_positions),vertex_positions,GL_STATIC_DRAW);
    std::cout<<"Finished filling buffer"<<std::endl;

    //Transfer buffer data to Vertex Shader

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(0);

    std::cout<<"Finished filling vertex shader from buffer, layout position 0"<<std::endl;

    glGenBuffers(1,&index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(vertex_indices),vertex_indices,GL_STATIC_DRAW);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

void render(GLfloat bgcolor[],
        const float &f0,
        const float &hisZ,
        const float& myX, const float myY, const float &myZ,
        const float &heading, const double &pitch)
{
    glUseProgram(rendering_program);


    glm::vec3 Cam_Pos;
    glm::vec3 Cam_Direction;
    glm::vec3 Cam_Front;
    glm::vec3 Cam_Up;

    Cam_Pos=glm::vec3(-myX,myZ,-myY);
    Cam_Direction=glm::vec3(-1.0f*(float)cos(glm::radians(heading)),1.0f*(float)cos(glm::radians(pitch)),-1.0f*(float)sin(glm::radians(heading)));
    Cam_Up=glm::vec3(0.0f,1.0f,0.0f);
    Cam_Front=Cam_Pos+Cam_Direction;

    glm::mat4 proj_matrix=
            glm::perspective(glm::radians(45.0f),(float)g_gl_width/(float)g_gl_height,0.001f,1000.0f)*
            glm::lookAt(Cam_Pos,Cam_Front,Cam_Up);

    static GLint proj_location;
    proj_location=glGetUniformLocation(rendering_program,"proj_matrix"); // The search field is as described in shader code
    glUniformMatrix4fv(proj_location,1,GL_FALSE,&proj_matrix[0][0]);

    static GLint mv_location;
    mv_location=glGetUniformLocation(rendering_program,"mv_matrix");// The search field is as described in shader code

    glClearBufferfv(GL_COLOR,0,bgcolor);
    static const GLfloat one = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &one);

    for (int i=0; i<24; i++)
    {
        float f=10*(float)i+f0;

        glm::mat4 mv_matrix=
                glm::translate(glm::mat4(1.0f),glm::vec3(sin(0.0f+f),cos(0.0f+f),hisZ))*
                glm::rotate(glm::mat4(1.0f),glm::radians(100.0f*f+f0),glm::vec3(1.0f,0.0f,0.0f))*
                glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,1.0f,1.0f));

        glUniformMatrix4fv(mv_location,1,GL_FALSE,&mv_matrix[0][0]);

        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,0);
    }


}

void shutdown()
{
    glDeleteVertexArrays(1,&vertex_array_object);
    glDeleteProgram(rendering_program);
    glDeleteVertexArrays(1,&vertex_array_object);
}

void getInputs(GameController& gameController)
{
    gameController.clearInputContents();
    gameController.readKeys();
}

void run (void)
{

    assert (restart_gl_log ());
    // all the start-up code for GLFW and GLEW is called here
    assert (start_gl ());

    glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const sf::Clock clock;
    sf::Time dt;
    double currentTime=0;
    double previousTime;
    double deltaTime;

    bool firstTime=true;
    bool lastTime=false;

    float hisZ=-1.0f;

    float pos_x=0.0f;
    float pos_y=0.0f;
    float pos_z=0.0f;

    float speedMax=5.0f;
    float speedLateral=0.0f;
    float speedFrontal=0.0f;

    float speedXGlobal=0.0f;
    float speedYGlobal=0.0f;
    float speedZGlobal=0.0f;

    double heading=0;
    double pitch=0;

    float myVel_Y=0.0f;

    float grav_Y=-9.8f;
    float f0;
    GameController gameController(g_window);


    //std::cout<<"Shading languages available: \n"<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;


    while (!glfwWindowShouldClose (g_window))
    {
        if (firstTime)
        {
            startup();
            firstTime=false;
        }


        //updateGame(gameClock, gameController, gameStatus);
        //emitGraphics(window, gameStatus,camera);

        dt=clock.getElapsedTime();
        previousTime=currentTime;
        currentTime=dt.asSeconds();
        deltaTime=currentTime-previousTime;
        _update_fps_counter (g_window);



        //gameController.readKeys();
        getInputs(gameController);

        heading+=gameController.Rotation_delta;
        pitch+=gameController.Pitch_delta;

        if (heading>360 || heading<-360)
        {
            heading=0;
        }

        speedLateral=0.0f;
        speedFrontal=0.0f;
        if (gameController.GoLeft)
        {
            speedLateral-=1.0f;
        }

        if (gameController.GoRight)
        {
            speedLateral+=1.0f;
        }

        if (gameController.GoForward)
        {
            speedFrontal+=1.0f;
        }

        if (gameController.GoBack)
        {
            speedFrontal-=1.0f;
        }

        if (gameController.RequestJump)
        {
            speedZGlobal+=1.0f;
        }


        speedXGlobal=speedFrontal*(cos((heading)/180*pi))+speedLateral*(cos((heading+90)/180*pi));
        speedYGlobal=speedFrontal*(sin((heading)/180*pi))+speedLateral*(sin((heading+90)/180*pi));
        double mag=std::sqrt(std::pow(speedXGlobal,2)+std::pow(speedYGlobal,2));
        if (mag>0)
        {
            speedXGlobal*=speedMax/mag;
            speedYGlobal*=speedMax/mag;
        }

        pos_x+=speedXGlobal*deltaTime;
        pos_y+=speedYGlobal*deltaTime;
        pos_z+=speedZGlobal*deltaTime;



        f0=(float)currentTime*(float)pi*0.1f;

        GLfloat bgcolor[]= {(float)sin(1.0f*currentTime)*0.5f+0.5f,
                            (float)cos(1.0f*currentTime)*0.5f+0.5f,
                            0.0f,1.0f
                           };

        render(bgcolor,f0,hisZ,pos_x,pos_y,pos_z,heading,pitch);
        glfwSwapBuffers (g_window);

    }


// close GL context and any other GLFW resources
    glfwTerminate();
}

};

