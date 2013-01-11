#include "ParticleEmitter.h"
ParticleEmitter::ParticleEmitter()
{
	_clear();		
}

ParticleEmitter::~ParticleEmitter()
{
	_clear();
}

void ParticleEmitter::spawn(float delta, const AnimationTime& at, ParticleList& ps)
{
	if (!mVisibility.getFrame(at.current))
	{
		return;
	}
	//换算成秒
	float timeFactor = delta / 1000.0f;
	mCurrentEmission += mEmitRate.getFrame(at) * timeFactor;
	for (; mCurrentEmission > 0.0f; mCurrentEmission -= 1.0f)
	{
		//调试一个粒子
		//mLimitNumber = 1;
		if (ps.size() >= mLimitNumber)
		{
			return;
		}
		Particle p;
		p.mEmitter = this;
		float		speed;
		float		width;
		float		length;
		float		height;
		float		latitude;
		float		variation;

		p.mVertices[0].texture_.x = 0;
		p.mVertices[0].texture_.y = 0;

		p.mVertices[1].texture_.x = 0;
		p.mVertices[1].texture_.y = 1;

		p.mVertices[2].texture_.x = 1;
		p.mVertices[2].texture_.y = 1;

		p.mVertices[3].texture_.x = 1;
		p.mVertices[3].texture_.y = 0;

		p.mStopMove = false;

		p.mColorStart = mColorStart;
		p.mColorMiddle = mColorMiddle;
		p.mColorEnd = mColorEnd;
		p.mAlpha = mAlpha;
		speed = mSpeedKFs.getFrame(at);
		width = mWidth.getFrame(at);
		length = mLength.getFrame(at);
		height = mHeigth.getFrame(at);
		latitude = mLatitude.getFrame(at);
		variation = mVariationKFs.getFrame(at);
		speed *= (1.0f + randomReal(0.0f, variation));

		p.mPosition = Vector3(randomReal(-width,width), randomReal(-height,height), randomReal(-length,length));
		p.mOriginalPosition = p.mPosition;

		Matrix mtxX;
		{
			Quaternion q;
			q.fromAngleAxis(randomReal(0,latitude * TwoPI / 360.f), Vector3::AxisX);
			mtxX.make(Vector3::Zero, Vector3::One, q);
		}
		
		Matrix mtxY;
		{
			Quaternion q;
			q.fromAngleAxis(randomReal(0,TwoPI), Vector3::AxisY);
			mtxY.make(Vector3::Zero, Vector3::One, q);
		}
		Matrix rot;
		rot.multiply(mtxY, mtxX);
		p.mVelocity = rot.applyVector(Vector3(0,1,0));
		p.mVelocity.normalise();

		p.mVelocity *= speed;

		if(mLifeSpan < mLifeVar)
			p.mAge = p.mLife = mLifeSpan;
		else
			p.mAge = p.mLife= randomReal(mLifeSpan - mLifeVar, mLifeSpan + mLifeVar);

		p.mGravity = mGravity.getFrame(at);
		p.mExplosiveForce = mExplosiveForce.getFrame(at);

		//每个粒子的缩放比例都可能不同
		p.mScale.x = randomReal(mScale.x - mScaleVar.x, mScale.x + mScaleVar.x);
		if(mFixedSize)
		{
			p.mScale.y = p.mScale.z = p.mScale.x;
		}
		else
		{
			p.mScale.y = randomReal(mScale.y - mScaleVar.y, mScale.y + mScaleVar.y);
			p.mScale.z = randomReal(mScale.z - mScaleVar.z, mScale.z + mScaleVar.z);
		}

		//粒子旋转的角度
		p.mAngle = randomReal(mInitAngleBegin, mInitAngleEnd);
		p.mRotateSpeed = randomReal(mRotateSpeed - mRotateSpeedVar, mRotateSpeed + mRotateSpeedVar) * timeFactor * 15.0f;

		if(mHead)
		{
			p.mHeadFramesNum = ((mHeadLifeSpan.y - mHeadLifeSpan.x + 1.0f) * mHeadLifeSpan.z);
			p.mHeadDecalFramesNum = ((mHeadDecay.y - mHeadDecay.x + 1.0f) * mHeadDecay.z);
		}
		else
		{
			p.mHeadFramesNum = 0;
			p.mHeadDecalFramesNum = 0;
		}

		if(mTail)
		{
			p.mTailFramesNum = ((mTailLifeSpan.y - mTailLifeSpan.x + 1.0f) * mTailLifeSpan.z);
			p.mTailDecalFramesNum = ((mTailDecay.y - mTailDecay.x + 1.0f) * mTailDecay.z);
		}
		else
		{
			p.mTailFramesNum = 0;
			p.mTailDecalFramesNum = 0;
		}

		if(mWander)
		{
			p.mWanderRadius = mWanderRadius;
			p.mWanderSpeed = mWanderSpeed;
			p.mWander = true;
			p.mWanderS = 0.0f;
			Matrix m1,m2;
			for(int i = 0;i < 4;i++)
			{
				{
					Quaternion q;
					q.fromAngleAxis(randomReal(0,TwoPI), Vector3::AxisX);
					m1.make(Vector3::Zero, Vector3::One, q);
				}
				{
					Quaternion q;
					q.fromAngleAxis(randomReal(0,TwoPI), Vector3::AxisY);
					m2.make(Vector3::Zero, Vector3::One, q);
				}
				Matrix mt;
				mt.multiply(m1, m2);
				p.mWanderCatmullRom[i] = mWanderRadius * (mt.applyVector(Vector3::AxisZ));
			}
		}
		//
		ps.push_back(p);
	}
}

void ParticleEmitter::_clear()
{
	mCurrentEmission = 0.0f;
}