#pragma once
#include "render/matrix.h"
#include "render/VertexBuffer.h"
#include "misc/stdHead.h"
#include "misc/helper.h"
#include "render/vector2.h"
#include "render/Rectangle.h"
#include "ConfigScene.h"
class Texture;
enum eTerrainLayer
{
	eTerrainLayer_0,
	eTerrainLayer_1,
	eTerrainLayer_2,
	eTerrainLayer_Size,
};
class ApiScene_ Chunk
{
public:
	Chunk(int x, int z);
	~Chunk();
public:
	void render();
	void destroy();
	Matrix& getWorldMatrix();
	float getHeightFromTopology(int x, int z);
	Vector2 getWorldCoordination(int x, int z);
	void setHeightFromTopology(int x, int z, float h);
	void refreshHeight();
	RectangleT getRect();
	bool isSelected();
	void setSelected(bool b);
	int getNumberX();
	int getNumberZ();
	float getMaxHeight();
	float getMinHeight();
	//±ÕÇø¼ä
	Vector2 getMaxMinHeightFromTopology(int x0, int x1, int z0, int z1);
	Texture* getLayer(eTerrainLayer e);
	eTerrainLayer getLayer(const tstring & resourceID);
	void setLayer(eTerrainLayer e, const tstring & resourceID);
	eTerrainLayer getNextLayer();
	//
	Vector4 getBlendFromTopology(int x, int z);
	void setBlendFromTopology(int x, int z, Vector4 h);
	void refreshBlend();
private:
	void clear_();
	void calcMaxMinHeight_();
	void refreshVB_();
private:
	RectangleT rect_;
	int x_;
	int z_;
	Vector3 center_;
	Matrix worldMatrix_;
	u32						nIndices_;
	VertexBuffer				vertexBuffer_;
	//std::vector<float>	heights_;
	bool isSelected_;
	float maxHeight_;
	float minHeight_;
	std::vector<Texture*> layers_;
};
typedef std::vector<Chunk*> ChunkVec;