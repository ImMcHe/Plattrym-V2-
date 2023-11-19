#pragma once
#ifndef __PLATTRYM_PLAYER__
#define __PLATTRYM_PLAYER__

#define PLAYER 15

double px,py,cx,cy,prevDeltaTime;


static inline void fixedUpdate()
{
	if(getKeyDown(GLFW_KEY_LEFT))
		cx+=.3;
	if(getKeyDown(GLFW_KEY_RIGHT))
		cx-=.3;
	if(getKeyDown(GLFW_KEY_UP))
		cy+=.3;
	if(getKeyDown(GLFW_KEY_DOWN))
		cy-=.3;
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
