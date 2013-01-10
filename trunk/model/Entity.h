#pragma once
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
class Part;
class Skeleton;
class Part;
class ApiModel_ Entity : public IRender
{
public:
	virtual void render();
public:
	bool create(const tstring& resourceId);
	void destroy();
	void addPart(const tstring& resourceId);
	size_t getPartNumber();
	Part* getPart(size_t index);
	Skeleton* getSkeleton();
public:
	static Entity* getNullObject()
	{
		static Entity s;
		return &s;
	}

private:
	typedef std::vector<Part*> PartVec;
	PartVec Parts_;
	tstring Name_;
	tstring FileName_;
	Skeleton* Skeleton_;
};

Create_Singleton_Declaration(EntityManager, Entity, ApiModel_)