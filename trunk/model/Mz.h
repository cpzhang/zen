#pragma once
#include "misc/helper.h"
#include "misc/stdHead.h"
#include "misc/dxHead.h"
#include "misc/DataChunk.h"
#include "render/KeyFrames.h"
#include "render/vector3.h"
#include "ConfigModel.h"
#include "Mesh.h"
#include "ParticleEmitter.h"
#include "BoneNode.h"
#include "Skin.h"
class Mesh;
struct sSubMesh
{
	sSubMesh()
	{
		memset(this, 0, sizeof(*this));
	}
	unsigned short matId;			// 材质id
	unsigned short vstart;			// first vertex
	unsigned short vcount;			// num vertices
	unsigned short istart;			// first index
	unsigned short icount;			// num indices
	bool		animated;
	u8		nameLen;
};

struct sSubMeshWithName: public sSubMesh
{
	sSubMeshWithName()
	{
		memset(this, 0, sizeof(*this));
	}
	char name[128];
};

struct sSubMeshWithName_V3
{
	sSubMeshWithName_V3()
	{
		memset(this, 0, sizeof(*this));
	}
	unsigned short matId;			//材质id
	unsigned short vstart;			// first vertex
	unsigned short vcount;			// num vertices
	unsigned short istart;			// first index
	unsigned short icount;			// num indices
	bool		animated;
	char name[256];
};

struct sVertex_V3 
{
	float pos[3];			/// 位置
	float normal[3];		/// 法线
	float color[4];			/// 颜色
	float weights[4];		/// 权重
	u8	bones[4];			/// 骨骼
	float texcoords[2];		/// 纹理坐标 
};
struct sVertex 
{
	sVertex()
	{
		memset(this, 0, sizeof(*this));	
	}
	sVertex(const sVertex_V3& v)
	{
		memcpy(pos, v.pos, sizeof(pos));
		memcpy(normal, v.normal, sizeof(normal));
		memcpy(weights, v.weights, sizeof(weights));
		memcpy(bones, v.bones, sizeof(bones));
		memcpy(texcoords, v.texcoords, sizeof(texcoords));
	}

	float pos[3];
	float normal[3];
	float weights[4];
	u8 bones[4];
	float texcoords[2];
};

struct sVertex_V3Color:public sVertex
{
	float color[4];
	sVertex_V3Color()
	{
		memset(this, 0, sizeof(*this));	
	}
	sVertex_V3Color(const sVertex_V3Color& v)
	{
		memcpy(this, &v, sizeof(*this));
	}
	sVertex_V3Color(const sVertex& v)
	{
		memcpy(pos, v.pos, sizeof(pos));
		memcpy(normal, v.normal, sizeof(normal));
		memcpy(weights, v.weights, sizeof(weights));
		memcpy(bones, v.bones, sizeof(bones));
		memcpy(texcoords, v.texcoords, sizeof(texcoords));
	}
	sVertex_V3Color(const sVertex_V3& v)
	{
		memcpy(pos, v.pos, sizeof(pos));
		memcpy(normal, v.normal, sizeof(normal));
		memcpy(weights, v.weights, sizeof(weights));
		memcpy(bones, v.bones, sizeof(bones));
		memcpy(texcoords, v.texcoords, sizeof(texcoords));
		memcpy(color, v.color, sizeof(color));
	}
};
struct sFace_V3
{
	int index[3];
};
struct sMat
{
	std::string mName;
	std::string mTextureName;
	D3DCULL mCullMode;
	sKeyFrameSet<float>		mRotationKFs;			/// 旋转关键帧
	//sKeyFrameSet<Vector3>		mScaleKFs;				/// 缩放关键帧
	sKeyFrameSet<float>		mFlowUKFs;				/// 平移关键帧
	sKeyFrameSet<float>		mFlowVKFs;				/// 平移关键帧
	sKeyFrameSet<Vector3>		mColorKFs;				/// 颜色关键帧
	sKeyFrameSet<float>		mAlphaKFs;				/// 透明度关键帧
	bool mRotate;
	bool mScale;
	bool mFlowU;
	bool mFlowV;
	D3DZBUFFERTYPE mZEnable;
	bool mZWriteEnable;
	bool mAlphaTestEnable;
	D3DBLEND mSrcBlend;
	D3DBLEND mDestBlend;
	D3DCMPFUNC mCmpFunc;
	u32 mAlphaRef;
	Vector4 mDiffuse;
	//序列图
	int mRows;
	int mCols;
	int mChangeInterval;
	int mChangeStyle;
};
struct tranMatrix
{
	float m_mat[4][3];
};
struct sAnimation
{
	std::string name;
	u32 begin;
	u32 end;
};

class ApiModel_ Mz
{
public:
	Mz()
	{
	}
	~Mz()
	{
	}
public:
	bool load( const std::string& fileName);
	
	Mesh* create( size_t sub, Mesh* m );

	void destroy();

	size_t getSubMeshNumber();
	size_t getParticleSystemNumber();
	std::string getSubMeshName( size_t sub );

	void decodeVersion( std::ifstream& f, int s );

	void decodeSubMesh( std::ifstream& f, int s );

	void decodeVertices( std::ifstream& f, int s );
	void decodeFaces( std::ifstream& f, int s );
	void decodeMaterial( std::ifstream& f, int s );
	void decodeAnimation( std::ifstream& f, int s );
	void decodeBone( std::ifstream& f, int s );
	void decodeRibbon( std::ifstream& f, int s );
	void decodeParticle( std::ifstream& f, int s, char* b);
	void saveAnimation( const std::string& fileName );
	void loadMzI( const std::string& fileName );
	void loadFb( const std::string& fileName );
	void saveSkeleton( const std::string& fileName );
	void saveSkin( const std::string& fileName );
	void saveEntity( const std::string& fileName );
	void saveMaterial( const std::string& fileName );
	void saveSubEntity( const std::string& fileName );
private:
	void clear();

private:
	std::string mzFileName_;
	char mVersion;
	typedef std::vector<sVertex_V3Color> VertexVec;
	VertexVec	mVertices;
	typedef std::vector<sSubMeshWithName> SubMeshVec;
	SubMeshVec	mSubmeshes;
	typedef std::vector<Mesh::sFace> FaceVec;
	FaceVec	mFaces;
	typedef std::vector<sMat> MatVec;
	MatVec	mMaterials;
	typedef std::vector<sAnimation> AniVec;
	AniVec	mAnimations;
	std::vector<std::string> fbFileNames_;
	std::vector<std::string> mAnimationNames;
	std::vector<Bone> mBones;
	std::vector<Skin> mSkins;
	bool mLoadFBs;
	bool mUseVertexColor;
	std::vector<ParticleEmitter> mParticleEmitter;
	friend class Mesh;
};
