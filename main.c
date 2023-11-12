#include<stdio.h>
#include<conio.h>
#include<glad/glad.h>
#include"glad.c"
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
	glfwWindowHint(GLFW_SAMPLES,32);

	GLFWwindow*window=glfwCreateWindow(width,height,"Plattrym",glfwGetPrimaryMonitor(),NULL);

	if(window==NULL)
	{
		printf("Failed to create GLFW window\n");
		getch();
		glfwTerminate();
		return-1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	

	glViewport(0,0,width,height);

	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0,0,1,1.F);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}

