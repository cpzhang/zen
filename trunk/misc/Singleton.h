#pragma once
#include "NamedObjectManager.h"
template <typename T>
class Singleton
{
protected:
	static T*	_instance;

public:
	Singleton()
	{
		_instance = static_cast<T*>(this);
	}

	~Singleton()
	{
		_instance = 0;
	}

	static T& getInstance()		{ return (*_instance); }
	static T* getInstancePtr()	{ return _instance; }
	static bool isNull()
	{
		return 0 == _instance;
	}
};

template <typename T> T* Singleton<T>::_instance = 0;

#define Create_Singleton_Imp(name, api)\
api void create##name()\
{\
	if (get##name() == 0)\
	{\
		new name;\
		if (!get##name()->create())\
		{\
			destroy##name();\
		}\
	}\
}\
api name* get##name()\
{\
	return name::getInstancePtr();\
}\
api void destroy##name()\
{\
	if (name::getInstancePtr())\
	{\
		name::getInstancePtr()->destroy();\
		delete name::getInstancePtr();\
	}\
}


#define Create_Singleton_Declaration(name, object, api)\
class api name : public NamedObjectManager<object>, public Singleton<name>\
{\
};\
api void create##name();\
api name* get##name();\
api void destroy##name();