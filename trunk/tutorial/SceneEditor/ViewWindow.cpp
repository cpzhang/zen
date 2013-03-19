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
#include "font/FontManager.h"
#include "font/FreeType.h"
#include "PreviewWindow.h"
#include "font/FlowText.h"
#include "StateManager.h"
ViewWindow::ViewWindow()
{
	_fps = 0.0f;
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
	getGlobal()->onKeyDown(wParam);
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
// 	b = FALSE;//不加这句，无法进入onIdle()
// 	return 1;
// }

LRESULT ViewWindow::onMouseWheel( UINT, WPARAM wParam, LPARAM, BOOL& b )
{
	//
	float delta = ( short )HIWORD( wParam );
	delta /= 120.0f;
	getGlobal()->onMouseWheel(delta);
	return 1;
}

LRESULT ViewWindow::onMouseMove( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	b = FALSE;
	//获得焦点
	SetFocus();
	getGlobal()->onMouseMove();
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
	//光照默认打开
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

void ViewWindow::onIdle(const float delta)
{
	if (NULL == getRenderContex())
	{
		return;
	}
	//
	if (!getRenderContex()->isInitialized())
	{
		//getRenderContex()->setWaitForVBL(false);
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
		getRenderContex()->updateProjectionMatrix();
		getSceneManager()->setRunType(eRunType_Editor);
		getSceneManager()->setAllChunksVisible(true);
		//
		{
			font_ = FontManager::getPointer()->createFont(std::string("freetype\\simkai.ttf"), 16, eFontProperty_Normal, "freeNormal");
		}
		//
		{
			getGlobal()->createPreviewer();
		}
	}
	//
	getSceneManager()->update(delta);
	getGlobal()->update(delta);	
	//
	u32 clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if ( getRenderContex()->isStencilAvailable() )
		clearFlags |= D3DCLEAR_STENCIL;
	static Vector4 scc(0.223f,0.427f,0.647f, 0.0f);
	getRenderContex()->getDxDevice()->Clear( 0, NULL, clearFlags, scc.getARGB(), 1, 0 );
	getRenderContex()->beginScene();
	renderAxisXYZ();
	//poem();
	if (getSceneManager())
	{
		//画地表
		getSceneManager()->render();
		//画人
		getGlobal()->render();
		//寻路点
		getGlobal()->renderPath();
	}
	FlowText::getSingletonP()->update(delta);
	FlowText::getSingletonP()->render();
	//屏幕字，最后画
	{
		std::ostringstream ss;
		ss<<"FPS = "<<delta;
		font_->render(Vector2(10, 10), Vector4(1, 0, 0, 1), ss.str());
	}
	font_->render();
	getRenderContex()->endScene();
	getRenderContex()->present();
	//
	//getGlobal()->renderPreviewWindow(delta);
}

void ViewWindow::onRefreshLuaScript()
{

}

LRESULT ViewWindow::onMouseLeftButtonDown( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	getGlobal()->onMouseLeftButtonDown();
	SetCapture();
	getStateManager()->getCurrentState()->onMouseLeftButtonDown();
	//
	
	return 1;
}
LRESULT ViewWindow::onMouseLeftButtonUp( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	//getGlobal()->getCamera()->setCapture(false);
	ReleaseCapture();
	getStateManager()->getCurrentState()->onMouseLeftButtonUp();
	return 1;
}
LRESULT ViewWindow::onMouseRightButtonDown( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	SetCapture();
	//getGlobal()->getCamera()->setCapture(true);
	getStateManager()->getCurrentState()->onMouseRightButtonDown();
	return 1;
}

LRESULT ViewWindow::onMouseRightButtonUp( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	ReleaseCapture();
	//getGlobal()->getCamera()->setCapture(false);
	getStateManager()->getCurrentState()->onMouseRightButtonUp();
	return 1;
}
