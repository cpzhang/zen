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
	//�ݹ��ͷţ����ͷź��ӣ����ͷ��Լ�
	void release();
	static RectangleT calculateRect(QuadNode* n);
	static RectangleT unionRect(QuadNode* n);
	//�ȼ���Rect�������BoudingBox
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