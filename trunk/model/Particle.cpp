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

void Particle::update(float delta, const Vector3& right, const Vector3& up, const Vector3& forword)
{
	mDelta = delta  * 0.001f;
	mAge += mDelta;
	if (!isAlive())
	{
		return;
	}
	mRate = mAge / mLife;
	//秒
	mDeltaTime += mDelta;
	updateUV_();
	updateColor_();
	updatePostion_(right, up, forword);
}

bool Particle::isAlive()
{
	return (mAge <= mLife) && (mLife > 0.0001f);
}

void Particle::_clear()
{
	mEmitter = NULL;
}

void Particle::updateUV_()
{
	//换贴图
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

	color2.x = mColorMiddle.x * 2.0f;
	color2.y = mColorMiddle.y * 2.0f;
	color2.z = mColorMiddle.z * 2.0f;
	color2.w = mAlpha.y * 2.0f;

	color3.x = mColorEnd.x;
	color3.y = mColorEnd.y;
	color3.z = mColorEnd.z;
	color3.w = mAlpha.z;

	mColor = InterpolateBezier(mRate, color1, color2, color3);
	{
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

void Particle::updatePostion_(const Vector3& right, const Vector3& up, const Vector3& forword)
{
	//首先更新中心位置
	//如果是Wander粒子
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
		//否则处理传统的流程
		if(!mStopMove)
		{
			Vector3 vOrigin = Vector3::Zero;
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
				//mVelocity += (mPosition - vOrigin) * timeFactor * mExplosiveForce;
				//mVelocity.y += (mGravity * timeFactor);
				//mPosition += (mVelocity * timeFactor);
				//重力导致的位移
				float d = 0.5f * 9.8f * mAge * mAge;
				mPosition.y -= d;
			}
		}
	}
	//attachToEmitter优先于moveWithEmitter
	if(mEmitter->mAttatchEmitter)
	{
	}
	else if(mEmitter->mMoveWithEmitter)
	{
	}
	//然后，以中心为坐标原点，以摄像机的基，构建正方形，旋转缩放之
	mAngle += mRotateSpeed * mDelta;
	float scale = InterpolateBezier(mRate, mScale.x, mScale.y, mScale.z);
	if(mEmitter->mHead)
	{
		Vector3 v0 = -right + up;
		Vector3 v1 = -right - up;
		Vector3 v2 = right - up;
		Vector3 v3 = right + up;
		//if(0)
		{
			Matrix ms;
			ms.setScale(scale, scale, scale);
			//绕vAxis旋转angle角度
			Quaternion q;
			q.fromAngleAxis(mAngle, forword);
			Matrix mr;
			mr.setRotate(&q);
			Matrix m = mr * ms;
			v0 = m.applyVector(v0);
			v1 = m.applyVector(v1);
			v2 = m.applyVector(v2);
			v3 = m.applyVector(v3);
		}
		{
			mVertices[0].position_ = mPosition + v0;
			mVertices[1].position_ = mPosition + v1;
			mVertices[2].position_ = mPosition + v2;
			mVertices[3].position_ = mPosition + v3;
		}
		//
		//std::ostringstream ss;
		//ss<<mPosition.x<<","<<mPosition.y<<","<<mPosition.z;
		//::OutputDebugString(ss.str().c_str());
	}
	if(mEmitter->mTail)
	{
		Vector3 v1 = right + up;
		Vector3 v2 = right - up;
		Vector3 v3 = v1 * scale;
		Vector3 v4 = v2 * scale;
		mVertices[0].position_ = mOriginalPosition - v3;
		mVertices[1].position_ = mOriginalPosition + v4;
		mVertices[2].position_ = mPosition + v3;
		mVertices[3].position_ = mPosition - v4;
	}
}
