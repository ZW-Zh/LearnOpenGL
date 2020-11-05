#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(){
	glfwInit();//初始化GLFW
	//glfwWindowHint配置GLFW
	//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	//次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	//使用核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	

	return 0;
}