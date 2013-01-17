#pragma once
#include "render/rendercontext.h"
class ParticleEmitter;
class Particle
{
public:
	Particle();
	~Particle();
public:
	void render();
	void update(float delta, const Vector3& right, const Vector3& up, const Vector3& forword);
	bool isAlive();
private:
	void _clear();
public:
	sVDT_PositionColorTexture mVertices[4];
	//
	Vector4 mColor;
	//
	bool mStopMove;
	Vector3 mPosition;
	Vector3 mOriginalPosition;
	Vector3 mVelocity;
	float mLife;
	float mAge;
	float mGravity;
	float mExplosiveForce;
	Vector3 mScale;
	float mAngle;
	float mRotateSpeed;
	int mHeadFramesNum;
	int mHeadDecalFramesNum;
	int mTailFramesNum;
	int mTailDecalFramesNum;
	//Wander����ز���
	Vector3		mWanderCatmullRom[4];	//���CatmullRom���ߵ�4����
	float		mWanderS;				//[0.0f,1.0f]
	bool mWander;
	float mWanderSpeed;
	float mWanderRadius;
	Vector3 mColorStart;
	Vector3 mColorMiddle;
	Vector3 mColorEnd;
	Vector3 mAlpha;
	//
	short		mRow;			/// ���±꣬����rows*columns��ͼ
	short		mColumn;			/// ���±꣬����rows*columns��ͼ
	float		mDeltaTime;		/// ���Լ��㻻��ͼ��ʱ���
	//
	ParticleEmitter* mEmitter;
};
typedef std::list<Particle> ParticleList;