#pragma once
#include "misc/helper.h"
#include "misc/dxHead.h"
#include "ConfigModel.h"
#include "misc/stdHead.h"
#include "misc/NamedObjectManager.h"
#include "misc/Singleton.h"
#include "render/Fx.h"
#include "render/KeyFrames.h"
#include "ModelResManager.h"
class Fx;
/*
Sets the state value for the currently assigned texture
*/
struct sTextureStageState
{
	/*
	Stage identifier of the texture for which the state value is set. Stage identifiers are zero-based. Devices can have up to eight set textures, so the maximum value allowed for Stage is 7. 
	*/
	DWORD stage_;
	/*
	Texture state to set. This parameter can be any member of the D3DTEXTURESTAGESTATETYPE enumerated type
	*/
	D3DTEXTURESTAGESTATETYPE type_;
	/*
	State value to set. The meaning of this value is determined by the Type parameter
	*/
	DWORD value_;
};
typedef std::vector<sTextureStageState> TextureStageStateVec;
//
struct sRenderState
{
	/*
	Device state variable that is being modified. This parameter can be any member of the D3DRENDERSTATETYPE enumerated type. 
	*/
	D3DRENDERSTATETYPE State_; 
	/*
	New value for the device render state to be set. The meaning of this parameter is dependent on the value specified for State. For example, if State were D3DRS_SHADEMODE, the second parameter would be one member of the D3DSHADEMODE enumerated type. 
	*/
	DWORD Value_; 
};
union uRenderStateValue
{
	DWORD dw_;
};

typedef std::vector<sRenderState> RenderStateVec;
// fx + texture + pipe state
class ApiModel_ Material : public IModelRes
{
public:
	Material();
	~Material();
public:
	virtual bool create(const tstring& resID);
	virtual void destroy();
public:
	static eModelResType getType()
	{
		return eModelResType_Material;
	}
public:
	Fx* getFx();
	void apply(const Matrix& uvMat, const Vector4& tf);
	void cancel();
	tstring getFilePath();
	//void update(float delta);
public:
	static Material* getNullObject()
	{
		static Material s;
		return &s;
	}
	static void initRenderStateDefault();
private:
	void clear_();
private:
public:
	tstring Name_;
	tstring FilePath_;
	tstring FxFileName_;
	Fx* Fx_;
	tstring TextureVariableName_;
	tstring TextureFileName_;
	//
	RenderStateVec RenderStates_;
	//
	TextureStageStateVec TextureStageStates_;
	//u32 Diffuse_;
	sKeyFrameSet<float>		RotationKFs_;			/// 旋转关键帧
	//sKeyFrameSet<Vector3>		mScaleKFs_;				/// 缩放关键帧
	sKeyFrameSet<float>		FlowUKFs_;				/// 平移关键帧
	sKeyFrameSet<float>		FlowVKFs_;				/// 平移关键帧
	sKeyFrameSet<Vector3>	ColorKFs_;				/// 颜色关键帧
	sKeyFrameSet<float>		AlphaKFs_;				/// 透明度关键帧
	//序列图
	int mRows;
	int mCols;
	int mChangeInterval;
	int mChangeStyle;
public:
	static std::vector<DWORD> tRenderStateDefault_;
	static std::vector<DWORD> tTextureStageStateDefault_;
	static bool tRSDIntialized_;
};
typedef std::vector<Material*> MaterialVec;
