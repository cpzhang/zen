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
	//Wander的相关参数
	Vector3		mWanderCatmullRom[4];	//拟合CatmullRom曲线的4个点
	float		mWanderS;				//[0.0f,1.0f]
	bool mWander;
	float mWanderSpeed;
	float mWanderRadius;
	Vector3 mColorStart;
	Vector3 mColorMiddle;
	Vector3 mColorEnd;
	Vector3 mAlpha;
	//
	short		mRow;			/// 行下标，用于rows*columns贴图
	short		mColumn;			/// 列下标，用于rows*columns贴图
	float		mDeltaTime;		/// 用以计算换贴图的时间差
	//
	ParticleEmitter* mEmitter;
};
typedef std::list<Particle> ParticleList;