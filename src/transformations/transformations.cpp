#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//窗口大小变化回调函数
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//处理输入
void processInput(GLFWwindow *window);
int main()
{
    glfwInit(); //初始化GLFW
    //glfwWindowHint配置GLFW
    //主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //次版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //使用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //创建窗口对象
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to Create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //初始化GLAD，传入加载系统相关opengl函数指针地址的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //设置渲染窗口（视口）的维度，前两个参数控制窗口在左下角
    //维度可以设置的比GLFW的维度小，这样渲染会在一个更小的窗口中显示
    //OpenGL坐标范围只为-1到1
    glViewport(0, 0, 800, 600);
    //注册窗口大小改变的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader ourShader("src/textures/shader.vs", "src/textures/textures_exercise1.fs");

    //位置数据会被存储为32位（4字节）浮点值
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    //VBO：顶点缓冲对象
    //VAO：顶点数组对象
    //EBO：索引缓冲对象
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //glVertexAttribPointer方法注册了VBO对象，我们可以安全的释放他。
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //之后，您可以取消绑定VAO，这样其他VAO调用就不会意外修改此VAO，但这很少发生。 无论如何，修改其他VAO都需要调用glBindVertexArray，因此通常在不需要直接取消绑定VAO（也没有VBO）的情况下，我们通常不会取消绑定。
    glBindVertexArray(0);

    //生成纹理
    unsigned int texture[2];
    //纹理数量
    glGenTextures(1, &texture[0]);
    //绑定纹理
    //all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //图片翻转y轴，图片的y轴在顶部
    stbi_set_flip_vertically_on_load(true);
    //加载图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load("src/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        //生成纹理
        //纹理目标，多级渐远纹理级别，纹理存储格式，最终纹理宽，高，固定值0，源图格式，源图类型，图片数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 2
    // ---------
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("src/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    //只有一个纹理，默认纹理单元是0，会默认激活，不需要glUniform1i赋值
    ourShader.use();                                                // 不要忘记在设置uniform变量之前激活着色器程序！
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置纹理单元
    ourShader.setInt("texture2", 1);                                // 或者使用着色器类设置

    //渲染循环
    //循环前检查GLFW是否被要求退出
    while (!glfwWindowShouldClose(window))
    {
        //输入
        processInput(window);
        //渲染指令
        //设置清空屏幕所用的颜色，glClearColor：状态设置函数
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        /*
		每次新的渲染前需要清屏，接受一个缓冲位来指定要清空的缓冲
		可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
		glClear：状态使用函数
		*/
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        //它会自动把纹理赋值给片段着色器的采样器：
        //glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        //激活程序对象
        ourShader.use();

        //打算绘制时绑定VAO
        //绑定VAO的同时也会自动绑定EBO。所以不用每次渲染一个物体都要绑定EBO
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //检查并调用事件，交换缓冲
        //交换颜色缓冲（存储每个像素的颜色值的大缓冲），用来绘制
        /*
		双缓冲(Double Buffer)
		前缓冲保存之前的图像，在屏幕上显示，渲染指令在后缓冲上绘制，指令执行完交换前缓冲和后缓冲。
		*/
        glfwSwapBuffers(window);
        //检查有没有触发什么事件（键盘输入，鼠标移动等）、更新窗口状态，调用对应回调函数
        glfwPollEvents();
    }

    //绘制完再释放VAO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    ourShader.shaderDelete();
    //释放/删除之前分配的所有资源。
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    //按下esc，关闭GLFW,下次while循环会失败，程序关闭
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
