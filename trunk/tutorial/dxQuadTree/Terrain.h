#pragma once
#include "stdHead.h"
#include "effectMaterial.h"
int getVerticesNumberOneSide(int maxLod)
{
	int verticesNum = pow((double)2, (double)maxLod) + 1;
	return verticesNum;
}
int getVerticesNumberAll(int maxLod)
{
	int verticesNum = getVerticesNumberOneSide(maxLod);
	return verticesNum * verticesNum;
}
int getTriangleNumberAll(int maxLod)
{
	int verticesNum = getVerticesNumberOneSide(maxLod);
	--verticesNum;
	return verticesNum*verticesNum*2;
}
void generateLOD(int index, IDirect3DIndexBuffer9* ib)
{
	std::vector<short> indices;
	int lod = pow((double)2, (long)index);
	int verticesNum = getVerticesNumberOneSide(gMaxLOD);
	for (int j = 0; j < verticesNum - 1;j += lod)
		for (int i = 0; i < verticesNum - 1;i += lod)
		{
			short baseIndex = i + j * verticesNum;
			indices.push_back(baseIndex);
			indices.push_back(baseIndex + verticesNum*lod);
			indices.push_back(baseIndex + lod);

			indices.push_back(baseIndex + verticesNum*lod);
			indices.push_back(baseIndex + lod + verticesNum*lod);
			indices.push_back(baseIndex + lod);
		}

		void* data;
		ib->Lock(0, 0, &data, 0);
		memcpy(data, &indices[0], indices.size()*sizeof(short));
		ib->Unlock();
}

class Terrain
{
public:
public:
	void create()
	{
		pEffectMaterial_ = new EffectMaterial;
		pEffectMaterial_->load(TEXT("e:/ZenBin/data/shader/Position.fx"));
		ID3DXEffect* dxe = pEffectMaterial_->getManagedEffect()->getDXEffect();

	}
	void render()
	{
	}
public:
	static int sMaxLod; 
private:
	EffectMaterial*	pEffectMaterial_;
};

int Terrain::sMaxLod(5);
