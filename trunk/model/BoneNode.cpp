#include "BoneNode.h"
#include "misc/stdHead.h"
#include "Skeleton.h"
#include "Skin.h"
BoneNode::BoneNode( Skeleton* ske, const std::string& name )
	: _bone(NULL), _skeleton(ske), _name(name), _parent(NULL)
{
}
BoneNode::~BoneNode()
{
	_clear();
}

void BoneNode::attachObject( Bone* bone )
{
	_bone = bone;
}

void BoneNode::detachObject()
{
	_bone = NULL;
}

Quaternion convertQ(const Quaternion& q)
{
	Quaternion qq;
	qq.x = -q.x;
	qq.y = -q.y;
	qq.z = -q.z;
	qq.w = q.w;

	return qq;
}

void BoneNode::update( const AnimationTime& at , Skin* sk)
{	
	if (_bone->id >= 0 && _bone->id < sk->boneKFs.size())
	{
		sBoneKFs& b = sk->boneKFs[_bone->id];
		//translation
		Vector3 t = b.translationKFs.getFrame(&at);

		//rotation
		Quaternion q = b.rotationKFs.getFrame(-1, &at);

		//scale
		Vector3 s = b.scaleKFs.getFrame(-1, &at);

		Matrix dynamicMtx = Matrix::Identity;
		dynamicMtx.make(t, s, q);
		//
		if(0)
		{
			Matrix tM;
			tM.setTranslate(t);
			Matrix tQ;
			tQ.setRotate(&q);
			dynamicMtx = tM * tQ;
		}

		//
		if (_parent)
		{
			_mtxTransform =  _parent->_fullMatrix * dynamicMtx;
		}
		else
		{
			_mtxTransform = dynamicMtx;
		}
		_fullMatrix = _mtxTransform;

		//
		_skeleton->_matricesFull[_bone->id] = _mtxTransform;
		Matrix mt = _bone->initialMatrix;
		mt.invert();
		_mtxTransform = _fullMatrix * mt;
		//
		_skeleton->_matrices[_bone->id] = _mtxTransform;
	}
	
	NameNodeMap::iterator it = _children.begin();
	for ( ; it != _children.end(); ++it)
	{
		BoneNode* n = it->second;
		n->update(at, sk);
	}
}

void BoneNode::_clear()
{
	_bone = NULL;
	_skeleton = NULL;
	_name.clear();
	_parent = NULL;
}

void BoneNode::addChild( BoneNode* node )
{
	_children[node->_name] = node;
	node->_parent = this;
}

BoneNode* BoneNode::getParentNode()
{
	return _parent;
}