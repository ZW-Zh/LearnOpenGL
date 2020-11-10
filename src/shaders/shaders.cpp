#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

//窗口大小变化回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理输入
void processInput(GLFWwindow* window);
//shader glsl语言编写
//顶点着色器。它会在GPU上创建内存用于储存我们的顶点数据，还要配置OpenGL如何解释这些内存，并且指定其如何发送给显卡。顶点着色器接着会处理我们在内存中指定数量的顶点。
const char *vertexShaderSource = "#version 330 core\n"
    //位置输入3维向量aPos
	"layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;"
    "}\0";
//片段着色器，计算一个像素的最终颜色
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\0";

int main(){
	glfwInit();//初始化GLFW
	//glfwWindowHint配置GLFW
	//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	//次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	//使用核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if(window == NULL)
	{
		std::cout<< "Failed to Create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//初始化GLAD，传入加载系统相关opengl函数指针地址的函数
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//设置渲染窗口（视口）的维度，前两个参数控制窗口在左下角
	//维度可以设置的比GLFW的维度小，这样渲染会在一个更小的窗口中显示
	//OpenGL坐标范围只为-1到1
	glViewport(0, 0, 800, 600);
	//注册窗口大小改变的回调函数
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

	//定义顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//着色器源码附到着色器对象上,并编译
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//检查编译是否成功
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
    	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}	
	//定义片段着色器，附加源码并编译
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

	//创建着色器链接程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	//检测链接程序
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
    	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
   		std::cout << "ERROR::SHADER::LINKPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	//链接之后就可以删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);	
	//位置数据会被存储为32位（4字节）浮点值
	float vertices[] = {
    	// 位置              // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};
	//VBO：顶点缓冲对象
	//VAO：顶点数组对象
    unsigned int VBO, VAO;
	//创建对象
 	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	// 1. 绑定VAO
	glBindVertexArray(VAO);
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. 设置顶点属性指针，顶点属性位置值，大小，类型，是否标准化，步长，偏移量
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//位置值0
    glEnableVertexAttribArray(0);

    //颜色属性，绑定
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //位置值1
    glEnableVertexAttribArray(1);
	//glVertexAttribPointer方法注册了VBO对象，我们可以安全的释放他。
	glBindBuffer(GL_ARRAY_BUFFER,0);
	//之后，您可以取消绑定VAO，这样其他VAO调用就不会意外修改此VAO，但这很少发生。 无论如何，修改其他VAO都需要调用glBindVertexArray，因此通常在不需要直接取消绑定VAO（也没有VBO）的情况下，我们通常不会取消绑定。
	glBindVertexArray(0);
	//线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//渲染循环
	//循环前检查GLFW是否被要求退出
	while(!glfwWindowShouldClose(window))
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

		//激活程序对象
		glUseProgram(shaderProgram);

        // 更新uniform颜色
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		//打算绘制时绑定VAO
		//绑定VAO的同时也会自动绑定EBO。所以不用每次渲染一个物体都要绑定EBO
		glBindVertexArray(VAO);
		//传递图元类型，顶点数组起始索引，顶点数量。
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
	glDeleteProgram(shaderProgram);
	//释放/删除之前分配的所有资源。
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{	
	//按下esc，关闭GLFW,下次while循环会失败，程序关闭
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

