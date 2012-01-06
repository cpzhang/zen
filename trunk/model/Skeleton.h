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
struct sSkinAnimation
{
	tstring name;
	float duration;
	tstring skinFilePath;
};
typedef std::vector<sSkinAnimation> SkinAnimationVec;
typedef std::map<tstring, Skin*> NameSkinMap;
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
	//
	bool decodeSkinAnimationXML(const std::string& fileName);
	int getSkinAnimationNumber();
	sSkinAnimation* getSkinAnimation(int index);
	//
	Skin* getSkin(const tstring& skinAnimationName);
	//
	void update(const AnimationTime& at, Skin* s);
	BoneNodeMapIterator	getCommandMapIterator(void) const;
	tstring getFilePath();
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
	tstring FilePath_;
	SkinAnimationVec SkinAnimations_;
	tstring SkinAnimationXMLFilePath_;
	NameSkinMap NameSkins_;
public:
	std::vector<Matrix> _matrices;
	std::vector<Matrix> _matricesFull;
	friend class BoneNode;
};
Create_Singleton_Declaration(SkeletonManager, Skeleton, ApiModel_)

