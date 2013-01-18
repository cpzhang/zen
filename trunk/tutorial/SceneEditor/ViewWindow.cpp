#include "ViewWindow.h"
#include "render/rendercontext.h"
#include "render/TextureManager.h"
#include "scene/SceneManager.h"
#include "render/FxManager.h"
#include "scene/Terrain.h"
#include "Global.h"
#include "render/Colour.h"
#include "render/TextureManager.h"
#include "render/Texture.h"
ViewWindow::ViewWindow()
{

}

ViewWindow::~ViewWindow()
{

}
void createBeforeD3DDevice()
{

}
void createAfterD3DDevice()
{
	
}
LRESULT ViewWindow::onKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT ViewWindow::onCreate( UINT, WPARAM, LPARAM, BOOL& )
{
	//
	createRenderContex();
	//
	return 1;
}

LRESULT ViewWindow::onDestroy( UINT, WPARAM, LPARAM, BOOL& )
{
	//
	
	return 1;
}

// LRESULT ViewWindow::onPaint( UINT, WPARAM, LPARAM, BOOL& b )
// {
// 	b = FALSE;//������䣬�޷�����onIdle()
// 	return 1;
// }

LRESULT ViewWindow::onMouseWheel( UINT, WPARAM wParam, LPARAM, BOOL& b )
{
	return 1;
}

LRESULT ViewWindow::onMouseMove( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	//��ý���
	SetFocus(); 
	return 1;
}

LRESULT ViewWindow::onMouseLeftButtonUp( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	return 1;
}

LRESULT ViewWindow::onSize( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	if (getRenderContex())
	{
		getRenderContex()->resetDevice();
	}
	return 1;
}

void ViewWindow::run()
{
}
void renderAxis(u32 scColor, Vector3 axis)
{
	//axis
	{
		static const float scLength = 10.0f;
		//x
		{
			sVDT_PositionColor ps[2];
			ps[0].color_ = scColor;
			ps[0].position_ = Vector3::Zero;//-scLength * axis;
			ps[1].color_ = scColor;
			ps[1].position_ = scLength * axis;
			getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 1, ps, sVDT_PositionColor::getSize());
		}
	}
	//arrow
	if(0){
		static const float scRadius = 500.0f;
		static const float scLength = 50.0f;
		static const float scDistance = 10.0f;
		//x
		{
			sVDT_PositionColor ps[5];

			ps[0].color_ = scColor;
			ps[0].position_ = (scLength + scRadius) * axis;
			ps[1].color_ = scColor;
			//
			{
				ps[1].position_ = ps[0].position_ + Vector3(scRadius, scDistance, scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 2, ps, sVDT_PositionColor::getSize());
			}
			//
			{
				ps[1].position_ = Vector3(scRadius, -scDistance, scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 2, ps, sVDT_PositionColor::getSize());
			}
			//
			{
				ps[1].position_ = Vector3(scRadius, scDistance, -scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 2, ps, sVDT_PositionColor::getSize());
			}
			//
			{
				ps[1].position_ = Vector3(scRadius, -scDistance, -scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 2, ps, sVDT_PositionColor::getSize());
			}
			//
			{
				ps[4].color_ = ps[3].color_ = ps[2].color_ = ps[1].color_;
				ps[0].position_ = Vector3(scRadius, scDistance, scDistance);
				ps[1].position_ = Vector3(scRadius, scDistance, -scDistance);
				ps[2].position_ = Vector3(scRadius, -scDistance, -scDistance);
				ps[3].position_ = Vector3(scRadius, -scDistance, scDistance);
				ps[4].position_ = Vector3(scRadius, scDistance, scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 5, ps, sVDT_PositionColor::getSize());
			}
		}
	}
}
void renderAxisXYZ()
{
	//����Ĭ�ϴ�
	getRenderContex()->getDxDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	getRenderContex()->SetTransform(D3DTS_WORLD, &Matrix::Identity);
	getRenderContex()->applyViewMatrix();
	getRenderContex()->applyProjectionMatrix();
	getRenderContex()->setVertexDeclaration(sVDT_PositionColor::getType());
	//x
	renderAxis(Colour::Red, Vector3::AxisX);
	//y
	renderAxis(Colour::Green, Vector3::AxisY);
	//z
	renderAxis(Colour::Blue, Vector3::AxisZ);
}

void ViewWindow::onIdle()
{
	if (NULL == getRenderContex())
	{
		return;
	}
	//
//	MessageBox("t");
	if (!getRenderContex()->isInitialized())
	{
		getRenderContex()->setWaitForVBL(false);
		int index = 0;
		bool pf = false;
		for (int i = 0; i != getRenderContex()->getDevicesNumber(); ++i)
		{
			DeviceInfo di = getRenderContex()->getDeviceInfo(i);
			std::string dp(di.identifier_.Description);
			if (dp.find("PerfHUD") != std::string::npos)
			{
				index = i;
				pf = true;
				break;
			}
		}
		getRenderContex()->createDevice(m_hWnd, index, 0, true, true, Vector2::Zero);
		getGlobal()->create();
		camera_.setSpeed(5.0f);
		camera_.turboSpeed(10.0f);
		Vector3 minBound = -Vector3( 100.5f, 0.f, 100.5f );
		Vector3 maxBound = Vector3(10000, 5000.0f, 10000.0f);
		camera_.limit_ =  BoundingBox( minBound, maxBound );
		camera_.create(10, MATH_PI, MATH_PI_Half);
		camera_.init(Vector3(0, 0, -20), Vector3(0, 0, 0), Vector3::AxisY);
		//
		Camera c = getRenderContex()->getCamera();
		c.setFarPlane(10000.0f);
		getRenderContex()->setCamera(c);
		getRenderContex()->updateProjectionMatrix();
		getSceneManager()->setRunType(eRunType_Editor);
		getSceneManager()->setAllChunksVisible(true);
	}
	//
	static float lastTick = GetTickCount();
	float currentTick = GetTickCount();
	float delta = currentTick - lastTick;
	lastTick = currentTick;
	camera_.update(delta);
	getSceneManager()->update();
	{
		getGlobal()->update(delta);
	}
	//
	getRenderContex()->setViewMatrix(camera_.view_);
	//
	u32 clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if ( getRenderContex()->isStencilAvailable() )
		clearFlags |= D3DCLEAR_STENCIL;
	getRenderContex()->getDxDevice()->Clear( 0, NULL, clearFlags, 0, 1, 0 );
	getRenderContex()->beginScene();
	renderAxisXYZ();
	//poem();
	if (getSceneManager())
	{
		getSceneManager()->render();
		getGlobal()->render();
	}
	getRenderContex()->endScene();
	getRenderContex()->present();
}

void ViewWindow::onRefreshLuaScript()
{

}