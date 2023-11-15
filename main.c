#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<math.h>
#include<glad/glad.h>
#include"glad.c"
#include<GLFW/glfw3.h>
#define uint unsigned int

GLFWwindow*window;
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
float*vertecies;

double deltaTime;
clock_t timeBef;

const float width=1920/1920.F,height=1080/1920.F;
#define SCREEN_X GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_Y GetSystemMetrics(SM_CYSCREEN)
#define getKeyDown(__keyCode__)(glfwGetKey(window,__keyCode__)==GLFW_PRESS)


#include"Shader.c"
#include"Buffer.c"
#include"Texture.c"
#include"Plattrym.c"


static inline void update()
{
	for(int i=0;i<(sizeof(squareVert)/sizeof(float));i++)
		vertecies[i]=squareVert[i];
	if(getKeyDown(GLFW_KEY_LEFT))
		for(int i=0;i<(sizeof(squareVert)/sizeof(float));i++)
			vertecies[i]=squareVert[i]+.5F;
	if(getKeyDown(GLFW_KEY_RIGHT))
		for(int i=0;i<(sizeof(squareVert)/sizeof(float));i++)
			vertecies[i]=squareVert[i]-.5F;
}


int main(int argc,char**argv)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,1);

	window=glfwCreateWindow(SCREEN_X,SCREEN_Y,"Plattrym (V2)",glfwGetPrimaryMonitor(),NULL);

	printf("Loading Window...\n");
	if(window==NULL)
	{
		printf("Failed to create window");
		getch();
		glfwTerminate();
		return-1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	//Tells opengGL version
	printf("You are using OpenGL %s\n",glGetString(GL_VERSION));

	uint vao,vbo,ebo;

	printf("Loadint Shaders...\n");
	uint shader=initShaders();
	printf("Loading Buffers...\n");
	initBuffers(1,&vao,&vbo,&ebo);
	printf("Loadint Textures...\n");
	initTextures(shader);
	printf("Done!\n");
	//Loading COMPLETE!!!

	glViewport(0,0,SCREEN_X,SCREEN_Y);
	vertecies=(float*)malloc(sizeof(squareVert)*sz);

	glUseProgram(shader);
	glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	timeBef=clock();

	//Generate Plattrym
	generateMapNormal(500,500,300,100,20,41,25);

	while(!glfwWindowShouldClose(window))
	{
		//DeltaTime
		clock_t curTime=clock();
		deltaTime=(curTime-timeBef)*.001;
		timeBef=curTime;

		update();
		glClearColor(0,0,1,1.F);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Buffer sub datas
		glBufferSubData(GL_ARRAY_BUFFER,0,sz*sizeof(squareVert),vertecies);

		glDrawElements(GL_TRIANGLES,sz*6,GL_UNSIGNED_INT,0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clearing up...
	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteProgram(shader);
	glDeleteBuffers(1,&vao);
	glDeleteBuffers(1,&vbo);
	glDeleteBuffers(1,&ebo);

    return 0;
}

