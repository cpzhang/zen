#include "PreviewWindow.h"
#include "render/rendercontext.h"
#include "font/FontManager.h"
#include "Global.h"
#include "model/EntityInstance.h"
#include "scene/Node.h"
#include "scene/SceneManager.h"
void PreviewWindow::render()
{
	//update
	getRenderContex()->setViewMatrix(Camera_.view_);
	//render
	u32 clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if ( getRenderContex()->isStencilAvailable() )
		clearFlags |= D3DCLEAR_STENCIL;
	static Vector4 scc(0.0f,0.0f,0.0f, 0.0f);
	getRenderContex()->getDxDevice()->Clear( 0, NULL, clearFlags, scc.getARGB(), 1, 0 );
	getRenderContex()->beginScene();
	//屏幕字，最后画
	{
		std::ostringstream ss;
		ss<<"Preview";
		FontManager::getPointer()->getFont()->render(Vector2(10, 10), Vector4(1, 0, 0, 1), ss.str());
	}
	FontManager::getPointer()->getFont()->render();
	if (Hero_)
	{
		Hero_->render();
	}
	getRenderContex()->endScene();
	getRenderContex()->present();
}

LRESULT PreviewWindow::OnPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	getGlobal()->renderPreviewWindow(1);
	return 1;    // we painted the background
}

void PreviewWindow::setModel( const std::string resID )
{
	if (EntityInstance_)
	{
		EntityInstance_->destroy();
		EntityInstance_->create(resID);
	}
	else
	{
		EntityInstance_ = getSceneManager()->createEntityInstance(resID);
	}
	EntityInstance_->setAnimation("Stand");
	Hero_->attach(EntityInstance_);
}

PreviewWindow::PreviewWindow()
{
	EntityInstance_ = NULL;
	Hero_ = NodeManager::getInstancePtr()->createNode("HeroPreviewerNode");
	//Hero_ = NULL;
	Camera_.setSpeed(5.0f);
	Vector3 minBound = -Vector3( 100.5f, 0.f, 100.5f );
	Vector3 maxBound = Vector3(10000, 5000.0f, 10000.0f);
	Camera_.limit_ =  BoundingBox( minBound, maxBound );
	Camera_.create(3, MATH_PI, MATH_PI_Half);
}
void PreviewWindow::destroy()
{
	if (EntityInstance_)
	{
		EntityInstance_->release();
		EntityInstance_ = NULL;
	}
	if (Hero_)
	{
		Hero_->release();
		Hero_ = NULL;
	}
}
PreviewWindow::~PreviewWindow()
{
	
}

LRESULT PreviewWindow::onMouseMove( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	//获得焦点
	SetFocus(); 
	Camera_.onMouseMove();
	return 1;
}

LRESULT PreviewWindow::onMouseLeftButtonDown( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	SetCapture();
	Camera_.setCapture(true);
	return 1;
}

LRESULT PreviewWindow::onMouseLeftButtonUp( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	Camera_.setCapture(false);
	ReleaseCapture();
	return 1;
}

LRESULT PreviewWindow::onMouseRightButtonDown( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	SetCapture();
	Camera_.setCapture(true);
	return 1;
}

LRESULT PreviewWindow::onMouseRightButtonUp( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	ReleaseCapture();
	Camera_.setCapture(false);
	return 1;
}

LRESULT PreviewWindow::onMouseWheel( UINT, WPARAM wParam, LPARAM, BOOL& b )
{
	//
	float delta = ( short )HIWORD( wParam );
	delta /= 120.0f;
	Camera_.onMouseWheel(delta);
	return 1;
}

void PreviewWindow::onIdle( float delta )
{
	//update
	Camera_.update(delta, 0.0f);
	if (Hero_)
	{
		Hero_->update(delta);
	}
	render();
}

std::string PreviewWindow::getModelResID() const
{
	std::string id;
	id = EntityInstance_->getResId();
	return id;
}
