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

//窗口的的长宽
int screenW = 1920;
int screenH = 1080;

//顶点着色器的源码，从vertex_shader.glsl复制过来，省去操作文件IO
const char *vertexShaderString = "#version 330 core\n"
                                 "layout(location = 0) in vec3 aPos;\n"
                                 "\n"
                                 "void main() {\n"
                                 "    gl_Position = vec4(aPos.xyz, 1.0);\n"
                                 "}";
//片段着色器的源码，从fragment_shader.glsl复制过来
const char *fragmentShaderString = "#version 330 core\n"
                                   "\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "void main() {\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}";
//顶点坐标的数组，会通过VBO传给显卡
float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);


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
    GLFWwindow *window = glfwCreateWindow(screenW, screenH, "OpenGL Test", nullptr, nullptr);
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


    //创建一个顶点数组VERTEX ARRAY，用来管理VBO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //绑定当前VAO，这样之后的VBO操作都是和这个VAO相关了
    glBindVertexArray(VAO);
    //===========================以下是VBO相关========================================
    //创建一个ARRAY BUFFER，并绑定在VBO上，这样之后对ARRAY BUFFER的操作都是对VBO的操作
    //通过bufferdata，把顶点数据放到显存中了，用VBO这个对象来管理这些数据
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //解释ARRAY BUFFER里的数据
    //0 就是顶点着色器里的layout(location = 0)，表示数据是传送到0号
    //3  是顶点着色器里的属性有几个数据组成，我们定义的aPos是vec3，因此一个属性需要3个数据
    //GL_FLOAT vec3是浮点数，因此用GL_FLOAT
    //3 * sizeof(float)  步长，每组数据的长度，由于我们数据是3个float一组，因此用3 * sizeof(float)
    //(void *) 0 偏移量 数据开始的偏移量，这里攻略上写的是(void *) 0，IDE提示我用nullptr
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    //启用着色器layout(location = 0)这个属性，默认是关闭的，必须打开，顶点着色器里的layout(location = 0)才会生效
    glEnableVertexAttribArray(0);
    //解绑当前的ARRAY_BUFFER，方便之后绑定其他的ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //===========================以上是VBO相关========================================
    //解绑当前的VAO，之后可以绑定并操作其他的VAO， 攻略上写的如果只有一个VAO不建议解绑
    //这里可以不用解绑，因为只绘制一个VAO，如果解绑了在绘制的时候再绑定即可
//    glBindVertexArray(0);


    //GLuint就是unsigned int
    //这里先创建一个VERTEX_SHADER对象vertexShader， 然后把shader的string传给这个对象，最后编译这个对象
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderString, nullptr);
    glCompileShader(vertexShader);
    //检查编译结果
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    //同上，fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderString, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    //链接着色器，生成程序
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINK_PROGRAM_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    //程序生成后，删除着色器对象即可
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //使用程序
    glUseProgram(shaderProgram);


    // render loop 渲染循环
    while (!glfwWindowShouldClose(window)) {
        //1、处理输入事件
        processInput(window);

        //2、进行渲染

        //2.1清空屏幕上之前绘制的的内容，这里用颜色来覆盖之前的内容
        // 设置清空屏幕颜色，RGBA,    设置状态
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清空屏幕，清空的是颜色
        glClear(GL_COLOR_BUFFER_BIT);

        //2.2 绘制内容
        // 选择绘制的程序，之前设定了，我们不需要多个程序，因此注释掉
        // glUseProgram(shaderProgram);
        //绑定要绘制的VAO
//        glBindVertexArray(VAO);
        //绘制三角形,0是数组起始的编号，3是打算绘制多少个顶点，因为我们数据中只有3个点的坐标，因此写3即可
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //解绑VAO，之后可以绘制其他VAO  我们这里只绘制1个VAO，不用解绑因此注释掉了
//        glBindVertexArray(0);

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

//按键事件，按下返回，关闭window
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}




