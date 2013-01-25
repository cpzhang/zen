#include "ParticleEmitter.h"
ParticleEmitter::ParticleEmitter()
{
	_clear();		
}

ParticleEmitter::~ParticleEmitter()
{
	_clear();
}

void ParticleEmitter::spawn(float delta, const AnimationTime& at, ParticleList& ps, const Matrix& m)
{
	if (!mVisibility.getFrame(at.current))
	{
		return;
	}
	//换算成秒
	float timeFactor = delta / 1000.0f;
	mCurrentEmission += mEmitRate.getFrame(at) * timeFactor;
	while(mCurrentEmission >= 1.0f)
	{
		if (ps.size() >= mLimitNumber /*|| !ps.empty()*/)
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
		p.mPosition = m.applyVector(p.mPosition);
		p.mOriginalPosition = p.mPosition;
		p.mNodeOriginalPosition = m.applyToOrigin();
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
		rot.multiply(mtxX, mtxY);
		p.mVelocity = rot.applyVector(Vector3(0,1,0));
		p.mVelocity.normalise();
		p.mVelocity = m.applyVectorNormal(p.mVelocity);

		p.mVelocity *= speed;

		p.mAge = 0.0f;
		if(mLifeSpan < mLifeVar)
			p.mLife = mLifeSpan;
		else
			p.mLife= randomReal(mLifeSpan - mLifeVar, mLifeSpan + mLifeVar);

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
		//
		mCurrentEmission -= 1.0f;
	}
}

void ParticleEmitter::_clear()
{
	mCurrentEmission = 0.0f;
	mVersion = 0;
}

void ParticleEmitter::destroy()
{

}

void ParticleEmitter::save( const tstring& fn )
{
	//============================================================================
	tinyxml2::XMLDocument doc;
	// 
	tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
	doc.LinkEndChild(dec);
	//
	tinyxml2::XMLElement* ele = doc.NewElement("ParticleEmitter");
	doc.LinkEndChild(ele);
	//
	addValue(doc, ele, "Alpha", mAlpha);
	addValue(doc, ele, "AttachEmitter", mAttatchEmitter);
	addValue(doc, ele, "AspectRadio", mAspectRadio);

	//
	addValue(doc, ele, "BlendMode", mBlendMode);
	//
	addValue(doc, ele, "ChangeStyle", mChangeStyle);
	addValue(doc, ele, "ChangeInterval", mChangeInterval);
	addValue(doc, ele, "Cols", mCols);
	addValue(doc, ele, "ColorStart", mColorStart);
	addValue(doc, ele, "ColorMiddle", mColorMiddle);
	addValue(doc, ele, "ColorEnd", mColorEnd);
	
	addValue(doc, ele, "FixedSize", mFixedSize);
	addValue(doc, ele, "ForSword", mForSword);

	addValue(doc, ele, "Head", mHead);
	addValue(doc, ele, "HeadDecay", mHeadDecay);
	addValue(doc, ele, "HeadLifeSpan", mHeadLifeSpan);
	
	addValue(doc, ele, "InitAngleBegin", mInitAngleBegin);
	addValue(doc, ele, "InitAngleEnd", mInitAngleEnd);
	addValue(doc, ele, "InitNumber", mInitNumber);
	
	addValue(doc, ele, "LifeSpan", mLifeSpan);
	addValue(doc, ele, "LifeVar", mLifeVar);
	
	addValue(doc, ele, "MaxNumber", mLimitNumber);
	addValue(doc, ele, "MoveWithEmitter", mMoveWithEmitter);
	
	addValue(doc, ele, "RotateSpeed", mRotateSpeed);
	addValue(doc, ele, "RotateSpeedVar", mRotateSpeedVar);
	addValue(doc, ele, "Rows", mRows);

	addValue(doc, ele, "Scale", mScale);
	addValue(doc, ele, "ScaleVar", mScaleVar);
	addValue(doc, ele, "SwordInitAngle", mSwordInitAngle);

	addValue(doc, ele, "Tail", mTail);
	addValue(doc, ele, "TailLifeSpan", mTailLifeSpan);
	addValue(doc, ele, "TailDecay", mTailDecay);	
	addValue(doc, ele, "Texture", mTextureFile.c_str());
	addValue(doc, ele, "Time", mTime);

	addValue(doc, ele, "Wander", mWander);
	addValue(doc, ele, "WanderRadius", mWanderRadius);
	addValue(doc, ele, "WanderSpeed", mWanderSpeed);
	//
	addKeyFrame<float>(doc, ele, "EmitRate", mEmitRate);
	addKeyFrame<float>(doc, ele, "ExplosiveForce", mExplosiveForce);
	addKeyFrame<float>(doc, ele, "Gravity", mGravity);
	addKeyFrame<float>(doc, ele, "Height", mHeigth);
	addKeyFrame<float>(doc, ele, "Length", mLength);
	addKeyFrame<float>(doc, ele, "Latitude", mLatitude);
	addKeyFrame<float>(doc, ele, "Speed", mSpeedKFs);
	addKeyFrame<float>(doc, ele, "Variation", mVariationKFs);
	addKeyFrame<bool>(doc, ele,	 "Visibility", mVisibility);
	addKeyFrame<float>(doc, ele, "Width", mWidth);
	//
	doc.SaveFile(fn.c_str());
}

bool ParticleEmitter::create( const tstring& resID )
{
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(resID.c_str()))
	{
		return false;
	}
	tinyxml2::XMLElement* ele = doc.RootElement();
	if (NULL == ele)
	{
		return false;
	}
	//
	getValue(ele, "Alpha", mAlpha);
	getValue(ele, "AttachEmitter", mAttatchEmitter);
	getValue(ele, "AspectRadio", mAspectRadio);

	//
	getValue(ele, "BlendMode", mBlendMode);
	//
	getValue(ele, "ChangeStyle", mChangeStyle);
	getValue(ele, "ChangeInterval", mChangeInterval);
	getValue(ele, "Cols", mCols);
	getValue(ele, "ColorStart", mColorStart);
	getValue(ele, "ColorMiddle", mColorMiddle);
	getValue(ele, "ColorEnd", mColorEnd);

	getValue(ele, "FixedSize", mFixedSize);
	getValue(ele, "ForSword", mForSword);

	getValue(ele, "Head", mHead);
	getValue(ele, "HeadDecay", mHeadDecay);
	getValue(ele, "HeadLifeSpan", mHeadLifeSpan);

	getValue(ele, "InitAngleBegin", mInitAngleBegin);
	getValue(ele, "InitAngleEnd", mInitAngleEnd);
	getValue(ele, "InitNumber", mInitNumber);

	getValue(ele, "LifeSpan", mLifeSpan);
	getValue(ele, "LifeVar", mLifeVar);

	getValue(ele, "MaxNumber", mLimitNumber);
	getValue(ele, "MoveWithEmitter", mMoveWithEmitter);

	getValue(ele, "RotateSpeed", mRotateSpeed);
	getValue(ele, "RotateSpeedVar", mRotateSpeedVar);
	getValue(ele, "Rows", mRows);

	getValue(ele, "Scale", mScale);
	getValue(ele, "ScaleVar", mScaleVar);
	getValue(ele, "SwordInitAngle", mSwordInitAngle);

	getValue(ele, "Tail", mTail);
	getValue(ele, "TailLifeSpan", mTailLifeSpan);
	getValue(ele, "TailDecay", mTailDecay);	
	getValue(ele, "Texture", mTextureFile);
	getValue(ele, "Time", mTime);

	getValue(ele, "Wander", mWander);
	getValue(ele, "WanderRadius", mWanderRadius);
	getValue(ele, "WanderSpeed", mWanderSpeed);
	//
	getKeyFrame<float>(ele, "EmitRate", mEmitRate);
	getKeyFrame<float>(ele, "ExplosiveForce", mExplosiveForce);
	getKeyFrame<float>(ele, "Gravity", mGravity);
	getKeyFrame<float>(ele, "Height", mHeigth);
	getKeyFrame<float>(ele, "Latitude", mLatitude);
	getKeyFrame<float>(ele, "Length", mLength);
	getKeyFrame<float>(ele, "Speed", mSpeedKFs);
	getKeyFrame<float>(ele, "Variation", mVariationKFs);
	getKeyFrame<bool>(ele,	"Visibility", mVisibility);
	getKeyFrame<float>(ele, "Width", mWidth);

	//
	mCurrentEmission = mInitNumber;
}
