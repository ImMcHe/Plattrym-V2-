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
#define SNOW 9 
#define ICE 10
#define VALLEYGRASS 11
#define VALLEYDIRT 12
#define NORMALGRASS 13
#define NORMALDIRT 14
#define HARDICE 16
#define HARDVALLEY 17
#define NORMALGRAVEL 18


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
	case SNOW:
		return .8;
	case ICE:
		return 1.5;
	case HARDICE:
		return 1.85;
	case VALLEYGRASS:
		return .98;
	case VALLEYDIRT:
		return .69;
	case HARDVALLEY:
		return 2.12;
	case NORMALGRASS:
		return 2.05;
	case NORMALDIRT:
		return 1.05;
	case NORMALGRAVEL:
		return.15;
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
	int curHeight=-250;
	int curHeightCh=0;
	char mapType=0;

	int*mapHeights=(int*)malloc(sizeof(int)*mapWidth);
	memset(mapHeights,0,sizeof(int)*mapWidth);
	char*biomes=(char*)malloc(mapWidth);

	for(uint x=0;x<mapWidth/2;x++)
	{
		if(rand()%99==0)
			mapType=rand()%4;
		if(curHeight>(mapType==1?-101:mapType==2?74:-1))
			curHeight=mapType==1?-101:mapType==2?74:-1,curHeightCh=-1;
		curHeightCh=(curHeightCh<0?-1:curHeightCh==0?rand()?1:-1:1)*(rand()%(abs(curHeightCh)+(mapType==2?5:2))==0?abs(curHeightCh)+1:abs(curHeightCh)-1);
		curHeight+=curHeightCh*(mapType==1?3:1);
		mapHeights[x]=curHeight+(mapType==1?100:mapType==2?-75:0);
		biomes[x]=mapType;
	}

	for(uint x=0;x<mapWidth/2;x++)
	{
		int bruh=0;
		for(int i=-5;i<6;i++)
			bruh+=mapHeights[(((int)x+)%(int)mapWidth+(int)mapWidth)%(int)mapWidth];
		mapHeights[x]=bruh/11;
	}
	for(uint x=0;x<mapWidth/2;x++)
		mapHeights[mapWidth-x-1]=mapHeights[x],biomes[mapWidth-x-1]=biomes[x];

	for(uint x=0;x<mapWidth;x++)
	{
		for(uint y=0;y<mapHeight;y++)
		{
			int curHeight=mapHeights[x];
			int rndShift=0;
			while(rand()%3)
				rndShift++;
			rndShift=rand()%2?rndShift:-rndShift;
			char mapType=biomes[(((int)x+rndShift)%(int)mapWidth+(int)mapWidth)%(int)mapWidth];
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
				*getMap(x,y)=rand()%gravelSpawnRate==0?(mapType==0?GRAVEL:mapType==1?HARDICE:mapType==2?HARDVALLEY:mapType==3?NORMALGRAVEL:AIR):(mapType==0?DIRT:mapType==1?ICE:mapType==2?VALLEYDIRT:mapType==3?NORMALDIRT:AIR);
			//Grass spawn simulation
			else if(y==curHeight+mapHeight)
				*getMap(x,y)=mapType==0?GRASS:mapType==1?SNOW:mapType==2?VALLEYGRASS:mapType==3?NORMALGRASS:AIR;
			else
				*getMap(x,y)=AIR;
		}
	}

	free(mapHeights);
	free(biomes);
}

#endif
