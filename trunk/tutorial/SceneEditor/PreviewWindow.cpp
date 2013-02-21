#include "PreviewWindow.h"
#include "render/rendercontext.h"
#include "font/FontManager.h"
#include "Global.h"
#include "model/EntityInstance.h"
void PreviewWindow::render()
{
	u32 clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if ( getRenderContex()->isStencilAvailable() )
		clearFlags |= D3DCLEAR_STENCIL;
	static Vector4 scc(0.0f,0.0f,0.0f, 0.0f);
	getRenderContex()->getDxDevice()->Clear( 0, NULL, clearFlags, scc.getARGB(), 1, 0 );
	getRenderContex()->beginScene();
	//ÆÁÄ»×Ö£¬×îºó»­
	{
		std::ostringstream ss;
		ss<<"Preview";
		FontManager::getPointer()->getFont()->render(Vector2(10, 10), Vector4(1, 0, 0, 1), ss.str());
	}
	FontManager::getPointer()->getFont()->render();
	if (model_)
	{
		model_->render();
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
