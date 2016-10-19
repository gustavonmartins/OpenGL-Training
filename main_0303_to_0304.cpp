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

#define GL_LOG_FILE "gl.log"
#include <SFML/System.hpp>

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow* g_window = NULL;



GLuint compile_shaders(void)
{
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar* vertex_shader_source[]=
    {
        "#version 400 core                                                              \n"
        "                                                                               \n"
        "layout(location=0) in vec4 partoffset;                                        \n"
        "layout(location=1) in vec4 partcolor;                                         \n"
        "out vec4 vs_color;                                                             \n"
        "                                                                               \n"
        "void main(void)                                                                \n"
        "{                                                                              \n"
        "   const vec4 vertices[3]=vec4[3](vec4(+0.25,-0.25,0.5,1.0),                        \n"
        "                                  vec4(-0.25,-0.25,0.5,1.0),                      \n"
        "                                  vec4(+0.25,+0.25,0.5,1.0));                      \n"
        "                                                                               \n"
        "   gl_Position=vertices[gl_VertexID]+partoffset;                                   \n"
        "   vs_color=partcolor;                                                         \n"
        "}                                                                              \n"
    };

    static const GLchar* fragment_shader_source[]=
    {
        "#version 400 core                                                           \n"
        "                                                                               \n"
        "in vec4 vs_color;                                                              \n"
        "out vec4 partcolor;                                                             \n"
        "                                                                               \n"
        "                                                                               \n"
        "void main(void)                                                                \n"
        "{                                           \n"
        "   partcolor=vs_color;                                                                                           \n"
        "}                                                                              \n"
    };

    vertex_shader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader,1,vertex_shader_source,NULL);
    glCompileShader(vertex_shader);

    fragment_shader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader,1,fragment_shader_source,NULL);
    glCompileShader(fragment_shader);

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
    glGenVertexArrays(1,&vertex_array_object);
    glBindVertexArray(vertex_array_object);

}

void render(double currentTime)
{
    const GLfloat bgcolor[]= {(float)sin(currentTime)*0.5f+0.5f,
                              (float)cos(currentTime)*0.5f+0.5f,
                              0.0f,1.0f
                             };
    GLfloat partoffset[]= {(float)sin(currentTime)*0.5f,
                           (float)cos(currentTime)*0.6f,
                           0.0f,0.0f
                          };
    GLfloat partcolor[]= {0.0f,0.8f,1.0f,1.0f};

    glVertexAttrib4fv(0,partoffset);
    glVertexAttrib4fv(1,partcolor);
    glUseProgram(rendering_program);
    glClearBufferfv(GL_COLOR,0,bgcolor);
    glDrawArrays(GL_TRIANGLES,0,3);
}

void shutdown()
{
    glDeleteVertexArrays(1,&vertex_array_object);
    glDeleteProgram(rendering_program);
    glDeleteVertexArrays(1,&vertex_array_object);
}



int main ()
{
    assert (restart_gl_log ());
    // all the start-up code for GLFW and GLEW is called here
    assert (start_gl ());

    const sf::Clock clock;
    sf::Time dt;
    double currentTime;

    bool firstTime=true;
    bool lastTime=false;

    //std::cout<<"Shading languages available: \n"<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;


    while (!glfwWindowShouldClose (g_window))
    {
        dt=clock.getElapsedTime();
        currentTime=dt.asSeconds();
        _update_fps_counter (g_window);
        glfwPollEvents ();
        if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE))
        {

            glfwSetWindowShouldClose (g_window, 1);
            shutdown();
        }
        if (firstTime)
        {
            startup();
            firstTime=false;
        }

        render(currentTime);
        glfwSwapBuffers (g_window);

    }
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
