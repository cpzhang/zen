#pragma once
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
#include "render/KeyFrames.h"
#include "Part.h"
#include "Mesh.h"
#include "Material.h"
#include "ParticleCluster.h"
class Entity;
class Skin;
class Material;
class Skeleton;
class ApiModel_ IMovable
{
public:
	virtual ~IMovable(){}
	virtual void setPosition(const Vector3& p) = 0;
	virtual void rotateY(float p) = 0;
	virtual void setScale(const Vector3& p) = 0;
};
class ApiModel_ EntityInstance : public IRender, public IMovable
{
public:
	~EntityInstance()
	{
		destroy();
	}
	EntityInstance()
	{
		SkinCurrent_ = NULL;
		Entity_ = NULL;
		Speed_ = 1.0f;
		Position_ = Vector3::Zero;
		AngleY_ = 0.0f;
		Scale_ = Vector3::One;
		//Scale_ *= 0.1f;
	}
public:
	virtual void render();
	void renderAABB(u32 color);
	virtual void update(float delta);
public:
	virtual void setPosition(const Vector3& p);
	Vector3 getPosition() const;
	virtual void rotateY(float p);
	virtual void setScale(const Vector3& p);
	Vector3 getScale() const;
public:
	bool create(const tstring& resourceId);
	void destroy();
	void release();
	Entity* getEntity();
	Skeleton* getSkeleton();
	void setAnimation(const tstring& resourceId, bool loop = true);
	void setSpeed(float s);
	EntityInstance* clone() const;
	std::string getResId() const;
	bool isPicking(const Ray& r) const;
public:
	void nmAddObj(std::vector<Vector3>& vertices, std::vector<Vector3Int>& indices);
private:
	void updateMaterial_( float delta );
	void renderImpT0();
	void renderImpT2();
	void updateBoundingBox_();
public:
	static EntityInstance* getNullObject()
	{
		static EntityInstance s;
		return &s;
	}
private:
	tstring FileName_;
	Entity* Entity_;
	AnimationTime AnimationTime_;
	Skin* SkinCurrent_;
	float Speed_;
	PartVec Parts_;
	MeshVec Meshes_;
	MaterialVec Materials_;
	Skeleton* Skeleton_;
	std::vector<Matrix> MatricesSkin_;
	typedef std::vector<KeyFrameController<float>> FloatKFCVec;
	typedef std::vector<float> FloatVec;
	FloatKFCVec Angles_;
	FloatVec AnglesCurrent_;
	FloatKFCVec Us_;
	FloatVec UsCurrent_;
	FloatKFCVec Vs_;
	FloatVec VsCurrent_;
	FloatKFCVec Alphas_;
	typedef std::vector<KeyFrameController<Vector3>> Vector3KFCVec;
	typedef std::vector<Vector4> Vector4Vec;
	Vector3KFCVec Colors_;
	Vector4Vec ColorsCurrent_;
	std::vector<Matrix> MatricesMaterial_;
	typedef std::vector<KeyFrameController<Quaternion>> QuaternionKFCVec;
	FloatVec UVSequenceTimer_;
	FloatVec Rows_;
	FloatVec Cols_;
	//
	ParticleClusterVec Particles_;
	//
	Vector3 Position_;
	float AngleY_;
	Vector3 Scale_;
	BoundingBox BBox_;
	BoundingBox BBoxOrigional_;
};
typedef std::vector<EntityInstance*> EntityInstanceVec;
Create_Singleton_Declaration(EntityInstanceManager, EntityInstance, ApiModel_)