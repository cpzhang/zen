#pragma once
#include "misc/dxHead.h"
#include "matrix.h"
#include "misc/helper.h"
#include "ConfigRender.h"
#include "misc/stdHead.h"
#include "Camera.h"
#include "vector2.h"
#include "misc\Singleton.h"
#include "Ray.h"
enum eVertexDeclarationType
{
	eVertexDeclarationType_Null,
	eVertexDeclarationType_Position,
	eVertexDeclarationType_PositionBoneWeight,
	eVertexDeclarationType_PositionColor,
	eVertexDeclarationType_PositionTexture,
	eVertexDeclarationType_PositionBlend,
	eVertexDeclarationType_PositionBlendNormal,
	eVertexDeclarationType_PositionTextureBoneWeight,
	eVertexDeclarationType_PositionTextureBoneWeightColor,
	eVertexDeclarationType_PositionTextureBoneWeightColorNormal,
	eVertexDeclarationType_PositionTextureNormal,
	eVertexDeclarationType_PositionColorTexture,
	eVertexDeclarationType_PositionTTexture,
	eVertexDeclarationType_Matrix,
	eVertexDeclarationType_PositionTextureNormalMatrix,
	eVertexDeclarationType_PositionTextureNormalIndex,
	eVertexDeclarationType_Size,
};

struct sVDT_Position
{
	Vector3 position_;
	static eVertexDeclarationType getType()
	{
		return eVertexDeclarationType_Position;
	}
	static size_t getSize()
	{
		return sizeof(sVDT_Position);
	}
};
struct sVDT_PositionTexture
{
	Vector3 position_;
	Vector2 texture_;
	static eVertexDeclarationType getType()
	{
		return eVertexDeclarationType_PositionColorTexture;
	}
	static size_t getSize()
	{
		return sizeof(sVDT_PositionTexture);
	}
};
struct sVDT_PositionColor
{
	Vector3 position_;
	u32	color_;
	static eVertexDeclarationType getType()
	{
		return eVertexDeclarationType_PositionColor;
	}
	static size_t getSize()
	{
		return sizeof(sVDT_PositionColor);
	}
};
struct sVDT_PositionColorTexture
{
	Vector3 position_;
	u32	color_;
	Vector2 texture_;
	static eVertexDeclarationType getType()
	{
		return eVertexDeclarationType_PositionColorTexture;
	}
	static size_t getSize()
	{
		return sizeof(sVDT_PositionColorTexture);
	}
};
struct sVDT_PositionTextureBoneWeightColor
{
	//
	Vector3	position_;
	//
	Vector2 texcoord_;
	//
	u8	bones_[4];
	//
	Vector4 weights_;
	//
	u32		color_ARGB_;
	static eVertexDeclarationType getType()
	{
		return eVertexDeclarationType_PositionTextureBoneWeightColor;
	}
	static size_t getSize()
	{
		return sizeof(sVDT_PositionTextureBoneWeightColor);
	}
};
//
struct sVDT_PositionTextureBoneWeightColorNormal
{

	//
	Vector3	position_;
	//
	Vector2	texcoord_;
	//
	u8	bones_[4];
	//
	Vector4    weights_;
	//
	u32		color_ARGB_;

	Vector3	normal_;
	static eVertexDeclarationType getType()
	{
		return eVertexDeclarationType_PositionTextureBoneWeightColorNormal;
	}
	static size_t getSize()
	{
		return sizeof(sVDT_PositionTextureBoneWeightColorNormal);
	}
};

enum eCompatibilityFlag
{
	eCompatibilityFlag_NOOVERWRITE =	1 << 0,
	eCompatibilityFlag_NVIDIA =			1 << 1,
	eCompatibilityFlag_ATI =			1 << 2
};

	//
struct DeviceInfo
{
	D3DADAPTER_IDENTIFIER9			identifier_;
	D3DCAPS9						caps_;
	u32							adapterID_;
	bool							windowed_;
	D3DDISPLAYMODE					windowedDisplayMode_;
	std::vector< D3DDISPLAYMODE >	displayModes_;
	u32							compatibilityFlags_;
};

class ApiRender_ RenderContext: public Singleton<RenderContext>
{
public:
	RenderContext();
	~RenderContext();
private:
	void clear_();
public:
	Ray getPickingRay();
	IDirect3DSurface9* getScreenCopy();

	void setWaitForVBL( bool wait );
	IDirect3DDevice9* getDxDevice() const;
	HRESULT beginScene();
	HRESULT endScene();
	void clearBindings();

	HRESULT present();

	bool create();
	void destroy();
	bool createDevice(HWND hWnd, u32 deviceIndex, u32 modeIndex, bool windowed,bool requireStencil,const Vector2 & windowedSize,bool forceRef = false);
//	u32 setVertexDeclaration( IDirect3DVertexDeclaration9* pVD );
	u32 setFVF( u32 fvf );
	void resetWorldMatricesStack();
	void updateViewProjectionMatrix();
	void updateProjectionMatrix();
	const Matrix& getViewMatrix( ) const;

	Matrix& getViewMatrix();
	void setViewMatrix(const Matrix& m);
	const Matrix& getProjectionMatrix( ) const;

	Matrix& getProjectionMatrix();
	Matrix& getViewProjectionMatrix();
	bool checkDevice(bool& reset);
	bool resetDevice();
	void releaseUnmanaged();
	void createUnmanaged();
	float getScreenWidth() const;

	float getScreenHeight() const;
	void setScreenWidth( int width );

	void setScreenHeight( int height );
	void updateDeviceInfo( );
	void fillPresentationParameters( void );
	D3DFORMAT getMatchingZBufferFormat( D3DFORMAT colourFormat, bool stencilWanted, bool & stencilAvailable );
	bool changeModePriv( u32 modeIndex, bool windowed, bool testCooperative, u32 backBufferWidthOverride );
	void setGammaCorrection();
	void initRenderStates();
	bool isStencilAvailable( ) const;
	bool checkDeviceYield(bool& reset);
	bool getTripleBuffering() const;
	void setTripleBuffering( bool enabled );
	bool isWindowed( ) const;
	const DeviceInfo& getDeviceInfo( u32 i ) const;
	u32 getDevicesNumber( ) const;
	void setFullScreenAspectRatio( float ratio );
	bool isMixedVertexProcessing() const;
	bool changeMode( u32 modeIndex, bool windowed, bool testCooperative = false, u32 backBufferWidthOverride = 0);
	bool isInitialized();
	u32 drawIndexedPrimitive( D3DPRIMITIVETYPE type,	INT  baseVertexIndex, UINT minIndex, UINT numVertices, UINT startIndex, UINT primitiveCount );
	u32 drawIndexedPrimitiveUP( D3DPRIMITIVETYPE primitiveType,UINT minIndex, UINT numVertices, UINT primitiveCount, CONST void* pIndexData, D3DFORMAT indexDataFormat, CONST void* pVertexStreamZeroData, UINT vertexStreamZeroStride );
	u32 drawPrimitiveUP( D3DPRIMITIVETYPE primitiveType, UINT primitiveCount, CONST void* pVertexStreamZeroData, UINT vertexStreamZeroStride );

	bool setVertexDeclaration( eVertexDeclarationType e );
	void setCamera(Camera& c);
	Camera getCamera();

	//
	HRESULT SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix);

	//
	HRESULT applyViewMatrix();
	HRESULT applyProjectionMatrix();

	//
	HRESULT LightEnable(DWORD LightIndex, BOOL bEnable);

private:
	// When converting a legacy application to Direct3D 9, you must add a call to either IDirect3DDevice9::SetFVF to use the fixed function pipeline, 
	// or IDirect3DDevice9::SetVertexDeclaration to use a vertex shader before you make any Draw calls.
	void initVertexDeclarations_();
private:
	//
	IDirect3DVertexDeclaration9*	vertexDeclarations_[eVertexDeclarationType_Size];
	float						fullScreenAspectRatio_;
	float						gammaCorrection_;
	bool						mrtSupported_;
	u16                      maxAnisotropy_;
	u16                      maxSimTextures_;
	u16						psVersion_;
	u16						vsVersion_;
	RECT						windowedRect_;
	Vector2						windowedSize_;
	bool						mixedVertexProcessing_;
	IDirect3DDevice9* Direct3DDevice9_;
	IDirect3D9* Direct3D9_;
	std::vector< DeviceInfo > devicesInfo_;
	u32						modeIndex_;
	bool						windowed_;
	HWND						windowHandle_;
	u32 deviceIndex_;
	IDirect3DVertexDeclaration9* vertexDeclaration_;
	u32 fvf_;
	std::stack<Matrix> worldMatrices_;
	Matrix						projectionMatrix_;
	Matrix						viewMatrix_;
	Matrix						viewProjectionMatrix_;
	Matrix						viewMatrixInverse_;
	Camera						camera_;
	bool						changingMode_;
	u32						backBufferWidthOverride_;
	D3DPRESENT_PARAMETERS		presentParameters_;
	float						halfScreenWidth_;
	float						halfScreenHeight_;
	bool						waitForVBL_;
	bool						tripleBuffering_;
	D3DSURFACE_DESC				backBufferDesc_;
	IDirect3DSurface9*			screenCopySurface_;
	bool lost_;
	bool						stencilWanted_;
	bool						stencilAvailable_;
	D3DDEVTYPE					deviceType_;
	LONG						windowedStyle_;
	bool						initialized_;
};

ApiRender_ bool createRenderContex();

ApiRender_ void destroyRenderContex();

ApiRender_ RenderContext* getRenderContex();
