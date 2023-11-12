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

inline uint initShaders()
{
	const char*vert=getFile("default.vert");
	const char*frag=getFile("default.frag");

	uint vertS=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertS,1,&vertS,NULL);
	glCompileShader(vertS);

	uint fragS=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragS,1,&fragS,NULL);
	glCompileShader(fragS);

	uint pgm=glCreateProgram();
	glAttatchShader(pgm,vertS);
	glAttatchShader(pgm,fragS);
	glLinkProgram(pgm);

	glDeleteShader(vertS);
	glDeleteShader(fragS);

	glActivateProgram(pgm);

	return pgm;
}

#endif
