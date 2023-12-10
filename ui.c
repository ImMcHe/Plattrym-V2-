#pragma once
#ifndef __UI__
#define __UI__

float sc1,sc2;
#define uiLen 10


float prevHP,prevShield,prevSpeed,prevJumpBoost,prevTimeDel;
float hpDrop=1.F,speedDrop=1.F,shieldDrop=1.F,jumpBoostDrop=1.F,timeDelDrop=1.F;


static inline float wtsX(float x)
{
	return x/(sc1*-dist)+cx;
}
static inline float wtsY(float y)
{
	return y/(sc2*dist)+cy;
}


static inline void fixedUI()
{
	prevHP=prevHP+.012*(health-prevHP);
	prevShield=prevShield+.012F*(shield*100.F-prevShield);
	prevJumpBoost=prevJumpBoost+.012*(scBoost-prevJumpBoost);
	prevSpeed=prevSpeed+.012*(hype-prevSpeed);
	prevTimeDel=prevTimeDel+.012*(timeDel-prevTimeDel);

	if(health>0.F)
		hpDrop=max(0.F,hpDrop-.001F);
	else
		hpDrop=min(0.5F,hpDrop+.001F);
	if(shield)
		shieldDrop=max(0.F,shieldDrop-.001F);
	else
		shieldDrop=min(0.5F,shieldDrop+.001F);
	if(timeDel>0.F)
		timeDelDrop=max(0.F,timeDelDrop-.001F);
	else
		timeDelDrop=min(0.5F,timeDelDrop+.001F);
	if(scBoost>0.F)
		jumpBoostDrop=max(0.F,jumpBoostDrop-.001F);
	else
		jumpBoostDrop=min(0.5F,jumpBoostDrop+.001F);
	if(hype>0.F)
		speedDrop=max(0.F,speedDrop-.001F);
	else
		speedDrop=min(0.5F,speedDrop+.001F);

	red+=.01F;
	if(red>1.F)
		red=1.F;
}
static inline void renderUI(double deltaTime)
{
	int numOfTimes=floor(prevDeltaTime);

	for(int i=0;i<numOfTimes;i++)
		fixedUI();
	prevDeltaTime=fmod(prevDeltaTime,1);

	size_t pos=(sz-uiLen+1)*16;

	// -- Render HP -- 
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;
		float sc=(i==12||i==8?1.F:((float)(prevHP))*.02F-1.F);

		vertecies[i+pos]=wtsX(cos(rad)*sc*.15F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.015F-.9F-hpDrop*hpDrop);
		int b=abs(prevHP-health);
		vertecies[i+pos+2]=b<7?.8515625F:0.8984375F;
		vertecies[i+pos+3]=b<7?0.671875F:0.859375F;
	}
	pos-=16;
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;

		vertecies[i+pos]=wtsX(cos(rad)*.157F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.022F-.9F-hpDrop*hpDrop);
		vertecies[i+pos+2]=0.5F;
		vertecies[i+pos+3]=0.4609375F;
	}

	pos+=48;

	// -- Render Shield -- 
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;
		float sc=(i==12||i==8?1.F:((float)(prevShield))*.02F-1.F);

		vertecies[i+pos]=wtsX(cos(rad)*sc*.15F-.8F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.015F+.9F+shieldDrop*shieldDrop);
		int b=abs(prevShield-shield*100.F);
		vertecies[i+pos+2]=b<7?0.2890625F:0.8984375F;
		vertecies[i+pos+3]=b<7?0.4609375F:0.859375F;
	}
	pos-=16;
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;

		vertecies[i+pos]=wtsX(cos(rad)*.157F-.8F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.022F+.9F+shieldDrop*shieldDrop);
		vertecies[i+pos+2]=0.5F;
		vertecies[i+pos+3]=0.4609375F;
	}

	pos+=48;

	// -- Render Speed -- 
	//double bombSpawnChance,health,hype,timeDel,scBoost,shield,regen;
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;
		float sc=(i==12||i==8?1.F:((float)(prevSpeed))*.02F-1.F);

		vertecies[i+pos]=wtsX(cos(rad)*sc*.15F-.8F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.015F-.9F-speedDrop*speedDrop);
		int b=abs(prevSpeed-hype);
		vertecies[i+pos+2]=b<7?.4375F:0.8984375F;
		vertecies[i+pos+3]=b<7?.3984375F:0.859375F;
	}
	pos-=16;
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;

		vertecies[i+pos]=wtsX(cos(rad)*.157F-.8F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.022F-.9F-speedDrop*speedDrop);
		vertecies[i+pos+2]=0.5F;
		vertecies[i+pos+3]=0.4609375F;
	}

	pos+=48;

	// -- Render Jump Boost -- 
	//double bombSpawnChance,health,hype,timeDel,scBoost,shield,regen;
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;
		float sc=(i==12||i==8?1.F:((float)(prevJumpBoost))*.02F-1.F);

		vertecies[i+pos]=wtsX(cos(rad)*sc*.15F+.8F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.015F-.9F-jumpBoostDrop*jumpBoostDrop);
		int b=abs(prevJumpBoost-scBoost);
		vertecies[i+pos+2]=b<7?0.53125F:0.8984375F;
		vertecies[i+pos+3]=b<7?.40625F:0.859375F;
	}
	pos-=16;
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;

		vertecies[i+pos]=wtsX(cos(rad)*.157F+.8F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.022F-.9F-jumpBoostDrop*jumpBoostDrop);
		vertecies[i+pos+2]=0.5F;
		vertecies[i+pos+3]=0.4609375F;
	}

	pos+=48;

	// -- Render Time Delation -- 
	//double bombSpawnChance,health,hype,timeDel,scBoost,shield,regen;
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;
		float sc=(i==12||i==8?1.F:((float)(prevTimeDel))*.02F-1.F);

		vertecies[i+pos]=wtsX(cos(rad)*sc*.15F+.8F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.015F+.9F+timeDelDrop*timeDelDrop);
		int b=abs(timeDel-prevTimeDel);
		vertecies[i+pos+2]=b<7?.65625F:0.8984375F;
		vertecies[i+pos+3]=b<7?0.46875F:0.859375f;
	}
	pos-=16;
	for(size_t i=0;i<16;i+=4)
	{
		float rad=(i==0?0.F:i==4?1.5707963267948966192313216915F:i==8?4.7123889803846898576939650745F:3.141592653589793238462643383F)-0.78539816339744830961566084575;

		vertecies[i+pos]=wtsX(cos(rad)*.157F+.8F);
		vertecies[i+pos+1]=wtsY(sin(rad)*.022F+.9F+timeDelDrop*timeDelDrop);
		vertecies[i+pos+2]=0.5F;
		vertecies[i+pos+3]=0.4609375F;
	}
}

#endif

