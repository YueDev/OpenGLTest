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




    // render loop 渲染循环
    while (!glfwWindowShouldClose(window)) {
        //1、处理输入事件
        processInput(window);

        //2、进行渲染

        //设置清空屏幕颜色，RGBA,    设置状态
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清空屏幕，清空的是颜色
        glClear(GL_COLOR_BUFFER_BIT);


        //3、检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

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
    }
}




