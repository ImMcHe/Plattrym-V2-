#pragma once
#ifndef __PLATTRYM_BOMB__
#define __PLATTRYM_BOMB__

#define SMALLBOMB 19
#define MIDBOMB 20
#define BIGBOMB 21
#define ABOMB 22

double bombSpawnChance,health,hype,timeDel,scBoost,shield,regen;
#define bombMaxLen 1023
#define particleMaxLen 12000
#define powerUpMaxLen 511
uint bombLen,particleLen;
float red;
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
		{
			health-=damage**power;
			regen=0.;
			red=0.F;
			double newPx=fmod(fmod(px,mapW)+mapW,mapW);
			if(newPx-xOg>mapW/2)
				newPx-=mapW;
			if(newPx-xOg<mapW/2)
				newPx+=mapW;

			double xcf=max(-.25,min(.25,*power*.07F/dist*(newPx<(double)xOg?-1:1)));
			double ycf=max(-.25,min(.25,*power*.08F/dist*(py<yOg?-1:1)));
			velX+=xcf;
			velY+=ycf;
			scx+=xcf*250.;
			scy+=ycf*225.;
			cx-=xcf*75.;
			cy-=ycf*50.;
		}

		if(*mapPos&&particleLen<particleMaxLen)
		{
			if(abs((int)round((xPos>cx+mapW*.5?xPos-mapW:xPos<cx-mapW*.5?xPos+mapW:xPos)-cx)%mapW)<80&&abs(yPos-cy)<50.)
				if(damage==4.456F)
					particles[particleLen++]=spawnParticle(*mapPos,rand()/(double)RAND_MAX*.1-.05,(xOg<xPos?1.:xOg==xPos?rand()/(double)RAND_MAX*2.-1.:-1.)/log2(1.+dist)+rand()/(double)RAND_MAX*.2-.1,(yOg<yPos?1:-1)/log2(1.+dist)+rand()/(double)RAND_MAX*.2-.1,xPos,yPos,rand()%750+250);
				else
					for(int i=0;i<pw;i++)
						if(particleLen<particleMaxLen)
							particles[particleLen++]=spawnParticle(*mapPos,rand()/(double)RAND_MAX*.1-.05,*power*.1F/dist*(xOg<xPos?1.:xOg==xPos?rand()/(double)RAND_MAX*2.-1.:-1.)+rand()/(double)RAND_MAX*.2-.1,*power*.1F/dist*(yOg<yPos?1:-1)+rand()/(double)RAND_MAX*.2-.1,xPos,yPos,rand()%750+250);
		}
		*power-=blr*multiplyer;
		*mapPos=AIR;
	}
	else
		*power-=blr*.7F*multiplyer;
	return;
}


static inline void explodeFwd(int xPos,int yPos,float power,float damage)
{
	destroy(xPos,yPos,xPos,yPos,&power,damage,.5F);
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
		for(int x=-120;x<=120;x++)
			for(int y=-120;y<=120;y++)
			{
				char ty=*getMap(x+xPos,y+yPos);
				char lvl=ABOMBLVL7-round(sqrt(x*x+y*y)/18);
				if(lvl>ABOMBLVL7||lvl<ABOMBLVL1)
					lvl=AIR;
				if(ty==AIR||(ty>=ABOMBLVL1&&ty<=ABOMBLVL7?ty<lvl:0))
					*getMap(x+xPos,y+yPos)=lvl;
			}
		explodeFwd(xPos,yPos,getBlastPower(ABOMB),4.456F);
		explodeFwd(xPos,yPos,getBlastPower(ABOMB),4.456F);
		for(int x=-120;x<=120;x++)
			for(int y=-120;y<=120;y++)
			{
				char ty=*getMap(x+xPos,y+yPos);
				if(ty>=ABOMBLVL1&&ty<=ABOMBLVL7)
					*getMap(x+xPos,y+yPos)=AIR;
			}
		return;
	}
	explodeFwd(xPos,yPos,getBlastPower(type),type==BIGBOMB?12.7F:16.9F);
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
		int width=rand()*mapW/(double)RAND_MAX;
		//double height=mapH;

		bombs[bombLen++]=spawnBomb(
			(randChance<46.?SMALLBOMB:randChance<77.?MIDBOMB:BIGBOMB),
			width,
			max(mapHeights[width],py)+250
		);
	}

	//char bruh=(getKeyDown(GLFW_KEY_SPACE));
	char bruh=0;
	if((rand()<2||bruh)&&bombLen<bombMaxLen)
	{
		int width=rand()*mapW/(double)RAND_MAX;

		bombs[bombLen++]=spawnBomb(
			ABOMB,
			bruh?round(px):width,
			bruh?py+50:max(mapHeights[width],py)+1000
		);
	}
	
	for(uint i=0;i<bombLen;i++)
	{
		bombs[i].yPos-=(bombs[i].type==SMALLBOMB?.1:bombs[i].type==MIDBOMB?.09:bombs[i].type==BIGBOMB?.07:.0045);
		if(bombs[i].fuse!=-1)
			bombs[i].fuse+=1;
		else if(*getMap(bombs[i].xPos,round(bombs[i].yPos))||(((int)round(px)%mapW+mapW)%mapW==bombs[i].xPos&&(int)round(py)==(int)round(bombs[i].yPos)))
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
		if(bombs[i].yPos<-50||bombs[i].fuse>=(type==SMALLBOMB?8:type==MIDBOMB?10:type==BIGBOMB?13:100))
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
