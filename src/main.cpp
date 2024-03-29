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

int imageW = 0;
int imageH = 0;


//顶点坐标的数组，会通过VBO传给显卡
float vertices[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 左上角   纹理坐标
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 左下角   纹理坐标
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // 右下角  纹理坐标
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 右上角  纹理坐标
};
//索引 ebo用
unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
};

Shader *shader;


void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void setImageInitTransform();


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    //启用着色器layout(location = 0)这个属性，默认是关闭的，必须打开，顶点着色器里的layout(location = 0)才会生效
    glEnableVertexAttribArray(0);

    //解释layout(location = 1) 并启用
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //解绑当前的ARRAY_BUFFER，方便之后绑定其他的ARRAY_BUFFER 如果没必要建议不解绑，这里做演示
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //===========================以上是VBO相关========================================

    //生成EBO，和VBO一样也是附着在VAO上
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    //绑定ELEMENT_ARRAY_BUFFER 到EBO对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //与VBO不同，EBO操作完成后不要解绑，因为EBO附着在VAO里，一个VAO有且只有一个EBO，解绑意味当前VAO绑定了一个空的EBO

    //解绑当前的VAO，之后可以绑定并操作其他的VAO， 攻略上写的如果只有一个VAO不建议解绑
    //这里可以不用解绑，因为只绘制一个VAO，如果解绑了在绘制的时候再绑定即可
    glBindVertexArray(0);



    //纹理
    //生成纹理，并绑定当前GL_TEXTURE_2D的纹理为GL_TEXTURE_2D
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //stb image 读取图片
    int imageChannels;
    //设置stb反转y轴
    stbi_set_flip_vertically_on_load(true);
    auto *imageData = stbi_load("../src/resources/3.jpg", &imageW, &imageH, &imageChannels, 0);
    if (imageData) {
        //设置纹理数据
        //GL_TEXTURE_2D :纹理目标
        //0： mipmap的级别，默认级别0
        //GL_RGB： 设置纹理的颜色格式
        //imageW： 图片宽度
        //imageH:  图片高度
        //0:   历史遗留 必须为0
        //GL_RGB: 图片的颜色格式
        //GL_UNSIGNED_BYTE: 图片的数据格式
        //imageData: 图片的数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageW, imageH, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "load image1 error!" << endl;
        return -1;
    }

    //图片读取完成释放资源
    stbi_image_free(imageData);


    //创建着色器程序 并设置各种uniform
    //在设置uniform之前一定要先使用当前的program
    shader = new Shader("../src/glsl/vertex_shader.glsl", "../src/glsl/fragment_shader.glsl");
    shader->use();

    setImageInitTransform();


    //设置纹理单元，这样着色器就知道每一个sampler2D对应几号纹理单元了
    shader->setTexture("imageTexture", 0);


    auto matrix = glm::mat4{1.0f};
    shader->setUniformMatrix4fv(glm::value_ptr(matrix), "transform");


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
        glBindVertexArray(VAO);

        //激活并绑定纹理  记得用GL_TEXTURE0  GL_TEXTURE1 等，不要使用0和1!
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);


        //绘制当前VAO的EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        //解绑VAO，之后可以绘制其他VAO  我们这里只绘制1个VAO，其实可以不用解绑
        glBindVertexArray(0);

        //3、检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    //释放资源，删除顶点数组VAO，两个缓冲：顶点缓冲VBO和元素/索引缓冲EBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //释放纹理
    glDeleteTextures(1, &texture);
    shader->clear();
    delete shader;

    //程序结束 中止glfw
    glfwTerminate();
    return 0;
}


//应用的窗口大小改变时回调这个，重新设置opengl的视窗大小
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    screenW = width;
    screenH = height;
    setImageInitTransform();
}

//按键事件，按下返回，关闭window
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

//计算图片的初始缩放
void setImageInitTransform() {

    auto initMatrix = glm::mat4{1.0f};


    float imageScale = (float) imageW / (float) imageH;
    float screenScale = (float) screenW / (float) screenH;

    if (imageScale > screenScale) {
        //长图
        float scale = 1.0f / imageScale * screenScale;
        initMatrix = glm::scale(initMatrix, glm::vec3(1.0f, scale, 1.0f));
    } else {
        //宽图
        float scale = imageScale * (1.0f / screenScale);
        initMatrix = glm::scale(initMatrix, glm::vec3(scale, 1.0f, 1.0f));
    }


    shader->setUniformMatrix4fv(glm::value_ptr(initMatrix), "initTransform");
}


