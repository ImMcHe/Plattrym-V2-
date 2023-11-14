#pragma once
#ifndef __SHADER__
#define __SHADER__

#include<stdio.h>
#include<stdlib.h>

static inline const char*getFile(const char*fname)
{
	FILE*file=fopen(fname,"r");
	
	fseek(file,0,SEEK_END);
	size_t size=ftell(file)+1;

	char*ret=malloc(size);
	memset(ret,0,size);
	fseek(file,0,SEEK_SET);

	size_t idx=0;
	char character=18;
	while((character=fgetc(file))!=EOF)
		ret[idx++]=character;

	fclose(file);

	return ret;
}

static inline uint initShaders()
{
	const char*vert=getFile("default.vert");
	const char*frag=getFile("default.frag");

	uint vertS=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertS,1,&vert,NULL);
	glCompileShader(vertS);

	uint fragS=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragS,1,&frag,NULL);
	glCompileShader(fragS);

	uint pgm=glCreateProgram();
	glAttachShader(pgm,vertS);
	glAttachShader(pgm,fragS);
	glLinkProgram(pgm);
	glUseProgram(pgm);

	glDeleteShader(vertS);
	glDeleteShader(fragS);

	return pgm;
}

#endif
