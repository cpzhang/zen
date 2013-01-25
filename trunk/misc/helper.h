#pragma once
#include "ConfigMisc.h"
typedef __int8				s8;
typedef unsigned __int8		u8;

typedef __int16				s16;
typedef unsigned __int16	u16;

typedef __int32				s32;
typedef unsigned __int32	u32;

typedef __int64				s64;
typedef unsigned __int64	u64;

// ÉèÖÃ¶Ïµã
#define	breakable		switch(1) case 1: default:

#define safeDelete(x) if((x)!=NULL){delete (x); (x) = NULL;}

ApiMisc_ bool isKeyDown(int vk);

#define STL_SAFE_DELETE(x)	{if(x){delete (x);(x)=0;}}
#define STL_SAFE_RELEASE(x)	{if(x){(x)->release();(x)=0;}}
#define STL_SAFE_Destroy(x)	{if(x){(x)->destroy();(x)=0;}}

template<class T>void STLDeleteAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_DELETE((*i).second);
	}
	t.clear();
}

template<class T>void STLDeleteSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_DELETE(*i);
	}
	t.clear();
}

template<class T>void STLReleaseAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_RELEASE((*i).second);
	}
	t.clear();
}
template<class T>void STLDestroySequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		safeDestroy(*i);
	}
	t.clear();
}

template<class T>void STLDestroyAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		safeDestroy((*i).second);
	}
	t.clear();
}

template<class T>void STLReleaseSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		(*i)->release();
	}
	t.clear();
}

template<class T,class TS>void STLDeleteAssociateSequence(T& t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		TS *pTS = (*i).second;
		if(pTS)
		{
			STLDeleteSequence(*pTS);
		}
		STL_SAFE_DELETE((*i).second);
	}
	t.clear();
}

ApiMisc_ float  randomReal(const float left, const float right, const unsigned int random_max = 0x7fff);

const float PI = 3.141592654f;

const float TwoPI = PI * 2.0f;

const float HalfPI = PI * 0.5f;
