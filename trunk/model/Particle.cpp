#include "Particle.h"
#include "ParticleEmitter.h"
#include "render/math.h"
#include "render/rendercontext.h"
#include "render/matrix.h"
#include "render/Colour.h"
Particle::Particle()
{
	memset(this, 0, sizeof(*this));
	_clear();
}

Particle::~Particle()
{

}

void Particle::render()
{
	static u16 s_indices[] = {0, 3, 1, 1, 3, 2};
	getRenderContex()->drawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, s_indices, D3DFMT_INDEX16, mVertices, sVDT_PositionColorTexture::getSize());
}

void Particle::update(float delta, const Vector3& right, const Vector3& up, const Vector3& forword, const Matrix& m)
{
	mDelta = delta  * 0.001f;
	mAge += mDelta;
	if (!isAlive())
	{
		return;
	}
	mRate = mAge / mLife;
	//��
	mDeltaTime += mDelta;
	updateUV_();
	updateColor_();
	updatePostion_(right, up, forword, m);
}

bool Particle::isAlive()
{
	return (mAge <= mLife) && (mLife > 0.00001f);
}

void Particle::_clear()
{
	mEmitter = NULL;
}

void Particle::updateUV_()
{
	//����ͼ
	if(mDeltaTime >= mEmitter->mChangeInterval)
	{
		switch(mEmitter->mChangeStyle)
		{
		case 0:
			{
				mRow++;
				if(mRow >= mEmitter->mRows)
				{
					mRow = 0;
					mColumn++;
				}
				if(mColumn >= mEmitter->mCols)
				{
					mColumn = 0;
				}
			}
			break;
		case 1:
			{
				mRow = randomReal(0,mEmitter->mRows);
				mColumn = randomReal(0,mEmitter->mCols);
			}
			break;
		}

		mDeltaTime = 0;
	}

	float cellWidth = (mEmitter->mCols == 0) ? 1.0f : (1.0f / mEmitter->mCols);
	float cellHeight = (mEmitter->mRows == 0) ? 1.0f : (1.0f / mEmitter->mRows);
	float top1 = cellWidth * mColumn;
	float left1 = cellHeight * mRow;
	float bottom1 = cellWidth * (mColumn + 1);
	float right1 = cellHeight * (mRow + 1);
	mVertices[0].texture_ = Vector2(left1, top1);
	mVertices[1].texture_ = Vector2(left1, bottom1);
	mVertices[2].texture_ = Vector2(right1, bottom1);
	mVertices[3].texture_ = Vector2(right1, top1);
}

void Particle::updateColor_()
{
	Vector4 color1,color2,color3;
	color1.x = mColorStart.x;
	color1.y = mColorStart.y;
	color1.z = mColorStart.z;
	color1.w = mAlpha.x;

	color2.x = mColorMiddle.x;
	color2.y = mColorMiddle.y;
	color2.z = mColorMiddle.z;
	color2.w = mAlpha.y;

	color3.x = mColorEnd.x;
	color3.y = mColorEnd.y;
	color3.z = mColorEnd.z;
	color3.w = mAlpha.z;

	if (mRate < mEmitter->mTime)
	{
		mColor = InterpolateBezier(mRate/mEmitter->mTime, color1, color2);
	}
	else
	{
		mColor = InterpolateBezier((mRate - mEmitter->mTime) / (1.0f - mEmitter->mTime), color2, color3);
	}
	if(0){
		std::ostringstream ss;
		ss<<mColor.w<<std::endl;
		::OutputDebugString(ss.str().c_str());
	}
	u32 c = Colour::getUint32FromNormalised(mColor);
	mVertices[0].color_ = c;
	mVertices[1].color_ = c;
	mVertices[2].color_ = c;
	mVertices[3].color_ = c;
}

void Particle::updatePostion_(const Vector3& right, const Vector3& up, const Vector3& forword, const Matrix& m)
{
	//���ȸ�������λ��
	//�����Wander����
	if(mWander)
	{
		mWanderS += mDelta * mWanderSpeed;
		if(mWanderS >= 1.0f)
		{
			mWanderS = mWanderS - 1.0f;
			mWanderCatmullRom[0] = mWanderCatmullRom[1];
			mWanderCatmullRom[1] = mWanderCatmullRom[2];
			mWanderCatmullRom[2] = mWanderCatmullRom[3];
			Matrix m1,m2;
			{
				Quaternion q;
				q.fromAngleAxis(randomReal(0,360), Vector3::AxisX);
				m1.make(Vector3::Zero, Vector3::One, q);
			}
			{
				Quaternion q;
				q.fromAngleAxis(randomReal(0,360), Vector3::AxisY);
				m2.make(Vector3::Zero, Vector3::One, q);
			}
			Matrix mt;
			mt.multiply(m1, m2);
			mWanderCatmullRom[3] = mWanderRadius * (mt.applyVector(Vector3::AxisZ));
		}
		D3DXVec3CatmullRom((D3DXVECTOR3*)&mPosition, 
			(D3DXVECTOR3*)&mWanderCatmullRom[0],
			(D3DXVECTOR3*)&(mWanderCatmullRom[1]),
			(D3DXVECTOR3*)&(mWanderCatmullRom[2]),
			(D3DXVECTOR3*)&(mWanderCatmullRom[3]),
			mWanderS);
	}
	else
	{
		//������ͳ������
		if(!mStopMove)
		{
			Vector3 vOrigin = m.applyToOrigin();
			if(mExplosiveForce < 0)
			{
				Vector3 posOrigin = mPosition;
				Vector3 posNew = mPosition;
				mVelocity += (mPosition - vOrigin) * mDelta * mExplosiveForce;
				mVelocity.y += (mGravity * mDelta);
				posNew += (mVelocity * mDelta);
				Vector3 posSave = posNew;
				posOrigin.normalise();
				posNew.normalise();
				if(posOrigin.dotProduct(posNew) >= 0)
				{
					mPosition = posSave;
				}
				else
				{
					mStopMove = true;
				}

			}
			else
			{
				mVelocity += (mPosition - vOrigin) * mDelta * mExplosiveForce;
				mVelocity.y += (mGravity * mDelta);
				mPosition += (mVelocity * mDelta);
			}
		}
	}
	Vector3 p = mPosition;
	//attachToEmitter������moveWithEmitter
	if(mEmitter->mAttatchEmitter)
	{
		p = m.applyToOrigin();
	}
	else if(mEmitter->mMoveWithEmitter)
	{
		Vector3 v =  m.applyToOrigin();
		//���ŹҽӵĶ�����ƶ����ƶ�
		p += (v - mNodeOriginalPosition);
		mOriginalPosition += (v - mNodeOriginalPosition);
		mNodeOriginalPosition = v;
		mPosition = p;
	}
	//Ȼ��������Ϊ����ԭ�㣬��������Ļ������������Σ���ת����֮
	mAngle += mRotateSpeed * mDelta;
	float scale;// = InterpolateBezier(mRate, mScale.x, mScale.y, mScale.z);
	//
	{
		if (mRate < mEmitter->mTime)
		{
			scale = InterpolateBezier(mRate/mEmitter->mTime, mScale.x, mScale.y);
		}
		else
		{
			scale = InterpolateBezier((mRate - mEmitter->mTime) / (1.0f - mEmitter->mTime), mScale.y, mScale.z);
		}
	}
	if(mEmitter->mHead)
	{
		float us = 1.0f;
		if (!almostEqual(mEmitter->mAspectRadio, 0.0f))
		{
			us = 1.0f / mEmitter->mAspectRadio;
		}
		Vector3 finalUp = up;
		Vector3 finalRight = right;
		float al = mAngle;
		if (mEmitter->mForSword)
		{
			Vector3 v = mVelocity;
			finalRight.crossProduct(v, forword);
			finalRight.normalise();
			finalUp.crossProduct(forword, finalRight);
			finalUp.normalise();
			//
			al = mEmitter->mSwordInitAngle;
		}
		
		Vector3 vt = finalUp * us;
		Vector3 v0 = finalRight + vt;
		Vector3 v1 = finalRight - vt;
		if(!almostEqual (al, 0.0f))
		{
			Quaternion q;
			q.fromAngleAxis(al * TwoPI / 360.f, forword);
			Matrix mr;
			mr.build(&q);
			v0 = mr.applyVector(v0);
			v1 = mr.applyVector(v1);
		}
		Vector3 vt2 = v0 * scale;
		Vector3 vt3 = v1 * scale;
		mVertices[0].position_ = p - vt2;
		mVertices[1].position_ = p + vt3;
		mVertices[2].position_ = p + vt2;
		mVertices[3].position_ = p - vt3;
	}
	if(mEmitter->mTail)
	{
		Vector3 v1 = right + up;
		Vector3 v2 = right - up;
		Vector3 v3 = v1 * scale;
		Vector3 v4 = v2 * scale;
		mVertices[0].position_ = mOriginalPosition - v3;
		mVertices[1].position_ = mOriginalPosition + v4;
		mVertices[2].position_ = p + v3;
		mVertices[3].position_ = p - v4;
	}
}
