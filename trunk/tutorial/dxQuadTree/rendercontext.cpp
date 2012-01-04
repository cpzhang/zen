#include "rendercontext.h"


RenderContext::RenderContext()
{
	clear_();
}

D3DFORMAT RenderContext::getMatchingZBufferFormat( D3DFORMAT colourFormat, bool stencilWanted, bool & stencilAvailable )
{
	uint32 adapter = devicesInfo_[ deviceIndex_ ].adapterID_;

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

bool RenderContext::changeModePriv( uint32 modeIndex, bool windowed, bool testCooperative, uint32 backBufferWidthOverride )
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
		hr = Direct3DDevice9_->Reset( &presentParameters_ );

		lost_ = true;
		if( FAILED( hr ) && hr != D3DERR_DEVICELOST )
		{
			return false;
		}
		Sleep( 100 );
	}

	lost_ = false;

	UINT availTexMem = Direct3DDevice9_->GetAvailableTextureMem();

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
	return Direct3DDevice9_->Present(NULL, NULL, 0, NULL);
}

bool RenderContext::create()
{
	//
	Direct3D9_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == Direct3D9_)
	{
		return false;
	}
	for (uint32 adapterIndex = 0; adapterIndex < Direct3D9_->GetAdapterCount(); adapterIndex++)
	{
		DeviceInfo deviceInfo;
		deviceInfo.windowed_ = false;
		deviceInfo.adapterID_ = adapterIndex;
		if (D3D_OK == Direct3D9_->GetAdapterIdentifier( adapterIndex, 0, &deviceInfo.identifier_ ))
		{
			if (D3D_OK == Direct3D9_->GetDeviceCaps( adapterIndex, D3DDEVTYPE_HAL, &deviceInfo.caps_ ))
			{
				const uint32 VENDOR_ID_ATI = 0x1002;
				const uint32 VENDOR_ID_NVIDIA = 0x10de;

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
				uint32 nFormats = sizeof( formats ) / sizeof(D3DFORMAT);

				for (uint32 fi = 0; fi < nFormats; fi++)
				{
					D3DFORMAT format = formats[fi];
					// Iterate through display modes
					for (uint32 displayModeIndex = 0; displayModeIndex < Direct3D9_->GetAdapterModeCount( adapterIndex, format ); displayModeIndex++)
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

bool RenderContext::createDevice( HWND hWnd, uint32 deviceIndex, uint32 modeIndex, bool windowed,bool requireStencil,const Vector2 & windowedSize,bool forceRef )
{
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
	uint32 vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	mixedVertexProcessing_ = false;

	if (devicesInfo_[deviceIndex].caps_.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		if (uint16(devicesInfo_[deviceIndex].caps_.VertexShaderVersion) > 0x100 &&
			uint16(devicesInfo_[deviceIndex].caps_.PixelShaderVersion) > 0x100)
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
			devType = D3DDEVTYPE_REF;
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
	vsVersion_ = uint16( devicesInfo_[ deviceIndex_ ].caps_.VertexShaderVersion );
	psVersion_ = uint16( devicesInfo_[ deviceIndex_ ].caps_.PixelShaderVersion );
	maxSimTextures_ = uint16( devicesInfo_[ deviceIndex_ ].caps_.MaxSimultaneousTextures );
	maxAnisotropy_ = uint16( devicesInfo_[ deviceIndex_ ].caps_.MaxAnisotropy );

	mrtSupported_ = (psVersion_ >= 0x300) && 
		(devicesInfo_[ deviceIndex_ ].caps_.NumSimultaneousRTs > 1) &&
		(devicesInfo_[ deviceIndex_ ].caps_.PrimitiveMiscCaps & D3DPMISCCAPS_INDEPENDENTWRITEMASKS);

	setGammaCorrection();
	UINT availTexMem = Direct3DDevice9_->GetAvailableTextureMem();
	updateDeviceInfo();
	updateProjectionMatrix();
	initRenderStates();
	if (mixedVertexProcessing_)
	{
		Direct3DDevice9_->SetSoftwareVertexProcessing( TRUE );
	}

	return true;
}

uint32 RenderContext::setVertexDeclaration( IDirect3DVertexDeclaration9* pVD )
{
	if (pVD != vertexDeclaration_)
	{
		vertexDeclaration_ = pVD;
		fvf_ = 0;
		return Direct3DDevice9_->SetVertexDeclaration( pVD );
	}
	return D3D_OK;
}

uint32 RenderContext::setFVF( uint32 fvf )
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
	viewProjectionMatrix_.multiply(viewMatrix_, projectionMatrix_);
	viewMatrixInverse_.invert(viewMatrix_);
}

void RenderContext::updateProjectionMatrix()
{
	projectionMatrix_.perspectiveProjection( camera_.getFov(), camera_.getAspectRatio(), camera_.getNearPlane(), camera_.getFarPlane() );
}

const Matrix& RenderContext::getViewMatrix() const
{
	return viewMatrix_;
}

Matrix& RenderContext::getViewMatrix()
{
	return viewMatrix_;
}

const Matrix& RenderContext::getProjectionMatrix() const
{
	return projectionMatrix_;
}

Matrix& RenderContext::getProjectionMatrix()
{
	return projectionMatrix_;
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
		presentParameters_.BackBufferHeight = static_cast<uint32>( backBufferWidthOverride_ * ( getScreenHeight() / getScreenWidth() ) );
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

		for (uint32 i = 0; i < 256; i++)
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

bool createRenderContex()
{
	new RenderContext();
	RenderContext::getInstancePtr()->create();
	return true;
}

void destroyRenderContex()
{
	RenderContext::getInstancePtr()->destroy();
	delete RenderContext::getInstancePtr();
}

RenderContext* getRenderContex()
{
	return RenderContext::getInstancePtr();
}
