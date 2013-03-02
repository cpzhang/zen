#pragma once
#include "misc/stdHead.h"
#include "misc/helper.h"
#include "ConfigScene.h"
#include "Chunk.h"
#include "TerrainTextureSetter.h"
class Chunk;
class Fx;
class Decal;
class ApiScene_ Terrain
{
public:
	Terrain();
	~Terrain();
public:
	void render();
	void renderDecal(Decal* dc);
	void create(int xChunks, int zChunks);
	void destroy();
	void save(const tstring& path);
	void open(const tstring& path);
	void setFileName(const tstring& fn);
	void setFX(const tstring& fn);
	Chunk* getChunkFromWorldSpacePostion(float x, float z);
	float getHeightFromeWorldSpacePosition(float x, float z);
	Chunk* getChunkFromTopology(int x, int z);
	void updateVisibleChunks(float x, float z);
	void clearSelected();
	float getHeightFromImage(int x, int z);
	void setHeightFromImage(int x, int z, float h);
	//
//	Vector4 getBlendFromImage(int x, int z);
//	void setBlendFromImage(int x, int z, Vector4 b);
	int getXChunkNumber()
	{
		return xChunkNumber_;
	}
	int getZChunkNumber()
	{
		return zChunkNumber_;
	}
	tstring getFXFileName();
private:
	void clear_();
	void destroyChunks_();
	bool isXInside_(float f);
	bool isZInside_(float f);
	//
private:
	tstring fileName_;
	int xChunkNumber_;
	int zChunkNumber_;
	ChunkVec chunks_;
	ChunkVec chunksVisible_;
	Fx* fx_;
	std::vector<float>	heights_;
	//std::vector<Vector4> blends_;
	int totalNumberX_;
	int totalNumberZ_;
	TerrainTextureSetter textureSetter_;
	int mVersion;
};