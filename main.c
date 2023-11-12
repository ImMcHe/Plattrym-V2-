#include<stdio.h>
#include<conio.h>
#include<GLFW/glfw3.h>
#include<stbi/image.h>
#define uint unsigned int

#include"Plattrym.c"

const int width=1920,height=1080;

int main(int argc,char**argv)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow*window=glfwCreateWindow(width,height,"Plattrym",glfwGetPrimaryMonitor(),NULL);

	if(window==NULL)
	{
		printf("Failed to create GLFW window\n");
		getch();
		glfwTerminate();
		return-1;
	}

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}

