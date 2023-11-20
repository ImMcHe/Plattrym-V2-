#pragma once
#ifndef __PLATTRYM_BOMB__
#define __PLATTRYM_BOMB__

#define SMALLBOMB 19
#define MIDBOMB 20
#define BIGBOMB 21
#define ABOMB 22

double bombSpawnChance;
#define bombMaxLen 2500
#define particleMaxLen 2500
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
	double rotation,xPos,yPos;
}
particles[particleMaxLen];

static inline struct Bomb spawnBomb(char type,int xPos,double yPos)
{
	struct Bomb bomb;
	bomb.type=type;
	bomb.xPos=xPos;
	bomb.yPos=yPos;
	bomb.fuse=-1;
	return bomb;
}
static inline struct Particle spawnParticle(char type,double rotation,double xPos,double yPos)
{
	struct Particle particle;
	particle.type=type;
	particle.rotation=rotation;
	particle.xPos=xPos;
	particle.yPos=yPos;
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
		return 50.F;
	default:
		return 1.F;
	}
}


static inline void destroy(int xPos,int yPos,float*power,float damage,float multiplyer)
{
	if(*power<=0.F)
		return;
	char*mapPos=getMap(xPos,yPos);
	float blr=getBlr(*mapPos);
	if(*power>blr)
	{
		*power-=blr*multiplyer;
		*mapPos=AIR;
	}
	else
		*power-=blr*.7F+multiplyer;
	return;
}


static inline void explodeFwd(int xPos,int yPos,float power,float damage)
{
	destroy(xPos,yPos,&power,damage,.25F);
	float pl=power,pr=power;
	int i=0;
	while(pl>0.F||pr>0.F)
	{
		i++;
		destroy(xPos,yPos+i,&pl,damage,1.F);
		destroy(xPos,yPos-i,&pr,damage,1.F);
	}
	pl=power;pr=power;i=0;
	while(pl>0.F||pr>0.F)
	{
		i++;
		destroy(xPos+i,yPos,&pl,damage,1.F);
		destroy(xPos-i,yPos,&pr,damage,1.F);
		float pl1=pl,pl2=pl,pr1=pr,pr2=pr;
		int j=0;
		while(pl1>0.F||pl2>0.F||pr1>0.F||pr2>0.F)
		{
			j++;
			destroy(xPos+i,yPos+j,&pl1,damage,1.F);
			destroy(xPos+i,yPos-j,&pl2,damage,1.F);
			destroy(xPos-i,yPos+j,&pr1,damage,1.F);
			destroy(xPos-i,yPos-j,&pr2,damage,1.F);
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

	if(
		rand()/(double)RAND_MAX*newChance>
		rand()/(double)RAND_MAX*(newChance+2000.)&&bombLen<bombMaxLen)
	{
		//printf("DEBUG");
		double randChance=rand()/(double)RAND_MAX*100.;
		int width=rand()/(double)RAND_MAX*mapW;
		//double height=mapH;
		double height=mapH+100.;

		bombs[bombLen++]=spawnBomb(
			(randChance<46.?SMALLBOMB:randChance<77.?MIDBOMB:BIGBOMB),
			width,
			height
		);
	}
	if((rand()==0||getKeyDown(GLFW_KEY_SPACE))&&bombLen<bombMaxLen)
	{
		int width=rand()/(double)RAND_MAX*mapW;
		double height=mapH;

		bombs[bombLen++]=spawnBomb(
			ABOMB,
			width,
			height
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

	int shiftW=0;
	for(int i=0;i<bombLen;i++)
	{
		char type=bombs[i].type;
		if(bombs[i].yPos<-50||bombs[i].fuse>=(type==SMALLBOMB?8:type==MIDBOMB?10:type==BIGBOMB?13:50))
			shiftW++,explode(bombs[i].xPos,round(bombs[i].yPos),type);
		bombs[i]=bombs[i+shiftW];
	}
	bombLen-=shiftW;
}

#endif
