#pragma once
#ifndef __PLATTRYM_PLAYER__
#define __PLATTRYM_PLAYER__

#define PLAYER 15

double px,py,cx=0,cy=1400,prevDeltaTime;
char isBlastDown=0;


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
	char isPress=getKeyDown(GLFW_KEY_SPACE);
	if(isPress&&!isBlastDown)
		explode(cx,cy,getBlastPower(BIGBOMB));
	isBlastDown=isPress;
}


static inline void playerUpdate(double deltaTime)
{
	prevDeltaTime+=deltaTime*400.;
	int numOfTimes=min(3200,floor(prevDeltaTime));
	
	for(int i=0;i<numOfTimes;i++)
		fixedUpdate();

	prevDeltaTime=fmod(prevDeltaTime,1);
}


#endif
