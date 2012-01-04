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
	static u16 s_indices[] = {0, 1, 3, 1, 2, 3};
	getRenderContex()->drawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, s_indices, D3DFMT_INDEX16, mVertices, sVDT_PositionColorTexture::getSize());
}

void Particle::update(float delta)
{
	float timeFactor;
	//秒
	mDeltaTime += delta;
	timeFactor = delta  * 0.001f;
	mAge -= timeFactor;
	if(mAge <= 0.0f)
	{
		return;
	}

	//如果是Wander粒子
	if(mWander)
	{
		mWanderS += timeFactor * mWanderSpeed;
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
				mVelocity += (mPosition - vOrigin) * timeFactor * mExplosiveForce;
				mVelocity.y += (mGravity * timeFactor);
				posNew += (mVelocity * timeFactor);
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
				mVelocity += (mPosition - vOrigin) * timeFactor * mExplosiveForce;
				mVelocity.y += (mGravity * timeFactor);

				mPosition += (mVelocity * timeFactor);
			}
		}
	}

	float lifeFactor = almostEqual(randomReal(mLife,0.0f), 0.0f) ? 0.0f : (1.0f - (mAge / mLife));
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

	float scale;
	if(lifeFactor < mEmitter->mTime)
	{
		float tempInterval = mEmitter->mTime;
		float tempFactor = (randomReal(tempInterval,0.0f) > 0.0f) ? 0.0f : (lifeFactor / tempInterval);

		scale = mScale.x + tempFactor * (mScale.y- mScale.z);
		mColor = InterpolateBezier(tempFactor,color1,color2);
	}
	else
	{
		float tempInterval = 1.0f - mEmitter->mTime;
		float tempFactor = (tempInterval <= 0.0f) ? 0.0f : ((lifeFactor - mEmitter->mTime) / tempInterval);

		scale = mScale.y + tempFactor * (mScale.z - mScale.y);
		mColor = InterpolateBezier(tempFactor,color2,color3);
	}
	mVertices[0].color_ = Colour::getUint32(mColor);
	mVertices[1].color_ = Colour::getUint32(mColor);
	mVertices[2].color_ = Colour::getUint32(mColor);
	mVertices[3].color_ = Colour::getUint32(mColor);
	Vector3 look;
	Vector3 _up;
	Matrix mtxNode;
	Vector3 pos = mPosition;
	//attachToEmitter优先于moveWithEmitter
	if(mEmitter->mAttatchEmitter)
	{
		pos = Vector3::Zero;
	}
	else if(mEmitter->mMoveWithEmitter)
	{
	}

	{
		Vector3 xlook = Vector3(-1.224745f,1,1.224745f);
		xlook.normalise();
//		look=cam->getDirection();
//		_up = cam->getUp();
	}

	Vector3 _right = _up * look;
	_right.normalise();
	_up = look * _right;
	_up.normalise();

	mAngle += mRotateSpeed * timeFactor;

	float aspectRatio1 = 1.0f;
	if(!randomReal(mEmitter->mAspectRadio,0.0f))
		aspectRatio1 = 1.0f / mEmitter->mAspectRadio;
	if(mEmitter->mHead)
	{
		Vector3 vAxis;
		//绕vAxis旋转angle角度
		Quaternion q;
		if(!randomReal(mAngle,0.0f))
		{
			vAxis = _right * _up;
			q.fromAngleAxis(mAngle,vAxis);
		}

		Vector3 vt = _up * aspectRatio1;
		Vector3 v0 = _right + vt;
		Vector3 v1 = _right - vt;
		Vector3 v2 = v0;
		Vector3 v3 = v1;
		if(!randomReal(mAngle,0.0f))
		{
			Matrix m;
			m.setRotate(&q);
			v0 = m.applyVector(v0);
			v1 = m.applyVector(v1);
			v2 = m.applyVector(v2);
			v3 = m.applyVector(v3);
		}
		Vector3 vt2 = v0 * scale;
		Vector3 vt3 = v1 * scale;
		mVertices[0].position_ = pos - vt2;
		mVertices[1].position_ = pos + vt3;
		mVertices[2].position_ = pos + vt2;
		mVertices[3].position_ = pos - vt3;
	}
	if(mEmitter->mTail)
	{
		Vector3 v1 = _right + _up;
		Vector3 v2 = _right - _up;
		Vector3 v3 = v1 * scale;
		Vector3 v4 = v2 * scale;
		mVertices[0].position_ = mOriginalPosition - v3;
		mVertices[1].position_ = mOriginalPosition + v4;
		mVertices[2].position_ = pos + v3;
		mVertices[3].position_ = pos - v4;
	}
}

bool Particle::isAlive()
{
	return mAge > 0.0f;
}

void Particle::_clear()
{
	mEmitter = NULL;
}