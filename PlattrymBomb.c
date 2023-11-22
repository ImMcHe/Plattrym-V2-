#pragma once
#ifndef __PLATTRYM_BOMB__
#define __PLATTRYM_BOMB__

#define SMALLBOMB 19
#define MIDBOMB 20
#define BIGBOMB 21
#define ABOMB 22

double bombSpawnChance;
#define bombMaxLen 1023
#define particleMaxLen 2047
#define powerUpMaxLen 511
uint bombLen,particleLen;
struct Bomb
{
	char type;
	double yPos;
	int xPos;
	int fuse;
}
bombs[bombMaxLen];
struct Particle
{
	char type;
	double rotation,xPos,yPos,xVel,yVel,rotationSpeed;
	int despawnTime;
}
particles[particleMaxLen];

static inline struct Bomb spawnBomb(char type,int xPos,int yPos)
{
	struct Bomb bomb;
	bomb.type=type;
	bomb.xPos=xPos;
	bomb.yPos=yPos;
	bomb.fuse=-1;
	return bomb;
}
static inline struct Particle spawnParticle(char type,double rotationSpeed,double xVel,double yVel,double xPos,double yPos,int despawnTime)
{
	struct Particle particle;
	particle.type=type;
	particle.rotation=-0.78539816339744830961566084581988;
	particle.rotationSpeed=rotationSpeed;
	particle.xPos=xPos;
	particle.yPos=yPos;
	particle.xVel=xVel;
	particle.yVel=yVel;
	particle.despawnTime=despawnTime;
	particle.xPos=particle.xPos>cx+mapW*.5?particle.xPos-mapW:particle.xPos<cx-mapW*.5?particle.xPos+mapW:particle.xPos;
	return particle;
}



static inline float getBlastPower(char bombType)
{
	switch(bombType)
	{
	case SMALLBOMB:
		return 3.F;
	case MIDBOMB:
		return 5.89F;
	case BIGBOMB:
		return 8.23F;
	case ABOMB:
		return 65.F;
	default:
		return 1.F;
	}
}


static inline void destroy(int xOg,int yOg,int xPos,int yPos,float*power,float damage,float multiplyer)
{
	if(*power<=0.f)
		return;
	char*mapPos=getMap(xPos,yPos);
	float blr=getBlr(*mapPos);
	if(*power>blr)
	{
		double dist=sqrt((xOg-xPos)*(xOg-xPos)+(yOg-yPos)*(yOg-yPos));
		dist=dist<.7?.7:dist;

		int pw=(int)ceil(*power*3.F);
		if(pw>72)
			pw=1;

		if(((xPos-(int)round(px))%mapW+mapW)%mapW==0&&yPos==(int)round(py))
			velX+=max(.5,*power*.05F/dist*(fmod(fmod(px,mapW)+mapW,mapW)<xOg?-1:1)),
			velY+=max(.5,*power*.05F/dist*(py<yOg?-1:1));

		if(*mapPos&&particleLen<particleMaxLen)
		{
			if(abs((int)round((xPos>cx+mapW*.5?xPos-mapW:xPos<cx-mapW*.5?xPos+mapW:xPos)-cx)%mapW)<80&&abs(yPos-cy)<50.)
				for(int i=0;i<pw;i++)
					if(particleLen<particleMaxLen)
						particles[particleLen++]=spawnParticle(*mapPos,rand()/(double)RAND_MAX*.1-.05,*power*.1F/dist*(xOg<xPos?1.:xOg==xPos?rand()/(double)RAND_MAX*2.-1.:-1.)+rand()/(double)RAND_MAX*.2-.1,*power*.1F/dist*(yOg<yPos?1:-1)+rand()/(double)RAND_MAX*.2-.1,xPos,yPos,rand()%750+250);
		}
		*power-=blr*multiplyer;
		*mapPos=AIR;
	}
	else
		*power-=blr*.7F+multiplyer;
	return;
}


static inline void explodeFwd(int xPos,int yPos,float power,float damage)
{
	destroy(xPos,yPos,xPos,yPos,&power,damage,.25F);
	float pl=power,pr=power;
	int i=0;
	while(pl>0.F||pr>0.F)
	{
		i++;
		destroy(xPos,yPos,xPos,yPos+i,&pl,damage,1.F);
		destroy(xPos,yPos,xPos,yPos-i,&pr,damage,1.F);
	}
	pl=power;pr=power;i=0;
	while(pl>0.F||pr>0.F)
	{
		i++;
		destroy(xPos,yPos,xPos+i,yPos,&pl,damage,1.F);
		destroy(xPos,yPos,xPos-i,yPos,&pr,damage,1.F);
		float pl1=pl,pl2=pl,pr1=pr,pr2=pr;
		int j=0;
		while(pl1>0.F||pl2>0.F||pr1>0.F||pr2>0.F)
		{
			j++;
			destroy(xPos,yPos,xPos+i,yPos+j,&pl1,damage,1.F);
			destroy(xPos,yPos,xPos+i,yPos-j,&pl2,damage,1.F);
			destroy(xPos,yPos,xPos-i,yPos+j,&pr1,damage,1.F);
			destroy(xPos,yPos,xPos-i,yPos-j,&pr2,damage,1.F);
		}
	}
}

static inline void explode(int xPos,int yPos,char type)
{
	if(type==ABOMB)
	{
		explodeFwd(xPos,yPos,getBlastPower(ABOMB),.5F);
		explodeFwd(xPos,yPos,getBlastPower(ABOMB),.5F);
		return;
	}
	explodeFwd(xPos,yPos,getBlastPower(type),1.F);
	return;
}


static inline void bombUpdate()
{
	bombSpawnChance+=.01;
	//bombSpawnChance+=100.;
	double newChance=pow(bombSpawnChance,.8);

	for(int i=0;i<mapW;i++)
	{
		while(!*getMap(i,mapHeights[i])&&mapHeights[i]>0)
			mapHeights[i]--;
	}

	if(
		rand()/(double)RAND_MAX*newChance>
		rand()/(double)RAND_MAX*(newChance+2000.)&&bombLen<bombMaxLen)
	{
		//printf("DEBUG");
		double randChance=rand()/(double)RAND_MAX*100.;
		int width=rand()/(double)RAND_MAX*mapW;
		//double height=mapH;

		bombs[bombLen++]=spawnBomb(
			(randChance<46.?SMALLBOMB:randChance<77.?MIDBOMB:BIGBOMB),
			width,
			max(mapHeights[width],py)+250
		);
	}

	if(getKeyDown(GLFW_KEY_SPACE)&&bombLen<bombMaxLen)
		bombs[bombLen++]=spawnBomb(ABOMB,((int)round(px)%mapW+mapW)%mapW,py+10.);

	if((rand()==0)&&bombLen<bombMaxLen)
	{
		int width=rand()/(double)RAND_MAX*mapW;

		bombs[bombLen++]=spawnBomb(
			ABOMB,
			width,
			max(mapHeights[width],py)+1000
		);
	}
	
	for(uint i=0;i<bombLen;i++)
	{
		bombs[i].yPos-=(bombs[i].type==SMALLBOMB?.1:bombs[i].type==MIDBOMB?.09:bombs[i].type==BIGBOMB?.07:.009);
		if(bombs[i].fuse!=-1)
			bombs[i].fuse+=1;
		else if(*getMap(bombs[i].xPos,round(bombs[i].yPos)))
			bombs[i].fuse=0;
	}
	for(uint i=0;i<particleLen;i++)
	{
		particles[i].yVel-=.0003;

		particles[i].xPos+=particles[i].xVel;
		particles[i].yPos+=particles[i].yVel;
		particles[i].despawnTime--;
		particles[i].rotation+=particles[i].rotationSpeed;

		if(*getMap(round(particles[i].xPos),round(particles[i].yPos)))
		{
			char xInt=0,yInt=0;
			particles[i].xPos-=particles[i].xVel;

			yInt=*getMap(round(particles[i].xPos),round(particles[i].yPos));

			particles[i].xPos+=particles[i].xVel;
			particles[i].yPos-=particles[i].yVel;

			xInt=*getMap(round(particles[i].xPos),round(particles[i].yPos));

			particles[i].xPos-=particles[i].xVel;

			particles[i].xVel=particles[i].xVel*.3;
			particles[i].yVel=particles[i].yVel*.3;
			if(xInt)
				particles[i].xVel=-particles[i].xVel;
			if(yInt)
				particles[i].yVel=-particles[i].yVel;
		}

		particles[i].xPos=particles[i].xPos>cx+mapW*.5?particles[i].xPos-mapW:particles[i].xPos<cx-mapW*.5?particles[i].xPos+mapW:particles[i].xPos;
		if(abs((int)round(particles[i].xPos-cx)%mapW)>80||abs(particles[i].yPos-cy)>50.)
			particles[i].despawnTime-=50;
	}

	int shiftW=0;
	for(int i=0;i<bombLen;i++)
	{
		char type=bombs[i].type;
		if(bombs[i].yPos<-50||bombs[i].fuse>=(type==SMALLBOMB?8:type==MIDBOMB?10:type==BIGBOMB?13:50))
			shiftW++,explode(bombs[i].xPos,round(bombs[i].yPos),type);
		else if(shiftW)
			bombs[i-shiftW]=bombs[i];
	}
	bombLen-=shiftW;

	shiftW=0;
	for(int i=0;i<particleLen;i++)
	{
		if(particles[i].despawnTime<=0)
			shiftW++;
		else if(shiftW)
			particles[i-shiftW]=particles[i];
	}
	particleLen-=shiftW;
}

#endif
