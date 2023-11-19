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
	-.5,.5,  0,.95,
	.5,-.5,  .95,0,
	.5,.5,   .95,.95,
};
const uint squareInd[]={
	0,1,2,1,3,2
};
#define scSize 13345
uint scaleLocation,positionLocation;
float*vertecies;

double deltaTime;
clock_t timeBef;
static inline float getTextCoordX(char type)
{
	return type%8*.125F+.001F;
}
static inline float getTextCoordY(char type)
{
	return .875F-(type/8)*.125F+.001F;
}


const float width=1920/1920.F,height=1080/1920.F;
#define SCREEN_X GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_Y GetSystemMetrics(SM_CYSCREEN)
#define getKeyDown(__keyCode__)(glfwGetKey(window,__keyCode__)==GLFW_PRESS)


#include"Plattrym.c"


const size_t sz=scSize+bombMaxLen;
#include"Shader.c"
#include"Buffer.c"
#include"Texture.c"


static inline void update()
{
	//glUniform2f(scaleLocation,.03F,.03F);
	glUniform2f(scaleLocation,.01F,.01F);
	glUniform2f(positionLocation,cx,-cy);

	playerUpdate(deltaTime);

	for(int x=0;x<=156;x++)
		for(int y=0;y<=84;y++)
			for(size_t i=0;i<16;i+=4)
			{
				size_t idx=(y*157+x)*16+i;
				char mapTy=*getMap(x-78-(int)round(cx),y-42+(int)round(cy));
				vertecies[idx]=mapTy?squareVert[i]+x-(float)round(cx)-78.F:0;
				vertecies[idx+1]=mapTy?squareVert[i+1]+y+(float)round(cy)-42.F:0;
				vertecies[idx+2]=squareVert[i+2]*.125F+getTextCoordX(mapTy);
				vertecies[idx+3]=squareVert[i+3]*.125F+getTextCoordY(mapTy);
			}
	for(int x=0;x<bombLen;x++)
		for(size_t i=0;i<16;i+=4)
		{
			//printf("BRUH");
			size_t idx=(scSize+x)*16+i;
			char mapTy=bombs[x].type;
			//vertecies[idx]=squareVert[i]+bombs[x].xPos;
			//vertecies[idx+1]=squareVert[i]+(float)bombs[x].yPos;
			vertecies[idx]=squareVert[i];
			vertecies[idx+1]=squareVert[i];
			//printf("%f %f\n",vertecies[idx],vertecies[idx+1]);
			vertecies[idx+2]=squareVert[i+2]*.125F+getTextCoordX(mapTy);
			vertecies[idx+3]=squareVert[i+3]*.125F+getTextCoordY(mapTy);
		}
	for(int x=bombLen;x<bombMaxLen;x++)
		for(size_t i=0;i<16;i+=4)
		{
			size_t idx=(scSize+x)*16+i;
			vertecies[idx]=0.F;
			vertecies[idx+1]=0.F;
			vertecies[idx+2]=0.F;
			vertecies[idx+3]=0.F;
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
	generateMapNormal(2000,2000,600,1300,30,41,25,51);

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

