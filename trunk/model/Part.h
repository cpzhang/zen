#pragma once
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
class Mesh;
class Material;
class Skeleton;
//×éºÏ£¬Mesh + Material
class ApiModel_ Part
{
public:
	Part();
public:
	bool create(const std::string& fileName);
	void render();
	void destroy();
	Mesh* getMesh();
	Material* getMaterial();
	Skeleton* getSkeleton();
	tstring getFilePath();
	void renderSkeleton();
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
	Skeleton* Skeleton_;
};

Create_Singleton_Declaration(PartManager, Part, ApiModel_)