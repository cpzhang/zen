#pragma once
#include "misc/Singleton.h"
#include "misc/stdHead.h"
#include "misc/helper.h"
#include "ConfigRender.h"
#include "Fx.h"
class ApiRender_ FxManager: public Singleton<FxManager>
{
public:
	FxManager();
private:
	void clear_();
	Fx* find_( const tstring& resourceID );
	void add_( Fx* pEffect, const tstring& resourceID );
public:
	Fx* getFx( const tstring& resourceID );
	void destroy();
	eFx getShaderType();
	void setShaderType(eFx e);
private:
	typedef std::map< tstring, Fx* > Effects;
	Effects effects_;
	eFx ShaderType_;
};
ApiRender_ FxManager* createFxManager();
ApiRender_ FxManager* getFxManager();
ApiRender_ void destroyFxManager();