#pragma once
#ifndef __TEXTURE__
#define __TEXTURE__

#define STB_IMAGE_IMPLEMENTATION
#include<stbi/image.h>
#include<glad/glad.h>

static inline void initTextures(uint shaderId)
{
	//Init textures!
	uint textuni=glGetUniformLocation(shaderId,"mainTex"),texture;
	glGenTextures(1,&texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

	unsigned char*bytes;
	int w,h,numColCh;
	//Otherwise we are all gonna go down there :sob:
	stbi_set_flip_vertically_on_load(1);
	bytes=stbi_load("main.png",&w,&h,&numColCh,4);
	bytes[1]=0;

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUseProgram(shaderId);
	glUniform1i(textuni,0);

	stbi_image_free(bytes);
}

#endif
