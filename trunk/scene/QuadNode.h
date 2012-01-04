#pragma once
#include "render/Rectangle.h"
#include "render/boundingbox.h"
#include "ConfigScene.h"
enum eQuadNode
{
	eQuadNode_LeftBottom,
	eQuadNode_RightBottom,
	eQuadNode_LeftTop,
	eQuadNode_RightTop,
	eQuadNode_Size,
};
class ApiScene_ QuadNode
{
public:
	QuadNode();
	~QuadNode();
public:
	static QuadNode* create(bool resetSN = false);
	//递归释放，先释放孩子，后释放自己
	void release();
	static RectangleT calculateRect(QuadNode* n);
	static RectangleT unionRect(QuadNode* n);
	//先计算Rect，后计算BoudingBox
	static void calculateBoundingBox(QuadNode* n);
private:
	void releaseImp_();
	void clear_();
public:
	QuadNode* children_[eQuadNode_Size];
	int xNumber_;//chunk
	int zNumber_;
	int x_;//grid inside chunk
	int z_;
	RectangleT rect_;
	BoundingBox boundingBox_;
	bool leaf_;
	int number_;
	bool insideChunk_;
};