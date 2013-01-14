#pragma once
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
#include "ModelResManager.h"
class Mesh;
class Material;
class Entity;
//×éºÏ£¬Mesh + Material
class ApiModel_ Part : public IModelRes
{
public:
	Part();
public:
	virtual bool create(const tstring& resID);
	virtual void destroy();
public:
	static eModelResType getType()
	{
		return eModelResType_Part;
	}
public:
	const tstring& getMesh();
	const tstring& getMaterial();
	const tstring& getFilePath();
	void renderSkeleton();
	//void setEntity(Entity* e);
public:
	static Part* getNullObject()
	{
		static Part s;
		return &s;
	}
private:
	void clear_();
private:
	tstring FilePath_;
	std::string Name_;
	tstring NameMesh_;
	tstring NameMaterial_;
	//Entity* Entity_;
};
typedef std::vector<Part*> PartVec;
//Create_Singleton_Declaration(PartManager, Part, ApiModel_)