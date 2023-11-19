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
const size_t scSize=(78*2+1)*(42*2+1);
const size_t sz=(78*2+1)*(42*2+1);
uint scaleLocation,positionLocation;
float*vertecies;

double deltaTime;
clock_t timeBef;
static inline float getTextCoordX(char type)
{
	return type%8*.125F;
}
static inline float getTextCoordY(char type)
{
	return .875F-(type/8)*.125F;
}


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
	glUniform2f(scaleLocation,.03F,.03F);
	glUniform2f(positionLocation,cx,-cy);

	playerUpdate(deltaTime);

	for(int x=0;x<=156;x++)
		for(int y=0;y<=84;y++)
			for(size_t i=0;i<16;i+=4)
			{
				size_t idx=(y*157+x)*16+i;
				char mapTy=*getMap(x-36-(int)round(cx),y-21+(int)round(cy));
				vertecies[idx]=squareVert[i]+x-(float)round(cx)-78.F;
				vertecies[idx+1]=squareVert[i+1]+y+(float)round(cy)-42.F;
				vertecies[idx+2]=squareVert[i+2]*.125F+getTextCoordX(mapTy);
				vertecies[idx+3]=squareVert[i+3]*.125F+getTextCoordY(mapTy);
			}
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
	initBuffers(sz,&vao,&vbo,&ebo);
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
	generateMapNormal(2000,1000,300,600,30,41,25);

	while(!glfwWindowShouldClose(window))
	{
		//DeltaTime
		clock_t curTime=clock();
		deltaTime=min(1.,(curTime-timeBef)*.001);
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

