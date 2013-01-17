#pragma once
#include "render/KeyFrames.h"
#include "Particle.h"
#include "ModelResManager.h"
#include "tinyXML2/tinyxml2.h"
class ParticleEmitter : public IModelRes
{
public:
	template<class T>
	void getKeyFrame(tinyxml2::XMLElement* r, const tstring& name, sKeyFrameSet<T>& kfs)
	{
		tinyxml2::XMLElement* a = r->FirstChildElement(name.c_str());
		if (a)
		{
			T s;
			if (tinyxml2::XML_SUCCESS == a->QueryFloatAttribute("static", &s))
			{
				kfs.setStaticData(s);
			}
			tinyxml2::XMLElement* mat = a->FirstChildElement("KeyFrame");
			while(mat)
			{
				sKeyFrame<T> tss;
				mat->QueryUnsignedAttribute("time", &tss.time);
				mat->QueryFloatAttribute("value", &tss.v);
				kfs.addKeyFrame(tss);
				mat = mat->NextSiblingElement("KeyFrame");
			}
		}		
	}
	template<>
	void getKeyFrame(tinyxml2::XMLElement* r, const tstring& name, sKeyFrameSet<bool>& kfs)
	{
		tinyxml2::XMLElement* a = r->FirstChildElement(name.c_str());
		if (a)
		{
			bool s;
			if (tinyxml2::XML_SUCCESS == a->QueryBoolAttribute("static", &s))
			{
				kfs.setStaticData(s);
			}
			tinyxml2::XMLElement* mat = a->FirstChildElement("KeyFrame");
			while(mat)
			{
				sKeyFrame<bool> tss;
				mat->QueryUnsignedAttribute("time", &tss.time);
				mat->QueryBoolAttribute("value", &tss.v);
				kfs.addKeyFrame(tss);
				mat = mat->NextSiblingElement("KeyFrame");
			}
		}		
	}
	void getValue(tinyxml2::XMLElement* r, const tstring& name, float& v)
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement(name.c_str());
		if (mat)
		{
			mat->QueryFloatAttribute("value", &v);
		}
	}
	void getValue(tinyxml2::XMLElement* r, const tstring& name, Vector3& v)
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement(name.c_str());
		if (mat)
		{
			mat->QueryVector3Attribute("value", &v);
		}
	}
	void getValue(tinyxml2::XMLElement* r, const tstring& name, int& v)
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement(name.c_str());
		if (mat)
		{
			mat->QueryIntAttribute("value", &v);
		}
	}
	void getValue(tinyxml2::XMLElement* r, const tstring& name, tstring& v)
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement(name.c_str());
		if (mat)
		{
			v = mat->Attribute("value");
		}
	}
	void getValue(tinyxml2::XMLElement* r, const tstring& name, bool& v)
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement(name.c_str());
		if (mat)
		{
			mat->QueryBoolAttribute("value", &v);
		}
	}
public:
	virtual bool create(const tstring& resID);
	virtual void destroy();

public:
	ParticleEmitter();
	~ParticleEmitter();
public:
	static eModelResType getType()
	{
		return eModelResType_ParticleEmitter;
	}
public:
	static ParticleEmitter* getNullObject()
	{
		static ParticleEmitter s;
		return &s;
	}
public:
	void spawn(float delta, const AnimationTime& at, ParticleList& ps);
	template<class T>
	void addKeyFrame(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* ele, const tstring& name, const sKeyFrameSet<T>& kfs)
	{
		tinyxml2::XMLElement* a = doc.NewElement(name.c_str());
		a->SetAttribute("static", kfs.getStaticData());
		for (size_t i = 0; i != kfs.numKeyFrames(); ++i)
		{
			tinyxml2::XMLElement* b = doc.NewElement("KeyFrame");
			b->SetAttribute("time", kfs.getKeyFrame(i)->time);
			b->SetAttribute("value", kfs.getKeyFrame(i)->v);
			a->LinkEndChild(b);
		}
		ele->LinkEndChild(a);
	}
	template<class T>
	void addValue(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* ele, const tstring& name, const T& v)
	{
		tinyxml2::XMLElement* a = doc.NewElement(name.c_str());
		a->SetAttribute("value", v);
		ele->LinkEndChild(a);
	}
	void save(const tstring& fn);
private:
	void _clear();
public:
	//xml
	tstring mBoneName;
	tstring mTextureFile;
	int mBlendMode;
	//
	float mCurrentEmission;
	Vector3 mColorStart;
	Vector3 mColorMiddle;
	Vector3 mColorEnd;
	Vector3 mAlpha;

	//
	int mInitNumber;
	int mLimitNumber;
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
	int mChangeStyle;
	int mChangeInterval;
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
	sKeyFrameSet<float> mSpeedKFs;
	sKeyFrameSet<float> mVariationKFs;
	sKeyFrameSet<float> mLatitude;
	sKeyFrameSet<float> mGravity;
	sKeyFrameSet<float> mExplosiveForce;
	sKeyFrameSet<bool> mVisibility;
	sKeyFrameSet<float> mEmitRate;
	sKeyFrameSet<float> mLength;
	sKeyFrameSet<float> mWidth;
	sKeyFrameSet<float> mHeigth;
	int mVersion;
};
