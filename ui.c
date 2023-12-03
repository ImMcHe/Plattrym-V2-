#pragma once
#ifndef __UI__
#define __UI__

float sc1,sc2;
#define uiLen 2


float prevHP;


static inline float wtsX(float x)
{
	return x/(sc1*-dist)+cx;
}
static inline float wtsY(float y)
{
	return y/(sc2*dist)+cy;
}


static inline void fixedUI()
{
	prevHP=prevHP+.012*(health-prevHP);
	red+=.01F;
	if(red>1.F)
		red=1.F;
}
static inline void renderUI(double deltaTime)
{
	int numOfTimes=floor(prevDeltaTime);

	for(int i=0;i<numOfTimes;i++)
		fixedUI();
	prevDeltaTime=fmod(prevDeltaTime,1);

	size_t pos=(sz-uiLen+1)*16;

	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;
		float sc=(i==12||i==8?1.F:((float)(prevHP))*.02F-1.F);

		vertecies[i+pos]=wtsX(cos(rad)*sc*.2F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.015F-.9F);
		int b=abs(prevHP-health);
		vertecies[i+pos+2]=b<7?0.5390625F:0.8984375F;
		vertecies[i+pos+3]=b<7?0.4609375F:0.859375F;
	}
	pos-=16;
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;

		vertecies[i+pos]=wtsX(cos(rad)*.207F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.022F-.9F);
		vertecies[i+pos+2]=0.5F;
		vertecies[i+pos+3]=0.4609375F;
	}
}

#endif
