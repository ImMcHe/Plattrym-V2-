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
	-.5,.5,  0,.98,
	.5,-.5,  .98,0,
	.5,.5,   .98,.98,
};
const uint squareInd[]={
	0,1,2,1,3,2
};
#define scSize 21037
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


const size_t sz=1+scSize+bombMaxLen+particleMaxLen+powerUpMaxLen;
#include"Shader.c"
#include"Buffer.c"
#include"Texture.c"


static inline void update()
{
	playerUpdate(!getKeyDown(GLFW_KEY_K)?deltaTime*.01:deltaTime);

	float dist=pow((cx-scx)*(cx-scx)+(cy-scy)*(cy-scy)+49.,.3)*.06+1.;
	glUniform2f(scaleLocation,-.03F/dist,.03F/dist);
	//glUniform2f(scaleLocation,-.01F,.01F);
	glUniform2f(positionLocation,-cx,-cy);

	// --- Clear Color ---
	float colR=0.F,colG=0.F,colB=0.F;
	for(int i=-50;i<51;i++)
	{
		//printf("%i\n",((i+(int)floor(cx))%mapW+mapW)%mapW);
		char biome=biomes[((i+(int)floor(cx))%mapW+mapW)%mapW];
		float ratio=(i==-20?fmod(cx,1):i==20?1.-fmod(cx,1):1.F);
		float colRInc=biome==0?0.F:biome==1?0.F:biome==2?.05F:1.F;
		float colGInc=biome==0?.6F:biome==1?.1F:biome==2?.02F:.5F;
		float colBInc=biome==0?1.F:biome==1?.7F:biome==2?.1F:0.F;
		colR+=colRInc*ratio;colG+=colGInc*ratio;colB+=colBInc*ratio;
	}
	colR=colR*.01F*(float)(cy/mapH)+(cy<mapH*.1?(float)(mapH*.1-cy)/mapH:0.F);
	colG=colG*.01F*(float)(cy/mapH);
	colB=colB*.01F*(float)(cy/mapH);

	glClearColor(colR,colG,colB,1.F);

	// --- Render Player ---
	for(size_t i=0;i<16;i+=4)
	{
		vertecies[i]=squareVert[i]*.8F+(float)px;
		vertecies[i+1]=squareVert[i+1]*.8F+(float)py;
		vertecies[i+2]=squareVert[i+2]*.125F+getTextCoordX(PLAYER);
		vertecies[i+3]=squareVert[i+3]*.125F+getTextCoordY(PLAYER);
	}

	// --- Render Map ---
	for(int x=0;x<=192;x++)
		for(int y=0;y<=108;y++)
			for(size_t i=0;i<16;i+=4)
			{
				size_t idx=(y*193+x)*16+i+16;
				char mapTy=*getMap(x-96+(int)round(cx),y-54+(int)round(cy));
				vertecies[idx]=mapTy?squareVert[i]+x+(float)round(cx)-96.F:0;
				vertecies[idx+1]=mapTy?squareVert[i+1]+y+(float)round(cy)-54.F:0;
				vertecies[idx+2]=squareVert[i+2]*.125F+getTextCoordX(mapTy);
				vertecies[idx+3]=squareVert[i+3]*.125F+getTextCoordY(mapTy);
			}
	
	// --- Render Bomb ---
	for(int x=0;x<bombLen;x++)
		for(size_t i=0;i<16;i+=4)
		{
			//printf("BRUH");
			size_t idx=(scSize+x)*16+i+16;
			char mapTy=bombs[x].type;
			vertecies[idx]=squareVert[i]*((mapTy==SMALLBOMB?.5F:mapTy==MIDBOMB?1.F:1.634F)+(bombs[x].fuse==-1?0.F:bombs[x].fuse*.05F))+(bombs[x].xPos+(bombs[x].xPos>cx+mapW*.5?-mapW:bombs[x].xPos<cx-mapW*.5?mapW:0));
			vertecies[idx+1]=squareVert[i+1]*((mapTy==SMALLBOMB?.5F:mapTy==MIDBOMB?1.F:1.634F)+(bombs[x].fuse==-1?0.F:bombs[x].fuse*.05F))+(float)bombs[x].yPos;
			//vertecies[idx]=squareVert[i];
			//vertecies[idx+1]=squareVert[i+1];
			//printf("%f %f\n",vertecies[idx],vertecies[idx+1]);
			vertecies[idx+2]=squareVert[i+2]*.125F+getTextCoordX(mapTy);
			vertecies[idx+3]=squareVert[i+3]*.125F+getTextCoordY(mapTy);
		}
	for(int x=bombLen;x<bombMaxLen;x++)
		for(size_t i=0;i<16;i+=4)
		{
			size_t idx=(scSize+x)*16+i+16;
			vertecies[idx]=0.F;
			vertecies[idx+1]=0.F;
			vertecies[idx+2]=0.F;
			vertecies[idx+3]=0.F;
		}

	// --- Render Particles --- 
	for(int x=0;x<particleLen;x++)
		for(size_t i=0;i<16;i+=4)
		{
			size_t idx=((scSize+bombMaxLen)+x)*16+i+16;
			char mapTy=particles[x].type;
			vertecies[idx]=cos(particles[x].rotation+(i==0?0.:i==4?1.5707963267948966192313216916398:i==12?3.1415926535897932384626433832795:4.7123889803846898576939650749193))*0.00471404520791031682933896241403*(particles[x].despawnTime>150?150:particles[x].despawnTime)+particles[x].xPos; // i * ((PI * 2) / 16) and sin(45)
			vertecies[idx+1]=sin(particles[x].rotation+(i==0?0.:i==4?1.5707963267948966192313216916398:i==12?3.1415926535897932384626433832795:4.7123889803846898576939650749193))*0.00471404520791031682933896241403*(particles[x].despawnTime>150?150:particles[x].despawnTime)+particles[x].yPos; // i * ((PI * 2) / 16)
			vertecies[idx+2]=squareVert[i+2]*.125F+getTextCoordX(mapTy);
			vertecies[idx+3]=squareVert[i+3]*.125F+getTextCoordY(mapTy);
		}
	for(int x=particleLen;x<particleMaxLen;x++)
		for(size_t i=0;i<16;i+=4)
		{
			size_t idx=((scSize+bombMaxLen)+x)*16+i+16;
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
	generateMapNormal(2000,2000,600,1300,30,41,25,71);
	initPlayer();

	while(!glfwWindowShouldClose(window))
	{
		//DeltaTime
		clock_t curTime=clock();
		deltaTime=min(1.,(curTime-timeBef)*.001);
		timeBef=curTime;

		update();
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

