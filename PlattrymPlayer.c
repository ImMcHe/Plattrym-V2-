#pragma once
#ifndef __PLATTRYM_PLAYER__
#define __PLATTRYM_PLAYER__

#define PLAYER 15

char keyLeft,keyRight,keyUp;
double timeDelFactor=1.;


static inline void quit()
{
	glfwSetWindowShouldClose(window,1);
}


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
	for(uint i=0;i<powerUpLen;i++)
		if((((int)round(px))%mapW+mapW)%mapW==(((int)round(powerUp[i].xPos))%mapW+mapW)%mapW&&(int)round(py)==(int)round(powerUp[i].yPos)&&powerUp[i].despawnTime>150)
		{
			powerUp[i].despawnTime=150;

			if(powerUp[i].type==HEALTH)
				health+=50.;
			if(powerUp[i].type==SHIELD)
				shield=1;
			if(powerUp[i].type==SPEED)
				hype=100.;
			if(powerUp[i].type==JUMPBOOST)
				scBoost=100.;
			if(powerUp[i].type==TIMEDEL)
				timeDel=100;
			if(powerUp[i].type==PINKORB)
			{
				if(!shield)
					health+=50.;
				explode((int)round(px),(int)round(py),MIDBOMB);
			}
		}

	if(keyLeft)
		velX+=hype?.0045:.003;
	if(keyRight)
		velX-=hype?.0045:.003;

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
			velY=scBoost>0.?.1:.061;
		
		px+=velX;
		py+=velY;
	}
	if(isT||(keyLeft^keyRight))
		velX*=.93;
	else
		velX*=.998;

	scx=(scx-px)*.9981+px;
	scy=(scy-py)*.9981+py;
	
	scy=2*py-scy;
	scx=2*px-scx;

	scy+=7.;

	cx=(cx-scx)*.997+scx;
	cy=(cy-scy)*.997+scy;
}


static inline void fixedUpdate()
{
	move();

	scBoost-=.005;
	hype-=.005;
	timeDel-=.02;
	
	if(scBoost<0.)
		scBoost=0.;
	if(hype<0.)
		hype=0.;
	if(timeDel<0.)
		timeDel=0.;

	if(px>mapW*.5)
		px-=mapW,cx-=mapW,scx-=mapW;
	if(px<mapW*.5)
		px+=mapW,cx+=mapW,scx+=mapW;

	bombUpdate();

	health+=pow(regen,3.5)*.0000001;
	if(health>100.)
	{
		health=100.;regen=0.;
	}
#ifdef DEBUG
	health=100.;
#endif
	regen+=.0005F;
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
