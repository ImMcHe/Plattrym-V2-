#pragma once
#ifndef __BUFFER__
#define __BUFFER__

#include<stdlib.h>

static inline void initBuffers(size_t sz,uint*vao_p,uint*vbo_p,uint*ebo_p)
{
	uint vao,vbo,ebo;
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
	glGenBuffers(1,&ebo);

	//mallocs the temp indices
	uint*tmpInd=(uint*)malloc(sz*sizeof(squareInd));
	for(size_t i=0;i<sz;i++)
		for(size_t j=0;j<6;j++)
			tmpInd[i*6+j]=squareInd[j]+i*4;

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sz*sizeof(squareVert),(float*)0,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sz*sizeof(squareInd),tmpInd,GL_STATIC_DRAW);

	glVertexAttribPointer(0,2,GL_FLOAT,0,4*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,2,GL_FLOAT,0,4*sizeof(float),(void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	*vao_p=vao;
	*vbo_p=vbo;
	*ebo_p=ebo;

	//frees the temp indices
	free(tmpInd);

	glBindBuffer(GL_ARRAY_BUFFER,vbo);
}

#endif
