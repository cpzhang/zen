#pragma once
#include "stdHead.h"
#include "vector2.h"
#include "rendercontext.h"
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
void generateLOD(int maxLod, int index, IDirect3DIndexBuffer9* ib)
{
	std::vector<short> indices;
	int lod = pow((double)2, (long)index);
	int verticesNum = getVerticesNumberOneSide(maxLod);
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
struct sVertex
{
	FLOAT    x, y, z;
	//D3DCOLOR diffuse;
	//FLOAT	u,v;
};
const DWORD gVertexFVF = D3DFVF_XYZ;// | D3DFVF_DIFFUSE | D3DFVF_TEX1;

const size_t gMaxLOD = 5;
class QuadTree
{
public:
	QuadTree()
	{
		clear_();
	}
	~QuadTree()
	{
		
	}
private:
	void clear_()
	{
		for (int i = 0; i != eChildrenNumber; ++i)
		{
			children_[i] = NULL;
		}
		lod_ = 0;
		ib_ = NULL;
		vb_ = NULL;
	}
	void releaseRes_()
	{
		if (ib_)
		{
			ib_->Release();
			vb_->Release();
		}
	}
public:
	void destroy()
	{
		bool isLeaf = true;
		for (int i = 0; i != eChildrenNumber; ++i)
		{
			if (children_[i])
			{
				children_[i]->destroy();
				delete children_[i];
				children_[i] = NULL;
				isLeaf = false;
			}
		}
		if (isLeaf)
		{
			releaseRes_();
			clear_();
		}
	}
	
	void render()
	{
		bool isLeaf = true;
		for (int i = 0; i != eChildrenNumber; ++i)
		{
			if (children_[i])
			{
				children_[i]->render();
				isLeaf = false;
			}
		}
		if (isLeaf)
		{
			getRenderContex()->getDxDevice()->SetFVF(gVertexFVF);
			getRenderContex()->getDxDevice()->SetStreamSource(0, vb_, 0, sizeof(sVertex));
			getRenderContex()->getDxDevice()->SetIndices(ib_);
			//gEffect->SetTexture("lay0", gTexture);
			UINT pPasses = 0;
//			gEffect->SetTechnique("PP2");
			//
//			gViewMatrix = gCamera.view_;
//			gEffect->SetMatrix("gView", &gViewMatrix);
//			D3DXMatrixPerspectiveFovLH(&gProjectionMatrix, 1.5, 1, 0.01, 1000);
//			gEffect->SetMatrix("gProjection", &gProjectionMatrix);
//			gEffect->Begin(&pPasses, D3DXFX_DONOTSAVESTATE);
// 			for (int i = 0; i != pPasses; ++i)
// 			{
// 				gEffect->BeginPass(i);
// 				getRenderContex()->device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, getVerticesNumberAll(gMaxLOD), 0, getTriangleNumberAll(gMaxLOD - lod_));
// 				gEffect->EndPass();
// 			}
// 			gEffect->End();			
		}
	}
public:
	enum{eChildrenNumber = 4};
	QuadTree* children_[eChildrenNumber];
	int lod_;
	Vector2 position_;
	IDirect3DVertexBuffer9* vb_;
	IDirect3DIndexBuffer9* ib_;
};
void constructQuadTree()
{
	QuadTree* gRoot = new QuadTree;
	for (int i = 0; i != QuadTree::eChildrenNumber; ++i)
	{
		QuadTree* n = new QuadTree;
		//
		gRoot->children_[i] = n;
		//
		getRenderContex()->getDxDevice()->CreateVertexBuffer(getVerticesNumberAll(gMaxLOD)*sizeof(sVertex), 0, gVertexFVF, D3DPOOL_MANAGED, &n->vb_, 0);
		//
		getRenderContex()->getDxDevice()->CreateIndexBuffer(getTriangleNumberAll(gMaxLOD)*3*2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &n->ib_, 0);
		//
		std::vector<sVertex> vertices;
		for (size_t z = 0; z != getVerticesNumberOneSide(gMaxLOD); z++)
		for (size_t x = 0; x != getVerticesNumberOneSide(gMaxLOD); x++)
		{
			sVertex v;
			v.x = x;
			v.z = z;
			v.y = 0;
			vertices.push_back(v);
		}
		{
			void* data;
			n->vb_->Lock(0, 0, &data, 0);
			memcpy(data, &vertices[0], vertices.size()*sizeof(sVertex));
			n->vb_->Unlock();
		}
		generateLOD(gMaxLOD, 0, n->ib_);
	}
}