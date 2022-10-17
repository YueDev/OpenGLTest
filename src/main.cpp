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


void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void setMix(float delta);

void setRotate(float delta);

void setCameraZ(float delta);

void setCenterY(float delta);
//顶点坐标  顶点颜色  纹理坐标
float vertices[] = {
        //扩展为 6个面的立方体，这是正面
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // tl  0
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bl  1
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // br  2
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     //tr   3

        //后面
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // tl  4
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  //bl  5
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // br  6
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   //tr   7

        // 右面
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // tl  8
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bl  9
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // br  10
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     //tr   11
        //左面
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // tl  12
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bl  13
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // br  14
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     //tr   15
        //上面
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // tl  16
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bl  17
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // br  18
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     //tr   19
        //下面
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // tl  20
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bl  21
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // br  22
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     //tr   23

};


unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 13, 14,
        12, 14, 15,
        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23
};


float mix = 0.0f;

float rotate = -45.0f;

float cameraZ = 3.0f;

float centerY = 0.0f;

int screenW = 800;
int screenH = 600;



int main() {

    //初始化glfw
    glfwInit();

    //设置window hint来配置glfw
    //opengl版本是3.3，因此设置主版本为3，此版本为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //下载的glad是core版本，因此声明opengl配置为核心配置
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //mac os需要设置向后兼容，才能使上边的设置生效
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //定义一个window对象
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Test", nullptr, nullptr);
    if (window == nullptr) {
        //如果window对象为空，中止glfw
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //glfw设置windows为当前线程的context
    glfwMakeContextCurrent(window);


    //检测GLAD是否可用，这是必须的
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    //设置opengl的视窗大小
    // opengl坐标范围是[-1 ,1] ，通过设置这个可以让opengl把坐标映射到800, 600
    glViewport(0, 0, screenW, screenH);
    //注册回调，当应用窗口大小改变时回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //开启剔除面  剔除背面
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //开启深度测试，这样可以使用Z缓冲
    glEnable(GL_DEPTH_TEST);


    int num;
    //检查vao最大有几个位置可以绑定vbo
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num);
    std::cout << "MAX VERTEX ATTRIBS: " << num << std::endl;

    //先初始化shader类
    auto *shader = new Shader("../src/glsl/VertexShader.glsl", "../src/glsl/FragmentShader.glsl");


    //配置完成后，准备渲染前，先定义各种数据

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);



    //绑定第一个VAO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //纹理
    unsigned int texture[2];
    //生成2D纹理
    glGenTextures(2, texture);

    //激活0号纹理
    glActiveTexture(GL_TEXTURE0);
    //绑定texture[0]到0号纹理
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    //设置纹理的环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置纹理的放大缩小的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //只有缩小的时候才用到mipmap，放大不设置mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);



    //使用stbimage读取图片数据
    int width, height, channelNum;
    //设置y轴翻转，stdb默认（0， 0）是左上角，而opengl纹理坐标(0, 0)点是左下角
    stbi_set_flip_vertically_on_load(1);
    unsigned char *imgData = stbi_load("../src/resources/1.jpg", &width, &height, &channelNum, 0);

    if (imgData) {
        //把图片数据设置到纹理上
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
        //设置mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load image" << std::endl;
        return -1;
    }

    //设置完纹理后清除图片数据
    stbi_image_free(imgData);

    //设置另一个纹理

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    //设置纹理的环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置纹理的放大缩小的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


    imgData = stbi_load("../src/resources/2.jpg", &width, &height, &channelNum, 0);

    if (imgData) {
        //把图片数据设置到纹理上 注意带透明，要用RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load image" << std::endl;
        return -1;
    }

    //设置完纹理后清除图片数据
    stbi_image_free(imgData);

    //通过uniform  告诉sample2d用几号通道,记得先useProgram
    shader->useProgram();
    shader->setTexture("hanaTexture", 0);
    shader->setTexture("stickerTexture", 1);

    //10个立方体的世界坐标，model矩阵做完旋转 后乘即可
    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };


    // render loop 渲染循环
    while (!glfwWindowShouldClose(window)) {
        //1、处理输入事件
        processInput(window);

        //2、进行渲染
        //设置清空屏幕颜色，RGBA,    设置状态
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清空屏幕，清空的是颜色，     使用状态  使用深度测试
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//        //获取当前时间秒，并通过sin函数/2 转换为-0.5到0.5，然后偏移0.5，让取值在0到1之间
//        //这样大概每3.14秒一个周期，用来做颜色变换效果很好用
//        double time = glfwGetTime();
//        float greenValue = (sin(time) / 2.0f) + 0.5f;
//

        //连接完成后，使用program
        shader->useProgram();
        shader->setMix(mix);

        //坐标变换
        //定义模型矩阵，视图矩阵、投影矩阵
        //因为更改比较频繁，就放到渲染线程了

        //这里用lookat构建camera的matrix
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, cameraZ), glm::vec3(0.0f, centerY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


        glm::mat4 projectionMatrix = glm::mat4(1.0f);

        //ortho正射投影，没有远近变化
        //        projectionMatrix = glm::ortho(-screenW * 1.0f / screenH, screenW * 1.0f / screenH, -1.0f, 1.0f, 0.1f, 100.0f);
        projectionMatrix = glm::perspective(glm::radians(45.0f), screenW * 1.0f / screenH, 0.1f, 100.0f);


//        //通过uniform把我们的数值传给opengl的program
//        glUniform4f(ourColorUniform, 0.0f, greenValue, 0.0f, 1.0f);

        //绘制前绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);


        glBindVertexArray(VAO);

        float angle = 20.0f;

        for (const auto &position: cubePositions) {
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, position);
            float degree = glm::radians(angle);
            if ((int) angle % 20) {
                degree *= (float) glfwGetTime();
            }
            modelMatrix = glm::rotate(modelMatrix, degree, glm::vec3(1.0f, 0.3f, 0.5f));
            angle += 10.0f;
            shader->setCoordMatrix(glm::value_ptr(modelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }



        //3、检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader->deleteProgram();

    //程序结束 中止glfw
    glfwTerminate();
    return 0;
}

//应用的窗口大小改变时回调这个，重新设置opengl的视窗大小
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

//按下返回，关闭window
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        setMix(0.01f);
    } else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        setMix(-0.01f);
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        setCameraZ(-0.01f);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        setCameraZ(0.01f);
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        setCenterY(0.01f);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        setCenterY(-0.01f);
    }
}

void setMix(float delta) {
    mix += delta;
    mix = fmin(1.0f, fmax(0.0f, mix));
}


void setCameraZ(float delta) {
    cameraZ += delta;
    cameraZ = fmin(5.0f, fmax(0.1f, cameraZ));
}

void setCenterY(float delta) {
    centerY += delta;
    centerY = fmin(1.0f, fmax(-1.0f, centerY));
    std::cout << centerY << std::endl;

}



