#include "rendercontext.h"
#include "math.h"
#include "misc/Logger.h"
RenderContext::RenderContext()
{
	clear_();
}

D3DFORMAT RenderContext::getMatchingZBufferFormat( D3DFORMAT colourFormat, bool stencilWanted, bool & stencilAvailable )
{
	u32 adapter = devicesInfo_[ deviceIndex_ ].adapterID_;

	if( colourFormat == D3DFMT_R8G8B8 || colourFormat == D3DFMT_X8R8G8B8 || colourFormat == D3DFMT_A8R8G8B8 )
	{
		if ( !stencilWanted )
		{
			if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) &&
				SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D32 ) ) )
				return D3DFMT_D32;
			if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) &&
				SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D24X8 ) ) )
				return D3DFMT_D24X8;
			if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) &&
				SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D24S8 ) ) )
				return D3DFMT_D24S8;
			if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) &&
				SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D24X4S4 ) ) )
				return D3DFMT_D24X4S4;
		}
		else
		{
			if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) &&
				SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D24S8 ) ) )
			{
				stencilAvailable = true;
				return D3DFMT_D24S8;
			}
			else
			{
				stencilAvailable = false;
			}
		}
	}

	if ( !stencilWanted )
	{
		if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) &&
			SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D16 ) ) )
			return D3DFMT_D16;
		if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) &&
			SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D15S1 ) ) )
			return D3DFMT_D15S1;
		if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) &&
			SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D24X8 ) ) )
			return D3DFMT_D24X8;
		if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) &&
			SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D32 ) ) )
			return D3DFMT_D32;
		if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) &&
			SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D24S8 ) ) )
			return D3DFMT_D24S8;
		if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) &&
			SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D24X4S4 ) ) )
			return D3DFMT_D24X4S4;
	}
	else
	{
		if( SUCCEEDED( Direct3D9_->CheckDeviceFormat( adapter, deviceType_, colourFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) &&
			SUCCEEDED( Direct3D9_->CheckDepthStencilMatch( adapter, deviceType_, colourFormat, colourFormat, D3DFMT_D24S8 ) ) )
		{
			stencilAvailable = true;
			return D3DFMT_D24S8;
		}
		else
		{
			stencilAvailable = false;
		}
	}
	return D3DFMT_D16;
}


bool RenderContext::changeModePriv( u32 modeIndex, bool windowed, bool testCooperative, u32 backBufferWidthOverride )
{
	if (NULL == Direct3DDevice9_)
	{
		return false;
	}
	if (testCooperative && 
		Direct3DDevice9_->TestCooperativeLevel() == D3DERR_DEVICELOST)
	{
		return false;
	}

	if (modeIndex != -1)
	{
		modeIndex_ = modeIndex;
		windowed_  = windowed;
	}
	backBufferWidthOverride_ = backBufferWidthOverride;

	// are we changing windowed status?
	fillPresentationParameters();

	// Some drivers have difficulty in restoring a device for a minimised 
	// window unless the width and height of the window are set in the 
	// presentation parameters.
	WINDOWPLACEMENT placement;
	GetWindowPlacement(windowHandle_, &placement);
	if (backBufferWidthOverride_ == 0 && placement.showCmd == SW_SHOWMINIMIZED)
	{
		RECT &rect = placement.rcNormalPosition;
		presentParameters_.BackBufferWidth  = abs(rect.right  - rect.left);
		presentParameters_.BackBufferHeight = abs(rect.bottom - rect.top );
	}

	HRESULT hr = D3DERR_DEVICELOST;
	while( hr == D3DERR_DEVICELOST )
	{
		/************************************************************************/
		/* Calling IDirect3DDevice9::Reset causes all texture memory surfaces to be lost, 
		managed textures to be flushed from video memory, and all state information to be lost. 
		Before calling the IDirect3DDevice9::Reset method for a device, 
		an application should release any explicit render targets, depth stencil surfaces, 
		additional swap chains, state blocks, and D3DPOOL_DEFAULT resources associated with the device.                                                                     */
		/************************************************************************/
		hr = Direct3DDevice9_->Reset( &presentParameters_ );

		lost_ = true;
		if( FAILED( hr ) && hr != D3DERR_DEVICELOST )
		{
			return false;
		}
		Sleep( 100 );
	}

	lost_ = false;

	u32 availTexMem = Direct3DDevice9_->GetAvailableTextureMem();

	updateDeviceInfo();
	updateProjectionMatrix();
	if (mixedVertexProcessing_)
	{
		Direct3DDevice9_->SetSoftwareVertexProcessing( TRUE );
	}

	// save windowed mode size
	if (windowed_)
	{
		RECT clientRect;
		GetClientRect( windowHandle_, &clientRect );
		windowedSize_ = Vector2(
			float(clientRect.right - clientRect.left),
			float(clientRect.bottom - clientRect.top));
	}

	return true;
}

void RenderContext::clear_()
{
	initialized_ = false;
	fullScreenAspectRatio_ = 4.f/3.f;
	tripleBuffering_ = true;
	stencilWanted_ = false;
	stencilAvailable_ = false;
	gammaCorrection_ =  1.f;
	screenCopySurface_ = NULL;
	waitForVBL_ = true;
	Direct3D9_ = NULL;
	Direct3DDevice9_ = NULL;
	deviceIndex_ = 0;
	fvf_ = 0xff81ff81;
	vertexDeclaration_ = NULL;
	changingMode_ = false;
	modeIndex_ = 0;
	windowed_ = true;
	backBufferWidthOverride_ = 0;
	halfScreenWidth_ = 320.f;
	halfScreenHeight_ = 240.f;
	currentRenderTargetKey_ = 0;
	currentRenderTarget_ = NULL;
	implicitRenderTargetKey_ = 0;
}

IDirect3DSurface9* RenderContext::getScreenCopy()
{
	IDirect3DSurface9* ret = NULL;
	if (screenCopySurface_)
	{
		IDirect3DSurface9* backBuffer;
		if( SUCCEEDED( Direct3DDevice9_->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer ) ) )
		{
			if (SUCCEEDED( D3DXLoadSurfaceFromSurface( screenCopySurface_, NULL, NULL,
				backBuffer, NULL, NULL, D3DX_FILTER_NONE, 0 ) ) )
			{
				ret = screenCopySurface_;
			}
		}
	}
	return ret;
}

RenderContext::~RenderContext()
{
	clear_();
}

HRESULT RenderContext::beginScene()
{
	return Direct3DDevice9_->BeginScene();
}

HRESULT RenderContext::endScene()
{
	return Direct3DDevice9_->EndScene();
}

void RenderContext::setWaitForVBL( bool wait )
{
	waitForVBL_ = wait;
}

IDirect3DDevice9* RenderContext::getDxDevice() const
{
	return Direct3DDevice9_;
}
IDirect3D9* RenderContext::getDxD3D9() const
{
	return Direct3D9_;
}

void RenderContext::clearBindings()
{
	Direct3DDevice9_->SetIndices(NULL);
	for( int i = 0; ( i < devicesInfo_[ deviceIndex_ ].caps_.MaxStreams ) || ( i < 1 ); i++ )
	{
		Direct3DDevice9_->SetStreamSource( i, NULL, 0, 0 );
	}

	// release all textures
	for( int i = 0; i < devicesInfo_[ deviceIndex_ ].caps_.MaxSimultaneousTextures; i++ )
	{
		Direct3DDevice9_->SetTexture( i, NULL );
	}

	//release shaders
	Direct3DDevice9_->SetPixelShader( NULL );
	Direct3DDevice9_->SetVertexShader( NULL );
}

HRESULT RenderContext::present()
{
	clearBindings();
	//return Direct3DDevice9_->Present(NULL, NULL, 0, NULL);
	if (currentRenderTarget_ == NULL)
	{
		return E_FAIL;
	}
	return currentRenderTarget_->present();
}

bool RenderContext::create()
{
	//
	Direct3D9_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == Direct3D9_)
	{
		return false;
	}
	for (u32 adapterIndex = 0; adapterIndex < Direct3D9_->GetAdapterCount(); adapterIndex++)
	{
		DeviceInfo deviceInfo;
		deviceInfo.windowed_ = false;
		deviceInfo.adapterID_ = adapterIndex;
		if (D3D_OK == Direct3D9_->GetAdapterIdentifier( adapterIndex, 0, &deviceInfo.identifier_ ))
		{
			if (D3D_OK == Direct3D9_->GetDeviceCaps( adapterIndex, D3DDEVTYPE_HAL, &deviceInfo.caps_ ))
			{
				const u32 VENDOR_ID_ATI = 0x1002;
				const u32 VENDOR_ID_NVIDIA = 0x10de;

				deviceInfo.compatibilityFlags_ = 0;
				if (strstr(deviceInfo.identifier_.Description, "GeForce4 MX") != NULL)
				{
					deviceInfo.compatibilityFlags_ |= eCompatibilityFlag_NOOVERWRITE;
				}

				if (deviceInfo.identifier_.VendorId == VENDOR_ID_NVIDIA)
				{
					deviceInfo.compatibilityFlags_ |= eCompatibilityFlag_NVIDIA;
				}

				if (deviceInfo.identifier_.VendorId == VENDOR_ID_ATI)
				{
					deviceInfo.compatibilityFlags_ |= eCompatibilityFlag_ATI;
				}

				D3DFORMAT formats[] = { D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8, D3DFMT_A2B10G10R10, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5, D3DFMT_R5G6B5 };
				u32 nFormats = sizeof( formats ) / sizeof(D3DFORMAT);

				for (u32 fi = 0; fi < nFormats; fi++)
				{
					D3DFORMAT format = formats[fi];
					// Iterate through display modes
					for (u32 displayModeIndex = 0; displayModeIndex < Direct3D9_->GetAdapterModeCount( adapterIndex, format ); displayModeIndex++)
					{
						D3DDISPLAYMODE mode;
						if (D3D_OK == Direct3D9_->EnumAdapterModes( adapterIndex, format, displayModeIndex, &mode ))
						{
							mode.RefreshRate = 0;
							bool found = false;
							for (std::vector<D3DDISPLAYMODE>::iterator it = deviceInfo.displayModes_.begin(); it != deviceInfo.displayModes_.end(); ++it)
							{
								if (it->Format == mode.Format && it->Height == mode.Height && it->RefreshRate == mode.RefreshRate && it->Width == mode.Width)
								{
									found = true;
									break;
								}
							}
							if(!found)
							{
								if( mode.Width >= 640 && mode.Height >= 480 )
									deviceInfo.displayModes_.push_back( mode );
							}
						}
					}
				}
				deviceInfo.windowed_ = true;
			}
			if (deviceInfo.displayModes_.size() != 0 || deviceInfo.windowed_ == true)
			{

				if( deviceInfo.windowed_ )
				{
					Direct3D9_->GetAdapterDisplayMode( deviceInfo.adapterID_, &deviceInfo.windowedDisplayMode_ );
				}

				//std::sort( deviceInfo.displayModes_.begin(), deviceInfo.displayModes_.end() );
				devicesInfo_.push_back( deviceInfo );
			}
		}
	}
	if (devicesInfo_.empty())
	{
		return false;
	}
	return true;
}

void RenderContext::destroy()
{
	//
	releaseRenderTarget(implicitRenderTargetKey_);
	//
	for (unsigned char i = eVertexDeclarationType_Null; i != eVertexDeclarationType_Size; ++i)
	{
		if (vertexDeclarations_[i])
		{
			vertexDeclarations_[i]->Release();
			vertexDeclarations_[i] = 0;
		}
	}

	if (Direct3DDevice9_)
	{
		Direct3DDevice9_->Release();
	}
	if (Direct3D9_)
	{
		Direct3D9_->Release();
	}
	clear_();
}

bool RenderContext::createDevice( HWND hWnd, u32 deviceIndex, u32 modeIndex, bool windowed,bool requireStencil,const Vector2 & windowedSize,bool forceRef )
{
	initialized_ = true;
	if (Direct3DDevice9_)
	{
		return false;
	}
	if (deviceIndex >= devicesInfo_.size())
	{
		return false;
	}
	//
	deviceIndex_ = deviceIndex;
	modeIndex_ = modeIndex;
	windowed_ = windowed;
	windowedSize_ = windowedSize;
	windowHandle_ = hWnd;
	stencilWanted_ = requireStencil;
	//
	windowedStyle_ = GetWindowLong( windowHandle_, GWL_STYLE );
	//
	if (forceRef)
	{
		deviceType_ = D3DDEVTYPE_REF;
	}
	else
	{
		deviceType_ = D3DDEVTYPE_HAL;
	}
	fillPresentationParameters();
	//
	if( !windowed_ )
	{
		GetWindowRect( windowHandle_, &windowedRect_ );
	}
	u32 vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	mixedVertexProcessing_ = false;

	if (devicesInfo_[deviceIndex].caps_.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		if (u16(devicesInfo_[deviceIndex].caps_.VertexShaderVersion) > 0x100 &&
			u16(devicesInfo_[deviceIndex].caps_.PixelShaderVersion) > 0x100)
		{
			vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		else
		{
			vertexProcessing = D3DCREATE_MIXED_VERTEXPROCESSING;
			mixedVertexProcessing_ = true;
		}
	}

	D3DDEVTYPE devType = deviceType_;
	if (forceRef)
		devType = D3DDEVTYPE_NULLREF;
	else 
	{
		// Force reference if using NVIDIA NVPerfHUD.
		std::string description 
			= devicesInfo_[ deviceIndex_ ].identifier_.Description;

		if ( description.find("PerfHUD") != std::string::npos )
		{
			devType = D3DDEVTYPE_REF;
		}
	}

	int retries = 5;
	HRESULT hr = D3D_OK;
	while ( retries-- )
	{
		hr = Direct3D9_->CreateDevice( devicesInfo_[ deviceIndex_ ].adapterID_, devType, 
			windowHandle_,
			vertexProcessing | D3DCREATE_DISABLE_DRIVER_MANAGEMENT | 
			D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE, 
			&presentParameters_, &Direct3DDevice9_);

		if( SUCCEEDED( hr ) )
			break;
		Sleep( 2000 );
	}

	if ( !SUCCEEDED( hr ) )
	{
		return false;
	}

	// succeeded
	vsVersion_ = u16( devicesInfo_[ deviceIndex_ ].caps_.VertexShaderVersion );
	psVersion_ = u16( devicesInfo_[ deviceIndex_ ].caps_.PixelShaderVersion );
	maxSimTextures_ = u16( devicesInfo_[ deviceIndex_ ].caps_.MaxSimultaneousTextures );
	maxAnisotropy_ = u16( devicesInfo_[ deviceIndex_ ].caps_.MaxAnisotropy );

	mrtSupported_ = (psVersion_ >= 0x300) && 
		(devicesInfo_[ deviceIndex_ ].caps_.NumSimultaneousRTs > 1) &&
		(devicesInfo_[ deviceIndex_ ].caps_.PrimitiveMiscCaps & D3DPMISCCAPS_INDEPENDENTWRITEMASKS);

	setGammaCorrection();
	u32 availTexMem = Direct3DDevice9_->GetAvailableTextureMem();
	updateDeviceInfo();
	//updateProjectionMatrix();
	initRenderStates();
	if (mixedVertexProcessing_)
	{
		Direct3DDevice9_->SetSoftwareVertexProcessing( TRUE );
	}
	initVertexDeclarations_();
	//
	implicitRenderTargetKey_ = createRenderTarget(eRenderTarget_Implicit);
	setCurrentRenderTarget(implicitRenderTargetKey_);
	//currentRenderTarget_ = getRenderTarget(implicitRenderTargetKey_);
	//currentRenderTargetKey_ = implicitRenderTargetKey_;
	//currentRenderTarget_->apply();
	//
	return true;
}

bool RenderContext::setVertexDeclaration( eVertexDeclarationType e )
{
	if (e >= eVertexDeclarationType_Null || e < eVertexDeclarationType_Size)
	{
		Direct3DDevice9_->SetVertexDeclaration(vertexDeclarations_[e]);
		return true;
	}
	return false;
}

u32 RenderContext::setFVF( u32 fvf )
{
	if (fvf != fvf_)
	{
		fvf_ = fvf;
		vertexDeclaration_ = NULL;
		return Direct3DDevice9_->SetFVF( fvf_ );
	}

	return D3D_OK;
}

void RenderContext::resetWorldMatricesStack()
{
	while(!worldMatrices_.empty())
	{
		worldMatrices_.pop();
	}
	Matrix m;
	m.setIdentity();
	worldMatrices_.push(m);
}

void RenderContext::updateViewProjectionMatrix()
{
	currentRenderTarget_->viewProjectionMatrix_.multiply(currentRenderTarget_->viewMatrix_, currentRenderTarget_->projectionMatrix_);
	currentRenderTarget_->viewMatrixInverse_.invert(currentRenderTarget_->viewMatrix_);
}

void RenderContext::updateProjectionMatrix()
{
	if( !windowed_ )
	{
		currentRenderTarget_->camera_.setAspectRatio( fullScreenAspectRatio_ );
	}
	else
	{
		currentRenderTarget_->camera_.setAspectRatio( float( backBufferDesc_.Width ) / float( backBufferDesc_.Height ) );
	}
	if (currentRenderTarget_->camera_.isOrtho())
	{
		currentRenderTarget_->projectionMatrix_.orthogonalProjection(currentRenderTarget_->camera_.getWidth(),
			currentRenderTarget_->camera_.getHeight(),
			currentRenderTarget_->camera_.getNearPlane(),
			currentRenderTarget_->camera_.getFarPlane());
	}
	else
	{
		currentRenderTarget_->projectionMatrix_.perspectiveProjection( currentRenderTarget_->camera_.getFov(), 
			currentRenderTarget_->camera_.getAspectRatio(),
			currentRenderTarget_->camera_.getNearPlane(), currentRenderTarget_->camera_.getFarPlane() );
	}
}

const Matrix& RenderContext::getViewMatrix() const
{
	return currentRenderTarget_->viewMatrix_;
}

Matrix& RenderContext::getViewMatrix()
{
	return currentRenderTarget_->viewMatrix_;
}

const Matrix& RenderContext::getProjectionMatrix() const
{
	return currentRenderTarget_->projectionMatrix_;
}

Matrix& RenderContext::getProjectionMatrix()
{
	return currentRenderTarget_->projectionMatrix_;
}

bool RenderContext::checkDevice( bool& reset )
{
	bool result = false;
	reset = false;
	HRESULT hr = Direct3DDevice9_->TestCooperativeLevel();
	switch (hr)
	{
	case D3D_OK:
		result = true;
		break;
	case D3DERR_DEVICELOST:
		result = false;
		break;
	case D3DERR_DEVICENOTRESET:
		{
			result = resetDevice();
			reset = result;
			break;
		}
	case D3DERR_DRIVERINTERNALERROR:
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		break;
	default:
		break;
	}
	return result;
}

bool RenderContext::resetDevice()
{
	if (changingMode_)
	{
		return false;
	}

	releaseUnmanaged();

	bool result = changeModePriv(modeIndex_, windowed_, true, 0);
	if (result)
	{
		createUnmanaged();
	}

	return result;
}

void RenderContext::releaseUnmanaged()
{

}

void RenderContext::createUnmanaged()
{

}

float RenderContext::getScreenWidth() const
{
	return halfScreenWidth_ + halfScreenWidth_;
}

float RenderContext::getScreenHeight() const
{
	return halfScreenHeight_ + halfScreenHeight_;
}

void RenderContext::setScreenWidth( int width )
{
	halfScreenWidth_ = float(width) * 0.5f;
}

void RenderContext::setScreenHeight( int height )
{
	halfScreenHeight_ = float(height) * 0.5f;
}

void RenderContext::updateDeviceInfo()
{
	IDirect3DSurface9*  pBackBuffer;
	Direct3DDevice9_->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	pBackBuffer->GetDesc( &backBufferDesc_ );
	pBackBuffer->Release();

	halfScreenWidth_ = float( backBufferDesc_.Width ) * 0.5f;
	halfScreenHeight_ = float( backBufferDesc_.Height ) * 0.5f;

	IDirect3DSurface9* surf = NULL;

	D3DDISPLAYMODE smode;
	Direct3D9_->GetAdapterDisplayMode( devicesInfo_[ deviceIndex_ ].adapterID_, &smode );

	HRESULT hr;
	if ( SUCCEEDED( hr = Direct3DDevice9_->CreateOffscreenPlainSurface( backBufferDesc_.Width, backBufferDesc_.Height, 
		D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surf, NULL ) ) )
	{
		screenCopySurface_ = surf;
		surf->Release();
	}
	else
	{
		screenCopySurface_ = NULL;
	}
}

void RenderContext::fillPresentationParameters( void )
{
	ZeroMemory( &presentParameters_, sizeof( presentParameters_ ) );

	if( windowed_ )
	{
		presentParameters_.BackBufferWidth = backBufferWidthOverride_;
		presentParameters_.BackBufferHeight = static_cast<u32>( backBufferWidthOverride_ * ( getScreenHeight() / getScreenWidth() ) );
		presentParameters_.BackBufferFormat = devicesInfo_[ deviceIndex_ ].windowedDisplayMode_.Format;
		presentParameters_.PresentationInterval = waitForVBL_ ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		D3DDISPLAYMODE mode = devicesInfo_[ deviceIndex_ ].displayModes_[ modeIndex_ ];
		presentParameters_.BackBufferWidth = mode.Width;
		presentParameters_.BackBufferHeight = mode.Height;
		presentParameters_.BackBufferFormat = mode.Format;
		presentParameters_.PresentationInterval = waitForVBL_ ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	presentParameters_.Windowed = windowed_;
	presentParameters_.BackBufferCount = tripleBuffering_ ? 2 : 1;
	presentParameters_.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters_.MultiSampleType = D3DMULTISAMPLE_NONE;
	presentParameters_.EnableAutoDepthStencil = TRUE;
	if (!lost_)
		presentParameters_.AutoDepthStencilFormat = 
		getMatchingZBufferFormat( presentParameters_.BackBufferFormat, 
		stencilWanted_, stencilAvailable_ );
	else if (stencilAvailable_)
		presentParameters_.AutoDepthStencilFormat = D3DFMT_D24S8;
	else
		presentParameters_.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters_.Flags = 0;
	presentParameters_.hDeviceWindow = windowHandle_;
}

void RenderContext::setGammaCorrection()
{
	gammaCorrection_ = max( 0.5f, min( 6.f, gammaCorrection_ ) );
	if (Direct3DDevice9_)
	{
		D3DGAMMARAMP ramp;

		for (u32 i = 0; i < 256; i++)
		{
			float f = /*1.f -*/ (float(i) / 255.f);
			ramp.red[i] = ramp.green[i] = ramp.blue[i] = WORD( ( /*1.f -*/ powf( f, 1.f / gammaCorrection_ ) ) * 65535.f );
		}

		Direct3DDevice9_->SetGammaRamp( 0, D3DSGR_CALIBRATE, &ramp );
	}
}

void RenderContext::initRenderStates()
{
	fvf_ = 0xff81ff81;
	vertexDeclaration_ = NULL;
	if (vsVersion_==0 && psVersion_ == 0)
		Direct3DDevice9_->SetRenderState( D3DRS_CLIPPING, TRUE );
	else
		Direct3DDevice9_->SetRenderState( D3DRS_CLIPPING, FALSE );
}

bool RenderContext::isStencilAvailable() const
{
	return stencilAvailable_;
}

bool RenderContext::checkDeviceYield( bool& reset )
{
	bool result = true;
	while (!(result = checkDevice(reset)))
	{
		MSG msg;
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_QUIT) ||
				(msg.message == WM_KEYDOWN && 
				msg.wParam == VK_F4 && ::GetKeyState(VK_MENU)))
			{
				result = false;
				break;
			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		::Sleep(50);
	}
	return result;
}

bool RenderContext::getTripleBuffering() const
{
	return tripleBuffering_;
}

void RenderContext::setTripleBuffering( bool enabled )
{
	tripleBuffering_ = enabled;
}

bool RenderContext::isWindowed() const
{
	return windowed_;
}

const DeviceInfo& RenderContext::getDeviceInfo( u32 i ) const
{
	return devicesInfo_[ i ];
}

u32 RenderContext::getDevicesNumber() const
{
	return devicesInfo_.size();
}

void RenderContext::setFullScreenAspectRatio( float ratio )
{
	fullScreenAspectRatio_ = ratio;
}

bool RenderContext::isMixedVertexProcessing() const
{
	return mixedVertexProcessing_;
}

bool RenderContext::changeMode( u32 modeIndex, bool windowed, bool testCooperative, u32 backBufferWidthOverride )
{
	if (changingMode_)
	{
		return false;
	}
	changingMode_ = true;
	bool goingFullScreen = windowed != windowed_ && windowed == false;
	bool goingWindowMode = windowed != windowed_ && windowed == true;

	if (goingFullScreen)
	{
		// from windowed to fullscreen
		// save windows geometry info
		GetWindowRect( windowHandle_, &windowedRect_ );
		// save the windowed style to restore it later, and set it to
		// WS_POPUP so the fullscreen window doesn't have a caption.
		windowedStyle_ = GetWindowLong( windowHandle_, GWL_STYLE );
		SetWindowLong( windowHandle_, GWL_STYLE, WS_POPUP );
	}
	else if (goingWindowMode)
	{
		// restore the window's windowed style
		SetWindowLong( windowHandle_, GWL_STYLE, windowedStyle_ );
	}

	// switch 
	releaseUnmanaged();
	bool result = changeModePriv(modeIndex, windowed, testCooperative, backBufferWidthOverride);
	if (result && goingWindowMode)
	{
		// If going back to windowed mode, resize it.
		SetWindowPos( windowHandle_, HWND_NOTOPMOST,
			windowedRect_.left, windowedRect_.top,
			( windowedRect_.right - windowedRect_.left ),
			( windowedRect_.bottom - windowedRect_.top ),
			SWP_SHOWWINDOW );

		// finally, adjust backbuffer size to window
		// equivalent to calling resetDevice, but we
		// don't want to call createAllUnmanaged and 
		// createAllUnmanaged again.
		result = changeModePriv(modeIndex_, windowed_, true, 0);
	}
	if (result)
	{
		createUnmanaged();
	}
	changingMode_ = false;
	return result;
}

bool RenderContext::isInitialized()
{
	return initialized_;
}

Matrix& RenderContext::getViewProjectionMatrix()
{
	return currentRenderTarget_->viewProjectionMatrix_;
}

u32 RenderContext::drawIndexedPrimitive( D3DPRIMITIVETYPE type, INT baseVertexIndex, u32 minIndex, u32 numVertices, u32 startIndex, u32 primitiveCount )
{
	return Direct3DDevice9_->DrawIndexedPrimitive( type, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount );
}

void RenderContext::initVertexDeclarations_()
{
	//
	static const u32 scMaxElementsNumber = 8;
	D3DVERTEXELEMENT9 ve[eVertexDeclarationType_Size][scMaxElementsNumber] = 
	{
		//	eVertexDeclarationType_Null
		{
		D3DDECL_END()
		},

		//	eVertexDeclarationType_Position
		{
		{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		D3DDECL_END()
		},
		//	eVertexDeclarationType_PositionBoneWeight
		{
		{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{0,		20,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,	0},
		{0,		36,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,	0},
		D3DDECL_END()
		},
		//	eVertexDeclarationType_PositionColor
		{
		{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{0,		12,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
		D3DDECL_END()
		},

		//	eVertexDeclarationType_PositionTexture
		{
		{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{0,		12,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
		D3DDECL_END()
		},

		//	eVertexDeclarationType_PositionBlend
		{
		{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{0,		12,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,	0},
		D3DDECL_END()
		},

		//	eVertexDeclarationType_PositionBlendNormal
		{
		{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{0,		12,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,	0},
		{ 0,	28, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
		D3DDECL_END()
		},

		//	eVertexDeclarationType_PositionTextureBoneWeight
		{
		{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{0,		12,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
		{0,		20,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,	0},
		{0,		36,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,	0},
		D3DDECL_END()
		},
		//	eVertexDeclarationType_PositionTextureBoneWeightColor
		{
		{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{0,		12,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
		{0,		20,	D3DDECLTYPE_UBYTE4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,	0},
		{0,		24,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,	0},
		{0,		40,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
		D3DDECL_END()
		},
		//	eVertexDeclarationType_PositionTextureBoneWeightColorNormal
		{
		{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{0,		12, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
		{0,		24,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
		{0,		32,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
		{0,		36,	D3DDECLTYPE_UBYTE4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,	0},
		{0,		40,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,	0},
		D3DDECL_END()
		},
		//	eVertexDeclarationType_PositionTextureNormal
		{
		{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{ 0,	12, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
		{ 0,	20, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
		D3DDECL_END()
		},

		//	eVertexDeclarationType_PositionColorTexture
		{
		{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{ 0,	12, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
		{ 0,	16, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
		D3DDECL_END()
		},
		//	eVertexDeclarationType_PositionTTexture
		{
		{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT,	0},
		{ 0,	12, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
		D3DDECL_END()
		},

		// eVertexDeclarationType_Matrix
		{
		{ 1,	0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1},
		{ 1,	16,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2},
		{ 1,	32,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3},
		{ 1,	48,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4},
		D3DDECL_END()
		},
		//	eVertexDeclarationType_PositionTextureNormalMatrix
		{
		{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{ 0,	12, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
		{ 0,	20, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
		{ 1,	0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1},
		{ 1,	16,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2},
		{ 1,	32,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3},
		{ 1,	48,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4},
		D3DDECL_END()
		},
		//	eVertexDeclarationType_PositionTextureNormalIndex
		{
		{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
		{ 0,	12, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
		{ 0,	20, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
		{ 0,	32,	D3DDECLTYPE_FLOAT1,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1},
		D3DDECL_END()
		},
	};

	//
	for (unsigned char i = eVertexDeclarationType_Null; i != eVertexDeclarationType_Size; ++i)
	{
		Direct3DDevice9_->CreateVertexDeclaration(ve[i], vertexDeclarations_ + i);
	}
}

void RenderContext::setViewMatrix( const Matrix& m )
{
	currentRenderTarget_->viewMatrix_ = m;
	updateViewProjectionMatrix();
}

void RenderContext::setCamera( Camera& c )
{
	currentRenderTarget_->camera_ = c;
	updateProjectionMatrix();
}

Camera RenderContext::getCamera()
{
	return currentRenderTarget_->camera_;
}

Ray RenderContext::getPickingRay()
{
	//
	POINT ptCursor;
	GetCursorPos( &ptCursor );
	::ScreenToClient( windowHandle_, &ptCursor );
	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 v;
	v.x = ( ( ( 2.0f * ptCursor.x ) / getRenderContex()->getScreenWidth() ) - 1 ) / currentRenderTarget_->projectionMatrix_._11;
	v.y = -( ( ( 2.0f * ptCursor.y ) / getRenderContex()->getScreenHeight() ) - 1 ) / currentRenderTarget_->projectionMatrix_._22;
	v.z = 1.0f;

	// Get the inverse view matrix
	const Matrix matView = getRenderContex()->getViewMatrix();
	//const Matrix matWorld = Matrix::Identity;
	//Matrix mWorldView = matWorld;
	//mWorldView.postMultiply(matView);
	Matrix m;
	m.invert(matView);

	// Transform the screen space pick ray into 3D space
	Ray r;
	r.direction_.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	r.direction_.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	r.direction_.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	r.origion_.x = m._41;
	r.origion_.y = m._42;
	r.origion_.z = m._43;
	return r;
}

u32 RenderContext::drawIndexedPrimitiveUP( D3DPRIMITIVETYPE primitiveType,u32 minIndex, u32 numVertices, u32 primitiveCount, CONST void* pIndexData, D3DFORMAT indexDataFormat, CONST void* pVertexStreamZeroData, u32 vertexStreamZeroStride )
{
	return Direct3DDevice9_->DrawIndexedPrimitiveUP( primitiveType, minIndex,numVertices, primitiveCount, pIndexData, indexDataFormat, pVertexStreamZeroData, vertexStreamZeroStride );
}

u32 RenderContext::drawPrimitiveUP( D3DPRIMITIVETYPE primitiveType, u32 primitiveCount, CONST void* pVertexStreamZeroData, u32 vertexStreamZeroStride )
{
	return Direct3DDevice9_->DrawPrimitiveUP( primitiveType, primitiveCount,pVertexStreamZeroData, vertexStreamZeroStride );
}

HRESULT RenderContext::SetTransform( D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix )
{
	return Direct3DDevice9_->SetTransform(State, pMatrix);
}

HRESULT RenderContext::applyViewMatrix()
{
	return Direct3DDevice9_->SetTransform(D3DTS_VIEW, &currentRenderTarget_->viewMatrix_);
}

HRESULT RenderContext::applyProjectionMatrix()
{
	return Direct3DDevice9_->SetTransform(D3DTS_PROJECTION, &currentRenderTarget_->projectionMatrix_);
}

HRESULT RenderContext::LightEnable( DWORD LightIndex, BOOL bEnable )
{
	return Direct3DDevice9_->LightEnable(LightIndex, bEnable);
}

u32 RenderContext::createRenderTarget( eRenderTarget e)
{
	static u32 tCounter = 0;
	u32 n = 0;
	IRenderTarget* t = NULL;
	switch (e)
	{
	case eRenderTarget_Implicit:
		t = new ImplicitRenderTarget;
		break;
	case eRenderTarget_Additional:
		t = new AdditionalRenderTarget;
		break;
	case eRenderTarget_Texture:
		t = new TextureRenderTarget;
		break;
	}
	if (NULL != t)
	{
		n = ++tCounter;
		renderTargets_[n] = t;
	}
	return n;
}

void RenderContext::setCurrentRenderTarget( u32 k )
{
	currentRenderTargetKey_ = k;
	currentRenderTarget_ = getRenderTarget(currentRenderTargetKey_);
	if (currentRenderTarget_)
	{
		currentRenderTarget_->apply();
	}
}

/**
 * Takes a screenshot
 *
 * @param fileExt - the extension to save the screenshot as, this can be "bmp", "jpg", "tga", "png" or "dds".
 *
 * @param fileName - the root name of the file to save out
 *
 * @param autoNumber - is this is true then try to postpend a unique identifying number to the shot name(e.g. shot_666.bmp)
 */
const std::string RenderContext::screenShot( const std::string& fileExt /*= "bmp"*/, const std::string& fileName /*= "shot"*/, bool autoNumber /*= true*/ )
{
	static std::map< std::string, D3DXIMAGE_FILEFORMAT > format;
	if (format.size() == 0)
	{
		format["dds"] = D3DXIFF_DDS;
		format["bmp"] = D3DXIFF_BMP;
		format["jpg"] = D3DXIFF_JPG;
		format["tga"] = D3DXIFF_TGA;
		format["png"] = D3DXIFF_PNG;
	}

	//Convert uppercase to lowercase
	std::string ext = fileExt;
	std::transform( ext.begin(), ext.end(), ext.begin(), tolower );

	//It the extension specified cannot be found then use "bmp"
	if (format.find( ext ) == format.end())
	{
		ext = "bmp";
	}

	char findName[256];
	std::string name;

	if (autoNumber)
	{	
		static u32 sequence = 1;

		// go through filenames until we find one that has not been created yet.
		bool foundEmptyFile = false;
		while( !foundEmptyFile )
		{
			// Create the filename
			sprintf( findName, "%s_%03d.%s", fileName.c_str(), sequence, ext.c_str() );
			FILE* f = fopen( findName, "rb" );

			// is there such a file?
			if( !f )
			{
				// nope, we have a winner.
				foundEmptyFile = true;
				name = findName;
			}
			else
			{
				// try the next file.
				sequence++;
				fclose( f );
			}
		}
	}
	else
	{
		sprintf( findName, "%s.%s", fileName.c_str(), ext.c_str() );
		name = findName;
	}

	IDirect3DSurface9* backBuffer = NULL;
	if( SUCCEEDED( currentRenderTarget_->getBackBuffer( &backBuffer ) ) )
	{
		if (SUCCEEDED( D3DXSaveSurfaceToFile( name.c_str(), format[ext], backBuffer, NULL, NULL) ) )
		{
			//INFO_MSG( "Moo::RenderContext::screenShot - saved image %s\n", name.c_str() );
		}
		else
		{
			D3DSURFACE_DESC backBufferDesc;
			IDirect3DSurface9* systemMemorySurface;
			backBuffer->GetDesc( &backBufferDesc );
			if( SUCCEEDED( Direct3DDevice9_->CreateOffscreenPlainSurface(	backBufferDesc.Width,
				backBufferDesc.Height,
				backBufferDesc.Format,
				D3DPOOL_SYSTEMMEM,
				&systemMemorySurface,
				NULL ) ) )
			{
				if( SUCCEEDED( Direct3DDevice9_->GetRenderTargetData( backBuffer, systemMemorySurface ) ) )
				{
					if( SUCCEEDED( D3DXSaveSurfaceToFile( name.c_str(), format[ext], systemMemorySurface, NULL, NULL) ) )
					{
						//INFO_MSG( "Moo::RenderContext::screenShot - saved image %s\n", name.c_str() );
					}
				}

				systemMemorySurface->Release();
			}
		}
	}
	else
	{
		Error( "Moo::RenderContext::screenShot - unable to get backbuffer surface\n" );
	}

	return name;
}

ApiRender_ bool createRenderContex()
{
	new RenderContext();
	RenderContext::getInstancePtr()->create();
	return true;
}

ApiRender_ void destroyRenderContex()
{
	RenderContext::getInstancePtr()->destroy();
	delete RenderContext::getInstancePtr();
}

ApiRender_ RenderContext* getRenderContex()
{
	return RenderContext::getInstancePtr();
}

IRenderTarget::IRenderTarget() :camera_( 0.25, 500, DEG_TO_RAD( 60 ), 4.f/3.f )
{

}
