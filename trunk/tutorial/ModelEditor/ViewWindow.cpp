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
	switch(wParam)
	{
	case VK_ADD:
		{
			//if (g_bActive)
			{
				camera_.setSpeed(camera_.getSpeed() * 1.2f);
			}
		}break;
	case VK_SUBTRACT:
		{
			//if (g_bActive)
			{
				camera_.setSpeed(camera_.getSpeed() * 0.8f);
			}
		}break;
	}

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
	delta /= 1200.0f;
	camera_.onMouseWheel(delta);
	return 1;
}

LRESULT ViewWindow::onMouseMove( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	//获得焦点
	SetFocus();
	camera_.onMouseMove();
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
void renderAxis(u32 scColor, const Vector3& axis)
{
	//axis
	{
		static const float scLength = 100.0f;
		//x
		{
			sVDT_PositionColor ps[2];
			ps[0].color_ = scColor;
			ps[0].position_ = -scLength * axis;
			ps[1].color_ = scColor;
			ps[1].position_ = scLength * axis;
			getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 1, ps, sVDT_PositionColor::getSize());
		}
	}
	//arrow
	{
		static const float scRadius = 10.0f;
		static const float scLength = 100.0f;
		static const float scDistance = 2.0f;
		//x
		{
			sVDT_PositionColor ps[5];

			ps[0].color_ = scColor;
			ps[0].position_ = (scLength + scRadius) * axis;
			ps[1].color_ = scColor;
			//
			{
				ps[1].position_ = scLength * axis + Vector3(0, scDistance, scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 1, ps, sVDT_PositionColor::getSize());
			}
			//
			{
				ps[1].position_ = scLength * axis + Vector3(0, -scDistance, scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 1, ps, sVDT_PositionColor::getSize());
			}
			//
			{
				ps[1].position_ = scLength * axis + Vector3(0, -scDistance, -scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 1, ps, sVDT_PositionColor::getSize());
			}
			//
			{
				ps[1].position_ = scLength * axis + Vector3(0, scDistance, -scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST, 1, ps, sVDT_PositionColor::getSize());
			}
			//
			{
				ps[4].color_ = ps[3].color_ = ps[2].color_ = ps[1].color_;
				ps[0].position_ = scLength * axis + Vector3(0, scDistance, scDistance);
				ps[1].position_ = scLength * axis + Vector3(0, scDistance, -scDistance);
				ps[2].position_ = scLength * axis + Vector3(0, -scDistance, -scDistance);
				ps[3].position_ = scLength * axis + Vector3(0, -scDistance, scDistance);
				ps[4].position_ = scLength * axis + Vector3(0, scDistance, scDistance);
				getRenderContex()->drawPrimitiveUP(D3DPT_LINESTRIP, 4, ps, sVDT_PositionColor::getSize());
			}
		}
	}
}
void renderPlane()
{
	getRenderContex()->SetTransform(D3DTS_WORLD, &Matrix::Identity);
	static const float scRadius = 300.0f;
	static const float scStep = 10.0f;
	static const Vector4 scColor(60.0f/255.0f, 80.0f/255.0f, 100.0f/255.0f, 1.0);
	getRenderContex()->setVertexDeclaration(sVDT_PositionColor::getType());
	for (float z = -scRadius; z <= scRadius; z += scStep)
	{
		sVDT_PositionColor ps[2];
		ps[0].color_ = scColor.getARGB();
		ps[0].position_ = Vector3(-scRadius, 0.0f, z);
		ps[1].color_ = scColor.getARGB();
		ps[1].position_ = Vector3(scRadius, 0.0f, z);
		getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST,  1, ps, sVDT_PositionColor::getSize());
	}
	for (float x = -scRadius; x <= scRadius; x += scStep)
	{
		sVDT_PositionColor ps[2];
		ps[0].color_ = scColor.getARGB();
		ps[0].position_ = Vector3(x, 0.0f, -scRadius);
		ps[1].color_ = scColor.getARGB();
		ps[1].position_ = Vector3(x, 0.0f, scRadius);
		getRenderContex()->drawPrimitiveUP(D3DPT_LINELIST,  1, ps, sVDT_PositionColor::getSize());
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
	Matrix m;
	Quaternion q;
	q.fromAngleAxis(HalfPI, Vector3::AxisZ);
	m.make(Vector3::Zero, Vector3::One, q);
	getRenderContex()->SetTransform(D3DTS_WORLD, &m);
	renderAxis(Colour::Green, Vector3::AxisX);
	//z
	q.fromAngleAxis(-HalfPI, Vector3::AxisY);
	m.make(Vector3::Zero, Vector3::One, q);
	getRenderContex()->SetTransform(D3DTS_WORLD, &m);
	renderAxis(Colour::Blue, Vector3::AxisX);
	//
	getRenderContex()->SetTransform(D3DTS_WORLD, &Matrix::Identity);
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
		Vector3 minBound = -Vector3( 100.5f, 0.f, 100.5f );
		Vector3 maxBound = Vector3(10000, 5000.0f, 10000.0f);
		camera_.limit_ =  BoundingBox( minBound, maxBound );
		camera_.create(30, MATH_PI*0.75f, MATH_PI_Half*0.5f);
		//
		Camera c = getRenderContex()->getCamera();
		c.setFarPlane(10000.0f);
		getRenderContex()->setCamera(c);
		getRenderContex()->updateProjectionMatrix();
		getSceneManager()->setRunType(eRunType_Editor);
		getSceneManager()->setAllChunksVisible(true);
		//
		{
			font_ = FontManager::getPointer()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 18, eFontProperty_Normal, "freeNormal");
		}
	}
	//
	camera_.update(delta, 0.0f);
	getSceneManager()->update(delta);
	{
		getGlobal()->update(delta);
	}
	//
	getRenderContex()->setViewMatrix(camera_.view_);
	//
	u32 clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if ( getRenderContex()->isStencilAvailable() )
		clearFlags |= D3DCLEAR_STENCIL;
	static Vector4 scc(0.5f,0.5f,0.5f, 0.5f);
	getRenderContex()->getDxDevice()->Clear( 0, NULL, clearFlags, scc.getARGB(), 1, 0 );
	getRenderContex()->beginScene();
	//plane
	renderPlane();
	//axis
	renderAxisXYZ();
	//
	if (getSceneManager())
	{
		getSceneManager()->render();
		getGlobal()->render();
	}
	//屏幕字，最后画
	{
		std::ostringstream ss;
		ss<<"FPS = "<<_fps;
		font_->render(Vector2(10, 10), Vector4(1, 0, 0, 1), ss.str());
	}
	font_->render();
	getRenderContex()->endScene();
	getRenderContex()->present();
}

void ViewWindow::onRefreshLuaScript()
{

}

void ViewWindow::setFPS( float fps )
{
	_fps = fps;
}
