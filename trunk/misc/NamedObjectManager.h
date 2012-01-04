#pragma once
#include "stdHead.h"
//#include "Singleton.h"
class IManager
{
public:
	virtual bool create() = 0;
	virtual void destroy() = 0;
};

template<class T>
class NamedObjectManager: public IManager
{
public:
	typedef stdext::hash_map<std::string, T*> NamePointerMap;
public:
	T* get(const tstring& name)
	{
		T* m = find_(name);
		if (m)
		{
			return m;
		}
		return create_(name);
	}
	virtual bool create()
	{
		return true;
	}
	virtual void destroy()
	{
		NamePointerMap::iterator it = meshes_.begin();
		for (; it != meshes_.end(); ++it)
		{
			T* t = it->second;
			if (t)
			{
				t->destroy();
				delete t;
			}
		}
		meshes_.clear();
	}
private:
	T* find_( const tstring & resourceID )
	{
		NamePointerMap::iterator it = meshes_.find(resourceID);
		if (it != meshes_.end())
		{
			return it->second;
		}
		return NULL;
	}
	T* create_( const tstring & resourceID )
	{
		T* mt = new T;
		if (mt->create(resourceID))
		{
			add_(mt, resourceID);
		}
		else
		{
			mt->destroy();
			delete mt;
			mt = T::getNullObject();
		}
		return mt;
	}
	void add_( T* pMesh, tstring resourceID )
	{
		meshes_.insert( std::make_pair(resourceID, pMesh) );
	}
private:
	NamePointerMap meshes_;
};
