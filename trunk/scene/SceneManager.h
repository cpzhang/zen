#pragma once
#include "misc/Singleton.h"
#include "LOD.h"
#include "render/Decal.h"
#include "ConfigScene.h"
#include "Chunk.h"
class Terrain;
class Hero;
class Decal;
class QuadNode;
enum eRunType
{
	eRunType_Editor,
	eRunType_Game,
	eRunType_Size,
};
class ApiScene_ SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();
public:
	void update();
	void render();
	void destroy();
	void createTerrain(int xChunks, int zChunks, int n, float unit);
	Terrain* getTerrain();
	LOD* getLOD();
	Hero* getHero();
	void setHero(Hero* h);
	void setRunType(eRunType rt);
	eRunType getRunType();
	bool IsAllChunksVisible();
	void setAllChunksVisible(bool b);
	Decal* createDecal(const tstring& name);
	QuadNode* getTerrainQuadTreeRoot();
	void getChunks(ChunkVec& cs, QuadNode* n, RectangleT& rc);
public:
	void save(const tstring& path);
	void open(const tstring& path);
private:
	void clear_();
	void destroyTerrain_();
	void destroyDecals();
	void constructTerrainQuadTree(int xChunks, int zChunks );
	void destroyTerrainQuadTree();
private:
	Terrain* terrainCurrent_;
	LOD		lod_;
	Hero*	hero_;
	eRunType runType_;
	bool allChunksVisible_;
	DecalMap decals_;
	QuadNode* terrainQuadTreeRoot_;
	tstring name_;
};
ApiScene_ SceneManager* createSceneManager();
ApiScene_ void destroySceneManager();
ApiScene_ SceneManager* getSceneManager();