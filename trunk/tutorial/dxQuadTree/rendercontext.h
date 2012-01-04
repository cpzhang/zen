#pragma once
#include "dxHead.h"
#include "matrix.h"
#include "helper.h"
#include "stdHead.h"
#include "Camera.h"
#include "vector2.h"
#include "Singleton.h"
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
	uint32							adapterID_;
	bool							windowed_;
	D3DDISPLAYMODE					windowedDisplayMode_;
	std::vector< D3DDISPLAYMODE >	displayModes_;
	uint32							compatibilityFlags_;
};

class RenderContext: public Singleton<RenderContext>
{
public:
	RenderContext();
	~RenderContext();
private:
	void clear_();
public:
	IDirect3DSurface9* getScreenCopy();

	void setWaitForVBL( bool wait );
	IDirect3DDevice9* getDxDevice() const;
	HRESULT beginScene();
	HRESULT endScene();
	void clearBindings();

	HRESULT present();

	bool create();
	void destroy();
	bool createDevice(HWND hWnd, uint32 deviceIndex, uint32 modeIndex, bool windowed,bool requireStencil,const Vector2 & windowedSize,bool forceRef);
	uint32 setVertexDeclaration( IDirect3DVertexDeclaration9* pVD );
	uint32 setFVF( uint32 fvf );
	void resetWorldMatricesStack();
	void updateViewProjectionMatrix();
	void updateProjectionMatrix();
	const Matrix& getViewMatrix( ) const;

	Matrix& getViewMatrix();
	const Matrix& getProjectionMatrix( ) const;

	Matrix& getProjectionMatrix();
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
	bool changeModePriv( uint32 modeIndex, bool windowed, bool testCooperative, uint32 backBufferWidthOverride );
	void setGammaCorrection();
	void initRenderStates();
	bool isStencilAvailable( ) const;
	bool checkDeviceYield(bool& reset);
	bool getTripleBuffering() const;
	void setTripleBuffering( bool enabled );
private:
	float						gammaCorrection_;
	bool						mrtSupported_;
	uint16                      maxAnisotropy_;
	uint16                      maxSimTextures_;
	uint16						psVersion_;
	uint16						vsVersion_;
	RECT						windowedRect_;
	Vector2						windowedSize_;
	bool						mixedVertexProcessing_;
	IDirect3DDevice9* Direct3DDevice9_;
	IDirect3D9* Direct3D9_;
	std::vector< DeviceInfo > devicesInfo_;
	uint32						modeIndex_;
	bool						windowed_;
	HWND						windowHandle_;
	uint32 deviceIndex_;
	IDirect3DVertexDeclaration9* vertexDeclaration_;
	uint32 fvf_;
	std::stack<Matrix> worldMatrices_;
	Matrix						projectionMatrix_;
	Matrix						viewMatrix_;
	Matrix						viewProjectionMatrix_;
	Matrix						viewMatrixInverse_;
	Camera						camera_;
	bool						changingMode_;
	uint32						backBufferWidthOverride_;
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
};

bool createRenderContex();

void destroyRenderContex();

RenderContext* getRenderContex();
