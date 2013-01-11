#include "FxManager.h"
#include "Fx.h"
FxManager::FxManager()
{
	clear_();
}

void FxManager::clear_()
{
	ShaderType_ = eFx_Shader0;
}

Fx* FxManager::find_( const tstring& resourceID )
{
	Fx* pEffect = NULL;
	Effects::iterator it = effects_.find( resourceID );
	if (it != effects_.end())
		pEffect = it->second;

	return pEffect;
}

void FxManager::add_( Fx* pEffect, const tstring& resourceID )
{
	effects_.insert( std::make_pair( resourceID, pEffect ) );
}

Fx* FxManager::getFx( const tstring& resourceID )
{
	Fx* pEffect = find_( resourceID );
	if (!pEffect)
	{
		pEffect = new Fx;
		if (pEffect->create( resourceID ))
		{
			add_( pEffect, resourceID );
		}
		else
		{
			delete pEffect;
			pEffect = Fx::getNullObject();
		}
	}
	return pEffect;
}

void FxManager::destroy()
{
	Effects::iterator it = effects_.begin();
	for(; it != effects_.end(); ++it)
	{
		Fx* f = it->second;
		if (f)
		{
			f->destroy();
			delete f;
		}
	}
	effects_.clear();
}

eFx FxManager::getShaderType()
{
	return ShaderType_;
}

void FxManager::setShaderType( eFx e )
{
	ShaderType_ = e;
}

ApiRender_ FxManager* createFxManager()
{
	new FxManager;
	return FxManager::getInstancePtr();
}

ApiRender_ FxManager* getFxManager()
{
	return FxManager::getInstancePtr();
}

ApiRender_ void destroyFxManager()
{
	if (getFxManager())
	{
		getFxManager()->destroy();
		delete FxManager::getInstancePtr();
	}
}
