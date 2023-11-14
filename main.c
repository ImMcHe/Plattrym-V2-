#include<stdio.h>
#include<conio.h>
#include<glad/glad.h>
#include"glad.c"
#include<GLFW/glfw3.h>
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
size_t sz=1;

#include"Shader.c"
#include"Buffer.c"
#include"Texture.c"
//#include"Plattrym.c"

const int width=1920,height=1080;


static inline void update()
{

}


int main(int argc,char**argv)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow*window=glfwCreateWindow(width,height,"Plattrym (V2)",glfwGetPrimaryMonitor(),NULL);

	printf("Loading Window...\n");
	if(window==NULL)
	{
		printf("Failed to create GLFW window\n");
		getch();
		glfwTerminate();
		return-1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	printf("You are using OpenGL %s\n",glGetString(GL_VERSION));

	uint vao,vbo,ebo;

	printf("Loadint Shaders...\n");
	uint shader=initShaders();
	printf("Loading Buffers...\n");
	initBuffers(1,&vao,&vbo,&ebo);
	printf("Loadint Textures...\n");
	initTextures(shader);
	printf("Done!\n");

	glViewport(0,0,width,height);

	glUseProgram(shader);
	glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER,vbo);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	while(!glfwWindowShouldClose(window))
	{
		update();
		glClearColor(0,0,1,1.F);
		glClear(GL_COLOR_BUFFER_BIT);
		bufferData(sz,squareVert);
		glDrawElements(GL_TRIANGLES,sz*6,GL_UNSIGNED_INT,0);
		glfwSwapBuffers(window);
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

