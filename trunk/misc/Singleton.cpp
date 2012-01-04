#pragma once

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
		_instance = NULL;
	}

	static T& getInstance()		{ return (*_instance); }
	static T* getInstancePtr()	{ return _instance; }
	static bool isNull()
	{
		return NULL == _instance;
	}
};

template <typename T> T* Singleton<T>::_instance = NULL;