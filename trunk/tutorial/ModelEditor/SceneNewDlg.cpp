#include "SceneNewDlg.h"
#include "scene/SceneManager.h"
#include "scene/Terrain.h"
void SceneNewDlg::clear()
{
	mWidth = 1;
	mHeight = 1;
	mN = 5;
	mName.clear();
	mPath.clear();
}

SceneNewDlg::SceneNewDlg()
{

}

SceneNewDlg::~SceneNewDlg()
{
	clear();
}

LRESULT SceneNewDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	//
	clear();
	// First DDX call, hooks up variables to controls.
	DoDataExchange(false); 
	//
	CenterWindow();
	ShowWindow(SW_NORMAL);
	return TRUE;
}

LRESULT SceneNewDlg::OnBnClickedOk( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// Transfer data from the controls to member variables.
	if (DoDataExchange(true))
	{
		//
		getSceneManager()->createTerrain(mWidth, mHeight, mN, 3);
		Terrain* tn = SceneManager::getInstancePtr()->getTerrain();
		if (tn)
		{
			tn->setFileName(mPath + TEXT("/") + mName);
			tstring fxName(TEXT("e:/ZenBin/data/shader/Terrain.fx"));
			tn->setFX(fxName);
		}
		//
		EndDialog(0);
		//
	}
	return 0;
}

LRESULT SceneNewDlg::OnBnClickedCancel( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(0);
	return 0;
}

void SceneNewDlg::OnDataValidateError( UINT nCtrlID, BOOL bSave, _XData& data )
{
	MessageBox(TEXT("width and height range [1, 200], lod range [0, 6]"));
	return;
}
