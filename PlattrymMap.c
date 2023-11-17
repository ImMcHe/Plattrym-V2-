#pragma once
#ifndef __PLATTRYM_MAP__
#define __PLATTRYM_MAP__


int mapW,mapH;
char*map,isMallocedMap=0;
#define AIR 0
#define GRASS 1
#define DIRT 2
#define STONE 3
#define WHITESTONE 4
#define ORE 5
#define DIAMOND 6
#define GRAVEL 7
#define PURPLE 8

static inline float getBlr(char type)
{
	switch(type)
	{
	case AIR:
		return.34;
	case GRASS:
		return 2.;
	case DIRT:
		return 1.;
	case STONE:
		return 1.674;
	case WHITESTONE:
		return 6.768;
	case ORE:
		return 2.225;
	case DIAMOND:
		return 3.2;
	case GRAVEL:
		return.1;
	case PURPLE:
		return-2.;
	default:
		return 1.;
	}
}


static inline char*getMap(int x,int y)
{
	if(y<0||y>=mapH)
		return map+(mapW*mapH+1);
	return map+((x%mapW+mapW)%mapW*mapH+y);
}


static inline void mallocMap(uint mapWidth,uint mapHeight)
{
	srand(clock());
	if(isMallocedMap)
		isMallocedMap=0,free(map);
	map=(char*)malloc(mapWidth*mapHeight+1);
	memset(map,0,mapWidth*mapHeight+1);
	mapW=mapWidth,mapH=mapHeight;
	isMallocedMap=1;
}


static inline void generateMapNormal(uint mapWidth,uint mapHeight,uint diamondSpawnHeight,uint dirtHeight,uint gravelSpawnRate,uint diamondSpawnRate,uint oreSpawnRate)
{
	mallocMap(mapWidth,mapHeight);
	int curHeight=-1;
	int curHeightCh=0;
	for(uint x=0;x<mapWidth/2;x++)
	{
		for(uint y=0;y<mapHeight;y++)
		{
			//Diamond spawn simulation
			if(y<curHeight+dirtHeight+rand()%20-10)
				*getMap(x,y)=rand()%(y+1)==0?
					WHITESTONE:
					rand()%diamondSpawnRate==0&&diamondSpawnHeight>=y?
					rand()%2?
						DIAMOND:
						PURPLE:
					0==rand()%oreSpawnRate?
					ORE:
					0==rand()%gravelSpawnRate?
					GRAVEL:
					STONE;
			//Dirt spawn simulation
			else if(y<curHeight+mapHeight)
				*getMap(x,y)=rand()%gravelSpawnRate==0?GRAVEL:DIRT;
			//Grass spawn simulation
			else if(y==curHeight+mapHeight)
				*getMap(x,y)=GRASS;
			else
				*getMap(x,y)=AIR;
			*getMap(mapWidth-x-1,y)=*getMap(x,y);
		}
		curHeightCh=(curHeightCh<0?-1:curHeightCh==0?rand()?1:-1:1)*(rand()%(abs(curHeightCh)+1)==0?abs(curHeightCh)+1:abs(curHeightCh)-1);
		curHeight+=curHeightCh;
		if(curHeight>-1)
			curHeight=-1,curHeightCh=-1;
	}
}

#endif
