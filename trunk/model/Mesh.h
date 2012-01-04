#pragma once
#include "ConfigModel.h"
#include "misc/helper.h"
#include "misc/stdHead.h"
#include "render/boundingbox.h"
#include "render/rendercontext.h"
#include "render/VDTManager.h"
#include "render/VertexBuffer.h"
#include "render/IndexBuffer.h"
class BoundingBox;
class VertexBuffer;
class IndexBuffer;
class Mz;
typedef std::map<u8, int> BoneIDReferenceMap;
//¶¥µã£¬Ãæ
class ApiModel_ Mesh
{
public:
	Mesh();
	~Mesh();
	bool create(const std::string& fileName);
	void createFromHeightMap(const std::string& fileName, int width, int height );
	bool createFromMZ(size_t sub, Mz* mz);
	void destroy();
	void clear();
	void render();
	int getFaceNumber();
	int getVertexNumber();
	int getVersion();
	tstring getFilePath();
	bool save(const std::string& path);
	void saveSkin( const std::string& fileName );
	void saveSkeleton(const std::string& fileName);
	void saveBoneMapping(const std::string& fileName, Mz* mz);
	void saveMaterial(const std::string& fileName);
	void loadBoneMapping(const std::string& fileName);
	BoundingBox* getAABB();
private:
	bool initBuffer_();
	void _initAABB();
	void onLoaded_();
public: 
	struct sFace
	{
		sFace()
		{
			memset(this, 0, sizeof(*this));
		}
		sFace(u16 v0, u16 v1, u16 v2)
		{
			index[0] = v0;
			index[1] = v1;
			index[2] = v2;
		}
		u16 index[3];
	};
public:
	static Mesh* getNullObject()
	{
		static Mesh s;
		return &s;
	}
public:
	typedef std::vector<sVDT_PositionTextureBoneWeightColorNormal> VertexPTBWCVec;
	VertexPTBWCVec	_vertices;
	typedef std::vector<sVDT_PositionTexture> VertexPTVec;
	VertexPTVec	_verticesPT;
	typedef std::vector<sFace> FaceVec;
	FaceVec	_faces;
	VertexBuffer vertexBuffer_;
	IndexBuffer indexBuffer_;
	std::string _name;
	tstring filePath_;
	u8 version_;
	BoneIDReferenceMap _bones;
	eVertexDeclarationType vdt_;
	BoundingBox mAABB;
private:
	//		friend class Terrain;
	friend class MZ;
};

Create_Singleton_Declaration(MeshManager, Mesh, ApiModel_)