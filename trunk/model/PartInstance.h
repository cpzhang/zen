#pragma once
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/Singleton.h"
#include "render/Quaternion.h"
class Part;
class ApiModel_ PartInstance : public IRender
{
public:
	PartInstance();
public:
	virtual void render();
public:
	bool create(const tstring& resourceId);
	void destroy();
	Part* getPart();
public:
	static PartInstance* getNullObject()
	{
		static PartInstance s;
		return &s;
	}
private:
	void clear_();
private:
	Part* part_;
	Vector3 Position_;
	Quaternion Angle_;
	Vector3 Scale_;
};

Create_Singleton_Declaration(PartInstanceManager, PartInstance, ApiModel_)