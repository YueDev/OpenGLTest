//
// Created by Yue on 2022/7/27.
//

#include "Shader.h"


Shader::Shader(const string &vertexPath, const string &fragmentPath) {
    string vertexCode;
    string fragmentCode;
    ifstream vertexFile;
    ifstream fragmentFile;

    // 保证ifstream对象可以抛出异常：
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        stringstream vertexStream;
        stringstream fragmentStream;

        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();

        vertexFile.close();
        fragmentFile.close();

    } catch (ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        exit(-1);
    }

    const char *vertexChars = vertexCode.c_str();
    const char *fragmentChars = fragmentCode.c_str();


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexChars, nullptr);
    glCompileShader(vertexShader);
    //检查编译结果
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(-1);
    }


    //同上，fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentChars, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(-1);
    }
    //链接着色器，生成程序
    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM::LINK_PROGRAM_FAILED\n" << infoLog << std::endl;
        exit(-1);
    }

    //程序生成后，删除着色器对象即可
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::setTexture(const string& sample2DName, int textureUnitId) const {
    int location = glGetUniformLocation(programId, sample2DName.c_str());
    glUniform1i(location, textureUnitId);
}

void Shader::use() const {
    glUseProgram(programId);
}

void Shader::clear() const{
    glDeleteProgram(programId);
}
