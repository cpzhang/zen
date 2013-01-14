#pragma once
#include "render/KeyFrames.h"
#include "ConfigModel.h"
#include "ModelResManager.h"
struct sBoneKFs
{
public:
	sBoneKFs();
private:
	void _clear();
public:
	int id;
	sKeyFrameSet<Quaternion>	rotationKFs;			/// 旋转关键帧
	sKeyFrameSet<Vector3>		scaleKFs;				/// 缩放关键帧
	sKeyFrameSet<Vector3>		translationKFs;			/// 平移关键帧
};

struct Skin : public IModelRes
{
public:
	Skin();
public:
	virtual bool create(const tstring& resID);
	virtual void destroy();
public:
	static eModelResType getType()
	{
		return eModelResType_Skin;
	}
public:
	static Skin* getNullObject()
	{
		static Skin s;
		return &s;
	}
private:
	void _clear();
public:
	char mVersion;
	std::vector<sBoneKFs> boneKFs;
};