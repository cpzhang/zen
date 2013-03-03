#pragma once
#include "common/dxHead.h"
#include "matrix.h"
#include "misc/helper.h"
#include "ConfigRender.h"
#include "misc/stdHead.h"
#include "Camera.h"
#include "vector2.h"
#include "misc\Singleton.h"
#include "Ray.h"
#include "Texture.h"
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
#pragma pack( push)
#pragma pack( 1 )

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
		return eVertexDeclarationType_PositionTexture;
	}
	static size_t getSize()
	{
		return sizeof(sVDT_PositionTexture);
	}
};
struct sVDT_PositionTTexture
{
	Vector3 position_;
	Vector2 texture_;
	static eVertexDeclarationType getType()
	{
		return eVertexDeclarationType_PositionTTexture;
	}
	static size_t getSize()
	{
		return sizeof(sVDT_PositionTTexture);
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
	Vector3	normal_;
	Vector2	texcoord_;
	u32		color_ARGB_;
	u8		bones_[4];
	Vector4    weights_;
	static eVertexDeclarationType getType()
	{
		return eVertexDeclarationType_PositionTextureBoneWeightColorNormal;
	}
	static size_t getSize()
	{
		return sizeof(sVDT_PositionTextureBoneWeightColorNormal);
	}
};
#pragma pack( pop )
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
enum eRenderTarget
{
	eRenderTarget_Implicit,
	eRenderTarget_Additional,
	eRenderTarget_Texture,
	eRenderTarget_Size,
};
class IRenderTarget
{
public:
	IRenderTarget();
	virtual ~IRenderTarget(){};
public:
	virtual void create(HWND h){};
	virtual void apply() = 0;
	virtual HRESULT present() = 0;
	virtual bool getBackBuffer(IDirect3DSurface9 **ppBackBuffer) = 0;
public:
	Matrix						projectionMatrix_;
	Matrix						viewMatrix_;
	Matrix						viewProjectionMatrix_;
	Matrix						viewMatrixInverse_;
	Camera						camera_;
};

class ApiRender_ RenderContext: public Singleton<RenderContext>
{
public:
	RenderContext();
	~RenderContext();
private:
	void clear_();
public:
	u32 createRenderTarget(eRenderTarget e);
	IRenderTarget* getRenderTarget(u32 k)
	{
		return renderTargets_[k];
	}
	void releaseRenderTarget(u32 k)
	{
		IRenderTarget* t = renderTargets_[k];
		if (t)
		{
			delete t;
			renderTargets_.erase(k);
		}
	}
	u32 getCurrentRenderTarget()
	{
		return currentRenderTargetKey_;
	}
	void setCurrentRenderTarget(u32 k);
	Ray getPickingRay();
	IDirect3DSurface9* getScreenCopy();

	void setWaitForVBL( bool wait );
	IDirect3DDevice9* getDxDevice() const;
	IDirect3D9* getDxD3D9() const;
	HRESULT beginScene();
	HRESULT endScene();
	void clearBindings();

	HRESULT present();

	bool create();
	void destroy();
	bool createDevice(HWND hWnd, u32 deviceIndex, u32 modeIndex, bool windowed,bool requireStencil,const Vector2 & windowedSize,bool forceRef = false);
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
	u32 drawIndexedPrimitive( D3DPRIMITIVETYPE type,	INT  baseVertexIndex, u32 minIndex, u32 numVertices, u32 startIndex, u32 primitiveCount );
	u32 drawIndexedPrimitiveUP( D3DPRIMITIVETYPE primitiveType,u32 minIndex, u32 numVertices, u32 primitiveCount, CONST void* pIndexData, D3DFORMAT indexDataFormat, CONST void* pVertexStreamZeroData, u32 vertexStreamZeroStride );
	u32 drawPrimitiveUP( D3DPRIMITIVETYPE primitiveType, u32 primitiveCount, CONST void* pVertexStreamZeroData, u32 vertexStreamZeroStride );

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

	const std::string screenShot( const std::string& fileExt /*= "bmp"*/, const std::string& fileName /*= "shot"*/, bool autoNumber /*= true*/ );

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
	/*Matrix						projectionMatrix_;
	Matrix						viewMatrix_;
	Matrix						viewProjectionMatrix_;
	Matrix						viewMatrixInverse_;*/
	//Camera						camera_;
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
	typedef stdext::hash_map<u32, IRenderTarget*> RenderTargetHashMap;
	RenderTargetHashMap renderTargets_;
	IRenderTarget* currentRenderTarget_;
	u32 currentRenderTargetKey_;
	u32 implicitRenderTargetKey_;
};

ApiRender_ bool createRenderContex();

ApiRender_ void destroyRenderContex();

ApiRender_ RenderContext* getRenderContex();

class ImplicitRenderTarget : public IRenderTarget
{
public:
	virtual void apply()
	{
		IDirect3DSurface9 *pBackBuffer = 0;
		getRenderContex()->getDxDevice()->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer);
		getRenderContex()->getDxDevice()->SetRenderTarget(0,pBackBuffer);
		pBackBuffer->Release();
	}
	virtual HRESULT present()
	{
		return getRenderContex()->getDxDevice()->Present(0,0,0,0);
	}
	virtual bool getBackBuffer(IDirect3DSurface9 **ppBackBuffer)
	{
		return SUCCEEDED(getRenderContex()->getDxDevice()->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,ppBackBuffer));
	}
};
class AdditionalRenderTarget : public IRenderTarget
{
public:
	AdditionalRenderTarget()
	{
		SwapChain_ = NULL;
	}
	~AdditionalRenderTarget()
	{
		if (SwapChain_)
		{
			SwapChain_->Release();
			SwapChain_ = NULL;
		}
	}
public:
	virtual void create(HWND h)
	{
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp,sizeof(d3dpp));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
		D3DDISPLAYMODE   mode;
		getRenderContex()->getDxD3D9()->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&mode);
		d3dpp.BackBufferFormat = mode.Format;
		d3dpp.hDeviceWindow = h;
		IDirect3DSwapChain9 *pSwapChain = 0;
		HRESULT hr = getRenderContex()->getDxDevice()->CreateAdditionalSwapChain(&d3dpp,&SwapChain_);
		
	}
	virtual void apply()
	{
		IDirect3DSurface9 *pBackBuffer = 0;
		SwapChain_->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer);
		getRenderContex()->getDxDevice()->SetRenderTarget(0,pBackBuffer);
		pBackBuffer->Release();
	}
	virtual HRESULT present()
	{
		return SwapChain_->Present(0,0,0,0,0);
	}
	virtual bool getBackBuffer(IDirect3DSurface9 **ppBackBuffer)
	{
		return SUCCEEDED(SwapChain_->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,ppBackBuffer));
	}
private:
	IDirect3DSwapChain9*	SwapChain_;
};
class TextureRenderTarget : public IRenderTarget
{
public:
	virtual void apply()
	{
		IDirect3DSurface9 *pSurface = 0;
		m_pTexture->getDxTexture()->GetSurfaceLevel(0,&pSurface);
		if(pSurface)
		{
			getRenderContex()->getDxDevice()->SetRenderTarget(0,pSurface);
			pSurface->Release();
		}
	}
	virtual HRESULT present()
	{
		return S_OK;
	};
	virtual bool getBackBuffer(IDirect3DSurface9 **ppBackBuffer)
	{
		return SUCCEEDED(m_pTexture->getDxTexture()->GetSurfaceLevel(0,ppBackBuffer));
	}
private:
	Texture*				m_pTexture;
};