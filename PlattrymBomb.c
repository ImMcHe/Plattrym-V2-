#pragma once
#ifndef __PLATTRYM_BOMB__
#define __PLATTRYM_BOMB__

#define SMALLBOMB 19
#define MIDBOMB 20
#define BIGBOMB 21
#define ABOMB 22

static inline float getBlastPower(char bombType)
{
	switch(bombType)
	{
	case SMALLBOMB:
		return 5.F;
	case MIDBOMB:
		return 7.89F;
	case BIGBOMB:
		return 11.23F;
	case ABOMB:
		return 50.F;
	default:
		return 1.F;
	}
}


static inline void destroy(int xPos,int yPos,float*power,float damage)
{
	if(*power<=0.F)
		return;
	char*mapPos=getMap(xPos,yPos);
	float blr=getBlr(*mapPos);
	if(*power>blr)
	{
		*power-=blr;
		*mapPos=AIR;
	}
	else
		*power-=blr*.7F;
	return;
}


static inline void explode(int xPos,int yPos,float power)
{
	xPos=-xPos;
	float damage=0;
	destroy(xPos,yPos,&power,damage);
	float pl=power,pr=power;
	int i=0;
	while(pl>0&&pr>0)
	{
		i++;
		destroy(xPos,yPos+i,&pl,damage);
		destroy(xPos,yPos-i,&pr,damage);
	}
	pl=power;pr=power;i=0;
	while(pl>0&&pr>0)
	{
		i++;
		destroy(xPos+i,yPos,&pl,damage);
		destroy(xPos-i,yPos,&pr,damage);
		float pl1=pl,pl2=pl,pr1=pr,pr2=pr;
		int j=0;
		while(pl1>0&&pl2>0&&pr1>0&&pr2>0)
		{
			j++;
			destroy(xPos+i,yPos+j,&pl1,damage);
			destroy(xPos+i,yPos-j,&pl2,damage);
			destroy(xPos-i,yPos+j,&pr1,damage);
			destroy(xPos-i,yPos-j,&pr2,damage);
		}
	}
}

#endif
