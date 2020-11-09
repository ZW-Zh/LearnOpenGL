#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//窗口大小变化回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理输入
void processInput(GLFWwindow* window);

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

