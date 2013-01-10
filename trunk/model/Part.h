#pragma once
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
class Mesh;
class Material;
class Entity;
//×éºÏ£¬Mesh + Material
class ApiModel_ Part : public IRender
{
public:
	virtual void render();
public:
	Part();
public:
	bool create(const std::string& fileName);
	void destroy();
	Mesh* getMesh();
	Material* getMaterial();
	tstring getFilePath();
	void renderSkeleton();
	void setEntity(Entity* e);
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
	Mesh* Mesh_;
	Material* Material_;
	Entity* Entity_;
};

Create_Singleton_Declaration(PartManager, Part, ApiModel_)