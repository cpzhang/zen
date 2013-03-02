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
class EntityInstance;
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
	void update(const float delta);
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
	Vector2 getPickingPoint();
public:
	EntityInstance* createEntityInstance(const std::string& resID);
	EntityInstance* addEntityInstance(const std::string& resID);
	void removeEntityInstance(EntityInstance* ei);
public:
	void save(const tstring& path);
	void open(const tstring& path);
	void constructTerrainQuadTree(int xChunks, int zChunks );
private:
	void clear_();
	void destroyTerrain_();
	void destroyDecals();
	void destroyTerrainQuadTree();
	void destroyEntityInstances_();
	void updatePickingPoint_();
private:
	Terrain* terrainCurrent_;
	LOD		lod_;
	Hero*	hero_;
	eRunType runType_;
	bool allChunksVisible_;
	DecalMap decals_;
	QuadNode* terrainQuadTreeRoot_;
	tstring name_;
	typedef std::vector<EntityInstance*> EntityInstanceVec;
	EntityInstanceVec entityInstances_;
	Vector2 PickingPoint_;
};
ApiScene_ SceneManager* createSceneManager();
ApiScene_ void destroySceneManager();
ApiScene_ SceneManager* getSceneManager();