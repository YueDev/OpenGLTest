//
// Created by Lenovo on 2021/4/26.
//

#include "Shader.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>



Shader::Shader(const char *vertexPath, const char *fragmentPath) {

    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::stringstream vertexStream;
    std::stringstream fragmentStream;

    std::string vertexString;
    std::string fragmentString;

    const char *vertexSource;
    const char *fragmentSource;


    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);


        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexString = vertexStream.str();
        fragmentString = fragmentStream.str();

        vertexSource = vertexString.c_str();
        fragmentSource = fragmentString.c_str();


    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }


    unsigned int vertex, fragment;

    int success;
    char log[512];


    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    //检查编译错误
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, log);
        std::cout << "VERTEX SHADER COMPILE ERROR!\n" << log << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, log);
        std::cout << "FRAGMENT SHADER COMPILE ERROR!\n" << log << std::endl;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    //检查program的正确性
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, nullptr, log);
        std::cout << "PROGRAM ERROR!\n" << log << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::useProgram() const {
    glUseProgram(programID);
}

void Shader::deleteProgram() const {
    glDeleteProgram(programID);
}

void Shader::setTexture(const char *textureName, int textureIndex) const {
    glUniform1i(glGetUniformLocation(programID, textureName), textureIndex);
}

void Shader::setMix(float mix) const {
    int uniform = glGetUniformLocation(programID, "mixf");
    glUniform1f(uniform, mix);
}

void Shader::setCoordMatrix(float *modelMatrix, float *viewMatrix, float *projectionMatrix) const {
    int model = glGetUniformLocation(programID, "model");
    int view = glGetUniformLocation(programID, "view");
    int projection = glGetUniformLocation(programID, "projection");

    glUniformMatrix4fv(model, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(view, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projection, 1, GL_FALSE, projectionMatrix);
}



