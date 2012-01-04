#pragma once
#include "Singleton.h"
class EffectManager: public Singleton<EffectManager>
{
public:
	EffectManager()
	{
		clear_();
	}
private:
	void clear_()
	{
	}
	ManagedEffect* find_( const std::string& resourceID )
	{
		ManagedEffect* pEffect = NULL;
		Effects::iterator it = effects_.find( resourceID );
		if (it != effects_.end())
			pEffect = it->second;

		return pEffect;
	}
	void add_( ManagedEffect* pEffect, const std::string& resourceID )
	{
		effects_.insert( std::make_pair( resourceID, pEffect ) );
	}
public:
	ManagedEffect* get( const std::string& resourceID )
	{
		ManagedEffect* pEffect = find_( resourceID );
		if (!pEffect)
		{
			pEffect = new ManagedEffect;
			if (pEffect->load( resourceID ))
			{
				add_( pEffect, resourceID );
			}
			else
			{
				delete pEffect;
				pEffect = NULL;
			}
		}
		return pEffect;
	}
private:
	typedef std::map< std::string, ManagedEffect* > Effects;
	Effects effects_;
};