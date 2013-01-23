#pragma once
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
#include "ModelResManager.h"
class Part;
class Skeleton;
class Part;
class ApiModel_ Entity : public IModelRes
{
public:
	virtual bool create(const tstring& resID);
	virtual void destroy();
public:
	static eModelResType getType()
	{
		return eModelResType_Entity;
	}
public:
	void addPart(const tstring& resourceId);
	size_t getPartNumber();
	const tstring& getPart(size_t index);
	const tstring& getSkeleton();
	//
	size_t getParticleNumber();
	const tstring& getParticleName(const size_t index);
	const tstring& getParticleBoneName(const size_t index);
public:
	static Entity* getNullObject()
	{
		static Entity s;
		return &s;
	}

private:
	ResIDVec Parts_;
	tstring Name_;
	tstring FileName_;
	tstring NameSkeleton_;
	tstringVec NameParticles_;
	tstringVec NameParticleBones_;
};

//Create_Singleton_Declaration(EntityManager, Entity, ApiModel_)