#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
struct GLManager {
    void CheckShaderStatus (const GLuint& shader, const std::string& message) {

        GLint status = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status!= 1) {
            std::cout<<message<<" failed to compile";
            GLint len;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
            std::string str;
            str.resize(len);
            glGetShaderInfoLog(shader, len, NULL, &str[0]);
            std::ofstream out("error.txt",std::ofstream::app);
            out<<"----------------"<<message<<" debug info: \n";
            out<<str;
            out.close(); }

    }

    GLuint compile_shaders(void) {
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
            glShaderSource(vertex_shader,1,&vertex_shader_source,NULL); }

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
            glShaderSource(fragment_shader,1,&fragment_shader_source,NULL); }

        glCompileShader(fragment_shader);
        CheckShaderStatus(fragment_shader,"Fragment Shader");

        program=glCreateProgram();
        glAttachShader(program,vertex_shader);
        glAttachShader(program,fragment_shader);
        glLinkProgram(program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program; }


};
