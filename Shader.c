#pragma once
#ifndef __SHADER__
#define __SHADER__

#include<stdio.h>
#include<stdlib.h>

inline const char*getFile(const char*fname)
{
	FILE*file=fopen(fname,"r");
	
	fseek(file,0,SEEK_END);
	size_t size=ftell(file)+1;
	char*ret=malloc(size);
	memset(ret,0,size);
	fseek(file,0,SEEK_SET);
	fgets(ret,size,file);
	fclose(file);

	return ret;
}

inline void initShader()
{

}

#endif
