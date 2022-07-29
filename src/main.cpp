#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//矩阵库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


void print(const glm::vec4 &vec4);
void print(const glm::mat4x4 &mat);

int main() {

    //新建一个1.0 1.0 1.0的向量，注意向量的w分量是1.0；
    auto v = glm::vec4 {1.0f};

    //新建一个单位矩阵
    auto m = glm::mat4 {1.0f};
    //矩阵平移(1.0f 2.0f 3.0f)
    m = glm::translate(m, glm::vec3{1.0f, 2.0f, 3.0f});

    //将向量映射到矩阵，即向量v 平移了(1.0f 2.0f 3.0f)
    v = m * v;

    print(v);
    print(m);

}

//打印向量
void print(const glm::vec4 &vec4) {
    std::cout << "{\t" << vec4.x << ",\t" << std::endl;
    std::cout << " \t" << vec4.y << ",\t" << std::endl;
    std::cout << " \t" << vec4.z << ",\t" << std::endl;
    std::cout << " \t" << vec4.w << ",\t" << "}" << std::endl;
}
//打印矩阵
void print(const glm::mat4 &mat) {
    std::cout << "{\t" << mat[0].x << ",\t" << mat[1].x << ",\t" << mat[2].x << ",\t" << mat[3].x << ",\t" << std::endl;
    std::cout << " \t" << mat[0].y << ",\t" << mat[1].y << ",\t" << mat[2].y << ",\t" << mat[3].y << ",\t" << std::endl;
    std::cout << " \t" << mat[0].z << ",\t" << mat[1].z << ",\t" << mat[2].z << ",\t" << mat[3].z << ",\t" << std::endl;
    std::cout << " \t" << mat[0].w << ",\t" << mat[1].w << ",\t" << mat[2].w << ",\t" << mat[3].w << ",\t" << "}" << std::endl;
};


