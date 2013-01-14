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
};

Create_Singleton_Declaration(EntityInstanceManager, EntityInstance, ApiModel_)