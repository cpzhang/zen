#include "PreviewWindow.h"
#include "render/rendercontext.h"
#include "font/FontManager.h"
#include "Global.h"
#include "model/EntityInstance.h"
#include "scene/Node.h"
void PreviewWindow::render()
{
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
	getGlobal()->renderPreviewWindow();
	return 1;    // we painted the background
}

void PreviewWindow::setModel( EntityInstance* m )
{
	Hero_->attach(m);
}

PreviewWindow::PreviewWindow()
{
	Hero_ = NodeManager::getInstancePtr()->createNode("HeroPreviewer");
}

PreviewWindow::~PreviewWindow()
{
	if (Hero_)
	{
		Hero_->release();
		Hero_ = NULL;
	}
}

LRESULT PreviewWindow::onMouseMove( UINT, WPARAM, LPARAM lParam, BOOL& b )
{
	//获得焦点
	SetFocus(); 
	return 1;
}
