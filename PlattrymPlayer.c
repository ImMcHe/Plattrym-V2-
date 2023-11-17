#pragma once
#ifndef __PLATTRYM_PLAYER__
#define __PLATTRYM_PLAYER__

double px,py,cx,cy,prevDeltaTime;


static inline void fixedUpdate()
{
	if(getKeyDown(GLFW_KEY_LEFT))
		cx+=.1;
	if(getKeyDown(GLFW_KEY_RIGHT))
		cx-=.1;
	if(getKeyDown(GLFW_KEY_UP))
		cy+=.1;
	if(getKeyDown(GLFW_KEY_DOWN))
		cy-=.1;
}


static inline void playerUpdate(double deltaTime)
{
	prevDeltaTime+=deltaTime*200.;
	int numOfTimes=floor(prevDeltaTime);
	
	for(int i=0;i<numOfTimes;i++)
		fixedUpdate();

	prevDeltaTime=fmod(prevDeltaTime,1);
}


#endif
