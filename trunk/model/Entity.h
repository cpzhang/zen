#pragma once
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
class PartInstance;
class ApiModel_ Entity : public IRender
{
public:
	virtual void render();
public:
	bool create(const tstring& resourceId);
	void destroy();
	void addPartInstance(const tstring& resourceId);
	size_t getPartInstanceNumber();
	PartInstance* getPartInstance(size_t index);
public:
	static Entity* getNullObject()
	{
		static Entity s;
		return &s;
	}

private:
	typedef std::vector<PartInstance*> PartInstanceVec;
	PartInstanceVec PartInstances_;
	tstring Name_;
	tstring FileName_;
};

Create_Singleton_Declaration(EntityManager, Entity, ApiModel_)