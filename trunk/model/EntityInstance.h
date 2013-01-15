#pragma once
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
#include "render/KeyFrames.h"
#include "Part.h"
#include "Mesh.h"
#include "Material.h"
class Entity;
class Skin;
class Material;
class Skeleton;
class ApiModel_ EntityInstance : public IRender
{
public:
	EntityInstance()
	{
		SkinCurrent_ = NULL;
		Entity_ = NULL;
		Speed_ = 1.0f;
	}
public:
	virtual void render();
	virtual void update(float delta);
public:
	bool create(const tstring& resourceId);
	void destroy();
	Entity* getEntity();
	void setAnimation(const tstring& resourceId);
	void setSpeed(float s);
private:
	void updateMaterial_( float delta );
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
};

Create_Singleton_Declaration(EntityInstanceManager, EntityInstance, ApiModel_)