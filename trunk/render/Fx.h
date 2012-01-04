#pragma once
#include "misc/dxHead.h"
#include "misc/stdHead.h"
#include "misc/helper.h"
#include "ConfigRender.h"
enum eFx
{
	eFx_Shader0,//fixed function program
	eFx_Shader1,
	eFx_Shader2,// shader 2.0
};
class ApiRender_ Fx
{
public:
	Fx();
private:
	void clear_();
public:
	ID3DXEffect* getDxEffect();
	bool create( const tstring& effectResource );
	void destroy();
	D3DXHANDLE getCurrentTechnique() const;
	static Fx* getNullObject()
	{
		Fx s;
		return &s;
	}
	tstring getFilePath();
public:
	static const char* getWorldMatrixName();
	static const char* getViewMatrixName();
	static const char* getProjectionMatrixName();
	static const char* getViewProjectionMatrixName();
	static const char* getWorldViewProjectionMatrixName();
private:
	ID3DXEffect* pEffect_;
	D3DXHANDLE	hCurrentTechnique_;
	tstring resourceID_;
public:
	static const std::string WorldMatrixName;
	static const std::string ViewMatrixName;
	static const std::string ProjectionMatrixName;
	static const std::string ViewProjectionMatrixName;
	static const std::string WorldViewProjectionMatrixName;
};