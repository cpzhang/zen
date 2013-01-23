#pragma once
#include "render/KeyFrames.h"
#include "ConfigModel.h"
class ApiModel_ Bone
{
public:
	Bone()
	{
		_clear();
	}
private:
	void _clear()
	{
		initialMatrix = Matrix::Identity;
		id = 0;
		parent = 0;
		name.clear();
	}
public:
	Matrix	initialMatrix;
	int		id;
	int     parent;
	std::string name;
	static const int tInvalidID;
};

class Skeleton;
class Skin;
class ApiModel_ BoneNode
{
public:
	BoneNode(Skeleton* ske, const std::string& name);
	~BoneNode();

public:
	void	attachObject(Bone* bone);
	void	detachObject();
	void	update(const AnimationTime& at, Skin* s);
	void    addChild(BoneNode* node);
	Bone*	getBone()
	{
		return _bone;
	}
	BoneNode* getParentNode();
	typedef std::map<std::string, BoneNode*> NameNodeMap;
private:
	void _clear();
private:
	Bone*		_bone;

	/// Pointer back to creator, for child creation (not smart ptr so child does not preserve parent)
	Skeleton*	_skeleton;
	Matrix		_mtxTransform;
	Matrix		_fullMatrix;
	std::string _name;
	BoneNode* _parent;
	NameNodeMap _children;
};