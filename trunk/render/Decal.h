#pragma once
#include "misc/stdHead.h"
#include "vector4.h"
class ApiRender_ Decal
{
public:
	Decal()
	{

	}
	~Decal()
	{

	}
public:
	bool create()
	{
		return true;
	}
	void destroy()
	{

	}
	void setName(const tstring& name)
	{
		name_ = name;
	}
	void setTexture(const tstring& fn)
	{
		textureFileName_ = fn;
	}
	tstring& getTextureFile()
	{
		return textureFileName_;
	}
	void setFxFile(const tstring& fn)
	{
		FxFileName_ = fn;
	}
	const TCHAR* getFxFile()
	{
		return FxFileName_.c_str();
	}
	Vector4 getCenter()
	{
		return center_;
	}
	void setCenter(const Vector4& v)
	{
		center_ = v;
	}
	void setRadius(float f)
	{
		radius_ = f;
	}
	float getRadius()
	{
		return radius_;
	}
private:
	Vector4 center_;
	float radius_;
	tstring name_;
	tstring textureFileName_;	
	tstring FxFileName_;
};
typedef std::map<tstring, Decal*> DecalMap;