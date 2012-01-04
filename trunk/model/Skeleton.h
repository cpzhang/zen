#pragma once
#include "ConfigModel.h"
#include "render/KeyFrames.h"
#include "misc/ConstBaseIterator.h"
#include "misc/singleton.h"
//#include "BoneNode.h"
class BoneNode;
class Skin;
typedef stdext::hash_map<std::string, BoneNode*> BoneNodeMap;
typedef	ConstBaseIterator<BoneNodeMap>	BoneNodeMapIterator;
class ApiModel_ Skeleton
{
public:
	virtual BoneNode* createBoneNode(const std::string& name);
	virtual u16 getNumBoneNodes(void) const;
	virtual BoneNode* getBoneNode(const std::string& name);
	virtual bool      hasBoneNode(const std::string& name);
public:
	bool create(const std::string& fileName);
	void destroy();
	void update(const AnimationTime& at, Skin* s);
	void reset();
	void render();
	BoneNodeMapIterator	getCommandMapIterator(void) const;
	
public:
	Skeleton();
	~Skeleton();
public:
	static Skeleton* getNullObject()
	{
		static Skeleton s;
		return &s;
	}
private:
	void _clear();
	void _relate();
	//void _addRootNode();
	BoneNode* _getBoneNode(int id);
protected:
	BoneNodeMap		_boneNodes;
	char			_version;
	std::vector<std::string> _roots;
public:
	std::vector<Matrix> _matrices;
	std::vector<Matrix> _matricesFull;
	friend class BoneNode;
};
// typedef stdext::hash_map<std::string, Skeleton*> NameSkeletonMap;
// class SkeletonManager : public Singleton<SkeletonManager>, public IManager
// {
// public:
// 	virtual bool create()
// 	{
// 		return true;
// 	}
// 	virtual void destroy();
// public:
// 	Skeleton* getSkeleton(const tstring& name);
// private:
// private:
// 	Skeleton* find_( const tstring & resourceID );
// 	Skeleton* create_( const tstring & resourceID );
// 	void add_( Skeleton* pMesh, tstring resourceID );
// private:
// 	NameSkeletonMap skeletons_;
// };
Create_Singleton_Declaration(SkeletonManager, Skeleton)

