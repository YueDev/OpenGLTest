//
// Created by Lenovo on 2021/4/26.
//
#ifndef OPENGLTEST_SHADER_H
#define OPENGLTEST_SHADER_H


class Shader {
public:

    unsigned int programID;

    Shader(const char *vertexPath, const char *fragmentPath);

    void useProgram() const;

    void deleteProgram() const;
    void setTexture(const char *textureName, int textureIndex) const;
    void setMix(float mix) const;
    void setCoordMatrix(float *modelMatrix, float *viewMatrix, float *projectionMatrix) const;
};

#endif //OPENGLTEST_SHADER_H
