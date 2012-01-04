#pragma once
#include "managedEffect.h"
#define SAFE_SET( pTheEffect, type, name, value ) \
{\
	D3DXHANDLE h = pTheEffect->GetParameterByName( NULL, name );\
	if (h)\
	{\
	pTheEffect->Set##type( h, value );\
	}\
}

class EffectMaterial
{
public:
public:
	bool load(const std::string& name)
	{
		pManagedEffect_ = FxManager::getInstance().get( name);
	}
	Fx* getManagedEffect() 
	{
		return pManagedEffect_; 
	}
	bool begin()
	{
		if (NULL == pManagedEffect_)
		{
			return false;
		}
	}
	bool end() const;

	bool beginPass( u32 pass ) const;
	bool endPass() const;

	bool commitChanges() const;
	D3DXHANDLE getCurrentTechnique() const 
	{ 
		D3DXHANDLE ret = 0;
		if ( pManagedEffect_ )
		{
			ret = pManagedEffect_->getCurrentTechnique();
		}
		return ret; 
	}
private:
	Fx*	pManagedEffect_;
};