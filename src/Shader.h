//
// Created by Yue on 2022/7/27.
//

#ifndef OPENGLTEST_SHADER_H
#define OPENGLTEST_SHADER_H

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
    unsigned int programId;

    //构造器 读取着色器的路径 生成programId
    Shader(const string& vertexPath, const string& fragmentPath);

    void use() const;

    void clear() const;

    void setTexture(const string &textureName, int textureUnitId) const;

    void setUniformMatrix4fv(const float *matrix, const string &locationName) const;
};


#endif //OPENGLTEST_SHADER_H
