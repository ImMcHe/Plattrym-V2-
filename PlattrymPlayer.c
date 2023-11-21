#pragma once
#ifndef __PLATTRYM_PLAYER__
#define __PLATTRYM_PLAYER__

#define PLAYER 15

char keyLeft,keyRight,keyUp;


static inline char getTouch()
{
	double pxP=fmod(px,1);
	double pyP=fmod(py,1);
	pxP=pxP<0.?-pxP:pxP;
	pyP=pyP<0.?-pyP:pyP;

	char isXt=pxP>.1501&&pxP<.8499;
	char isYt=pyP>.1501&&pyP<.8499;

	if(isXt)
		if(isYt)
			return*getMap((int)floor(px),(int)floor(py))||*getMap((int)floor(px),(int)ceil(py))||*getMap((int)ceil(px),(int)floor(py))||*getMap((int)ceil(px),(int)ceil(py));
		else
			return*getMap((int)floor(px),(int)round(py))||*getMap((int)ceil(px),(int)round(py));
	else
		if(isYt)
			return*getMap((int)round(px),(int)floor(py))||*getMap((int)round(px),(int)ceil(py));
		else
			return*getMap((int)round(px),(int)round(py))||*getMap((int)round(px),(int)round(py));
}

static inline void move()
{
	if(keyLeft)
		velX+=.003;
	if(keyRight)
		velX-=.003;

	velY-=.0003;

	scy-=7.;

	scy=2*py-scy;
	scx=2*px-scx;

	px+=velX;
	py+=velY;

	char isT=getTouch();
	if(isT)
	{
		px-=velX;
		char isYt=getTouch();
		px+=velX;py-=velY;
		char isXt=getTouch();
		px-=velX;
		char isYVelLessThan0=velY<0.;
		
		if(isXt)
			if(isYt)
			{
				py=round(py)+(velY<0.?-.15:.15);
				velY=0.;
				px=round(px)+(velX<0.?-.15:.15);
				velX=0.;
			}
			else
			{
				px=round(px)+(velX<0.?-.15:.15);
				velX=0.;
			}
		else
			if(isYt)
			{
				py=round(py)+(velY<0.?-.15:.15);
				velY=0.;
			}
			else
			{
				px=round(px)+(velX<0.?-.15:.15);
				velX=0.;
			}

		if(keyUp&&isYt&&isYVelLessThan0)
			velY=.061;
		
		px+=velX;
		py+=velY;
	}
	if(isT||(keyLeft^keyRight))
		velX*=.93;
	else
		velX*=.992;

	scx=(scx-px)*.9971+px;
	scy=(scy-py)*.9971+py;
	
	scy=2*py-scy;
	scx=2*px-scx;

	scy+=7.;

	cx=(cx-scx)*.996+scx;
	cy=(cy-scy)*.996+scy;
}


static inline void fixedUpdate()
{
	move();

	if(px>mapW*.5)
		px-=mapW,cx-=mapW,scx-=mapW;
	if(px<mapW*.5)
		px+=mapW,cx+=mapW,scx+=mapW;

	bombUpdate();
}


static inline void playerUpdate(double deltaTime)
{
	prevDeltaTime+=deltaTime*400.;
	int numOfTimes=floor(prevDeltaTime);

	keyLeft=getKeyDown(GLFW_KEY_LEFT);
	keyRight=getKeyDown(GLFW_KEY_RIGHT);
	keyUp=getKeyDown(GLFW_KEY_UP);
	
	for(int i=0;i<numOfTimes;i++)
		fixedUpdate();

	prevDeltaTime=fmod(prevDeltaTime,1);
}


static inline void initPlayer()
{
	px=round(rand()/(double)RAND_MAX*mapW);
	py=mapHeights[(int)px]+1;
	cx=px;
	cy=py+7.;
	scx=cx;
	scy=cy;
}

#endif
