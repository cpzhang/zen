#pragma once
#include "render/IndexBuffer.h"
#include "misc/stdHead.h"
#include "misc/helper.h"
#include "ConfigScene.h"
class ApiScene_ LOD
{
public:
	LOD();
	~LOD();
public:
	void create();
	void destroy();
	int getVerticesNumberOneChunk();
	int getVerticesNumberOneSide();
	IndexBuffer* getIndexBuffer();
	int getPrimitiveNumber();
	float getScale();
	void setScale(float s)
	{
		scale_ = s;
	}

	float getChunkSize()
	{
		return chunkSize_;
	}
	void setChunkSize(float f)
	{
		chunkSize_ = f;
		setScale(f / (nVerticesNumberOneSide_ - 1.0f));
	}
	float getLengthOneSide();
	int getN();
	void setN(int n);
private:
	void clear_();
private:
	int n_;
	float chunkSize_;
	float scale_;
	int nVerticesNumberOneChunk_;
	int nVerticesNumberOneSide_;
	int nPrimitiveNumberOneChunk_;
	IndexBuffer indexBuffers_;
};