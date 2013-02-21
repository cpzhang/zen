#pragma once
#include "misc/stdHead.h"
class IAttachable
{
public:
	virtual ~IAttachable(){};
};
enum eModelResType
{
	eModelResType_None,
	eModelResType_Entity,
	eModelResType_Skeleton,
	eModelResType_Skin,
	eModelResType_Part,
	eModelResType_Mesh,
	eModelResType_Material,
	eModelResType_ParticleEmitter,
	eModelResType_Size
};
typedef std::vector<tstring> ResIDVec;
class IModelRes
{
public:
	IModelRes()
	{
		Refer_ = 0;
	}
	virtual ~IModelRes(){};
public:
	virtual bool create(const tstring& resID) = 0;
	virtual void destroy() = 0;
public:
	void addReference()
	{
		++Refer_;
	}
	void removeReference()
	{
		--Refer_;
		if (Refer_ <= 0)
		{
			onNoReference_();
		}
	}
	void setResID(const tstring& resID)
	{
		ResID_ = resID;
	}
//public:
//	static eModelResType getType()
//	{
//		return eModelResType_None;
//	}
private:
	void onNoReference_();
protected:
	int Refer_;
	tstring ResID_;
};
struct sModelResNode
{
	eModelResType Type_;
	IModelRes* Ele_;
};
class ModelResManager
{
public:
	~ModelResManager()
	{
		
	}
public:
	static ModelResManager* getInstance()
	{
		static ModelResManager s;
		return &s;
	}
	void destroy()
	{
		ResHashMap::iterator it = Res_.begin();
		while(it != Res_.end())
		{
			sModelResNode& n = it->second;
			IModelRes* m = n.Ele_;
			if (m)
			{
				m->destroy();
				delete m;
			}
			++it;
		}
		Res_.clear();
	}
public:
	template<class T>
	T* get(const tstring& resID)
	{
		if (resID.empty())
		{
			return T::getNullObject();
		}
		sModelResNode* n = getNode(resID);
		if (n == NULL)
		{
			sModelResNode m;
			m.Type_ = T::getType();
			m.Ele_ = new T;
			if (m.Ele_->create(resID))
			{
				m.Ele_->setResID(resID);
				Res_[resID] = m;
				return (T*)(m.Ele_);	
			}
			else
			{
				delete m.Ele_;
				return T::getNullObject();
			}
		}
		/*else
		{
			if (n->Ele_->getType() != T::getType())
			{
				return NULL;
			}
		}*/
		return (T*)n->Ele_;
	}
	void release(const tstring& resID)
	{
		ResHashMap::iterator it = Res_.find(resID);
		if (it != Res_.end())
		{
			sModelResNode* n = &it->second;
			if (n)
			{
				n->Ele_->destroy();
				delete n->Ele_;
				//
				Res_.erase(it);
			}
		}
	}
private:
	sModelResNode* getNode(const tstring& resID)
	{
		ResHashMap::iterator it = Res_.find(resID);
		if (it == Res_.end())
		{
			return NULL;
		}
		return &it->second;
	}
private:
	typedef stdext::hash_map<tstring, sModelResNode> ResHashMap;
	ResHashMap Res_;
};