#include "LOD.h"
#include "render/rendercontext.h"
#include "render/BufferLocker.h"
LOD::LOD()
{
	clear_();
}

LOD::~LOD()
{

}

void LOD::create()
{
	static const double base = 2;
	double m = pow(base, n_) + 1;
	nVerticesNumberOneSide_ = m;
	nVerticesNumberOneChunk_ = m * m;
	nPrimitiveNumberOneChunk_ = (m - 1) * (m - 1) * 2; 
	int level = 1;
	//
	indexBuffers_.create(nPrimitiveNumberOneChunk_ * 3* 2, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED);
	//
	std::vector<short> gIndices;
	for (int j = 0; j < m  - 1;j += level)
	for (int i = 0; i < m  - 1;i += level)
	{
		short baseIndex = i + j * m;
		gIndices.push_back(baseIndex);
		gIndices.push_back(baseIndex + m*level);
		gIndices.push_back(baseIndex + level);

		gIndices.push_back(baseIndex + m*level);
		gIndices.push_back(baseIndex + level + m*level);
		gIndices.push_back(baseIndex + level);
	}
	BufferLocker<IndexBuffer, u16> vl(indexBuffers_);
	vl.fill(&gIndices[0], gIndices.size()*2);
}

void LOD::clear_()
{
	n_ = 5;
	nVerticesNumberOneChunk_= 0;
	nVerticesNumberOneSide_ = 0 ;
	nPrimitiveNumberOneChunk_ = 0;
	scale_ = 3;
}

int LOD::getVerticesNumberOneChunk()
{
	return nVerticesNumberOneChunk_;
}

void LOD::destroy()
{
	indexBuffers_.destroy();
}

IndexBuffer* LOD::getIndexBuffer( )
{
	return &indexBuffers_;
}

int LOD::getPrimitiveNumber( )
{
	return nPrimitiveNumberOneChunk_;
}

int LOD::getVerticesNumberOneSide()
{
	return nVerticesNumberOneSide_;
}

float LOD::getScale()
{
	return scale_;
}

float LOD::getLengthOneSide()
{
	return (nVerticesNumberOneSide_ - 1) * scale_;
}

int LOD::getN()
{
	return n_;
}

void LOD::setN( int n )
{
	n_ = n;
}
