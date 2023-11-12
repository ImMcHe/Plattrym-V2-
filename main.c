#include<stdio.h>
#include<conio.h>
#include<glad/glad.h>
#include"glad.c"
#include<GLFW/glfw3.h>
#include<stbi/image.h>
#define uint unsigned int

const float squareVert[]={
	-.5,-.5, 0,0,
	-.5,.5,  0,1,
	.5,-.5,  1,0,
	.5,.5,   1,1,
};
const uint squareInd[]={
	0,1,2,1,3,2
};

#include"Shader.c"
#include"Buffer.c"
//#include"Plattrym.c"

const int width=1920,height=1080;

int main(int argc,char**argv)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow*window=glfwCreateWindow(width,height,"Plattrym (V2)",glfwGetPrimaryMonitor(),NULL);

	if(window==NULL)
	{
		printf("Failed to create GLFW window\n");
		getch();
		glfwTerminate();
		return-1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	uint shader=initShaders();
	uint vao,vbo,ebo;
	initBuffers(1,&vao,&vbo,&ebo);

	glUseProgram(shader);
	glBindVertexArray(vao);

	glViewport(0,0,width,height);

	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0,0,1,1.F);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glDrawArrays(GL_TRIANGLES,0,4);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteProgram(shader);
	glDeleteBuffers(1,&vao);
	glDeleteBuffers(1,&vbo);
	glDeleteBuffers(1,&ebo);

    return 0;
}

