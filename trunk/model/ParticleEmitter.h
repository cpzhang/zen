#pragma once
#include "render/KeyFrames.h"
#include "Particle.h"
class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();
public:
	void spawn(float delta, const AnimationTime& at, ParticleList& ps);
private:
	void _clear();
public:
	//
	float mCurrentEmission;
	//
	std::string mTextureFile;
	int mBlendMode;
	Vector3 mColorStart;
	Vector3 mColorMiddle;
	Vector3 mColorEnd;
	Vector3 mAlpha;

	//
	short mInitNumber;
	short mLimitNumber;
	bool mAttatchEmitter;
	bool mMoveWithEmitter;
	bool mForSword;
	float mSwordInitAngle;
	bool mWander;
	float mWanderRadius;
	float mWanderSpeed;
	Vector3 mScale;
	Vector3 mScaleVar;
	bool mFixedSize;
	Vector3 mHeadLifeSpan;
	Vector3 mHeadDecay;
	Vector3 mTailLifeSpan;
	Vector3 mTailDecay;
	int mRows;
	int mCols;
	short mChangeStyle;
	short mChangeInterval;
	float mTime;
	float mLifeSpan;
	float mLifeVar;
	float mTailLength;
	float mAspectRadio;
	float mInitAngleBegin;
	float mInitAngleEnd;
	float mRotateSpeed;
	float mRotateSpeedVar;
	bool mHead;
	bool mTail;
	bool mUnShaded;
	bool mUnFogged;
	bool mBlockedByY0;
	//
	sKeyFrames<float> mSpeedKFs;
	sKeyFrames<float> mVariationKFs;
	sKeyFrames<float> mLatitude;
	sKeyFrames<float> mGravity;
	sKeyFrames<float> mExplosiveForce;
	sKeyFrames<bool> mVisibility;
	sKeyFrames<float> mEmitRate;
	sKeyFrames<float> mLength;
	sKeyFrames<float> mWidth;
	sKeyFrames<float> mHeigth;
};
