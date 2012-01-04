#include "QuadNode.h"
#include "SceneManager.h"
#include "Chunk.h"
#include "Terrain.h"
QuadNode::QuadNode()
{
	clear_();
}

QuadNode::~QuadNode()
{
	clear_();
}

QuadNode* QuadNode::create( bool resetSN /*= false*/ )
{
	static int sn = 0;
	QuadNode* n = new QuadNode;
	if (resetSN)
	{
		sn = 0;
	}
	n->number_ = sn++;
	return n;
}

void QuadNode::release()
{
	for (int i = eQuadNode_LeftBottom; i != eQuadNode_Size; ++i)
	{
		QuadNode* c = children_[i];
		if (c)
		{
			c->release();
		}
	}
	releaseImp_();
}

RectangleT QuadNode::calculateRect( QuadNode* n )
{
	if (n == NULL)
	{
		return RectangleT::Zero;
	}
	if (n->leaf_)
	{
		Chunk* ck = getSceneManager()->getTerrain()->getChunkFromTopology(n->xNumber_, n->zNumber_);
		n->rect_ = ck->getRect();
		return n->rect_;
	}
	return unionRect(n);
}

RectangleT QuadNode::unionRect( QuadNode* n )
{
	if (n == NULL)
	{
		return RectangleT::Zero;
	}
	RectangleT rc;
	QuadNode* m = NULL;
	m = n->children_[eQuadNode_LeftBottom];
	if (m)
	{
		rc = calculateRect(m);
	}
	m = n->children_[eQuadNode_RightBottom];
	if (m)
	{
		RectangleT t = calculateRect(m);
		rc.right_ += (t.right_ - t.left_);
	}
	m = n->children_[eQuadNode_LeftTop];
	if (m)
	{
		RectangleT t = calculateRect(m);
		rc.top_ += (t.top_ - t.bottom_);
	}
	m = n->children_[eQuadNode_RightTop];
	if (m)
	{
		RectangleT t = calculateRect(m);
	//	rc.top_ += (t.top_ - t.bottom_);
	}
	n->rect_ = rc;
	return rc;
}

void QuadNode::releaseImp_()
{
	delete this;
}

void QuadNode::clear_()
{
	for (int i = eQuadNode_LeftBottom; i != eQuadNode_Size; ++i)
	{
		children_[i] = 0;
	}
	xNumber_ = -1;
	zNumber_ = -1;
	x_ = -1;
	z_ = -1;
	leaf_ = true;
	rect_ = RectangleT::Zero;
	insideChunk_ = false;
}

void QuadNode::calculateBoundingBox( QuadNode* n )
{
	if (NULL == n)
	{
		return;
	}
	n->boundingBox_.max_ = Vector3(n->rect_.right_, 0.0f, n->rect_.top_);
	n->boundingBox_.min_ = Vector3(n->rect_.left_, 0.0f, n->rect_.bottom_);
// 	if (n->insideChunk_)
// 	{
// 		for (int x = n->xNumber_; x !)
// 		{
// 		}
// 	}
// 	else
	{
		if (n->leaf_)
		{
			Chunk* ck = getSceneManager()->getTerrain()->getChunkFromTopology(n->xNumber_, n->zNumber_);
			n->boundingBox_.max_.y = ck->getMaxHeight();
			n->boundingBox_.min_.y = ck->getMinHeight();
		}
	}
	
	QuadNode* m = NULL;
	m = n->children_[eQuadNode_LeftBottom];
	if (m)
	{
		calculateBoundingBox(m);
		n->boundingBox_.max_.y = max(n->boundingBox_.max_.y, m->boundingBox_.max_.y);
		n->boundingBox_.min_.y = min(n->boundingBox_.min_.y, m->boundingBox_.min_.y);
	}
	m = n->children_[eQuadNode_RightBottom];
	if (m)
	{
		calculateBoundingBox(m);
		n->boundingBox_.max_.y = max(n->boundingBox_.max_.y, m->boundingBox_.max_.y);
		n->boundingBox_.min_.y = min(n->boundingBox_.min_.y, m->boundingBox_.min_.y);
	}
	m = n->children_[eQuadNode_LeftTop];
	if (m)
	{
		calculateBoundingBox(m);
		n->boundingBox_.max_.y = max(n->boundingBox_.max_.y, m->boundingBox_.max_.y);
		n->boundingBox_.min_.y = min(n->boundingBox_.min_.y, m->boundingBox_.min_.y);
	}
	m = n->children_[eQuadNode_RightTop];
	if (m)
	{
		calculateBoundingBox(m);
		n->boundingBox_.max_.y = max(n->boundingBox_.max_.y, m->boundingBox_.max_.y);
		n->boundingBox_.min_.y = min(n->boundingBox_.min_.y, m->boundingBox_.min_.y);
	}
}
