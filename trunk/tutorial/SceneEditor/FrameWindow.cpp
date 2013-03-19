#include "FrameWindow.h"
#include "SceneNewDlg.h"
#include "StateManager.h"
#include "scene/SceneManager.h"
#include "Global.h"
#include "PreviewWindow.h"
#include "luaScript/LuaScript.h"
#include "font/FlowText.h"
#include "scene/navigation.h"
FrameWindow::FrameWindow()
{

}

LRESULT FrameWindow::onCreate( UINT, WPARAM, LPARAM, BOOL& )
{
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
	UIAddToolBar(hWndToolBar);
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	//
	verticalSplitter_.Create ( *this, rcDefault );
	m_hWndClient = verticalSplitter_;	
	verticalSplitter_.SetSplitterExtendedStyle(SPLIT_FIXEDBARSIZE);

	//
	canvas_.Create(verticalSplitter_, rcDefault);
	verticalSplitter_.SetSplitterPane(SPLIT_PANE_LEFT, canvas_);

	//
	tabs_.Create(verticalSplitter_, rcDefault);
	verticalSplitter_.SetSplitterPane(SPLIT_PANE_RIGHT, tabs_);
	//
	dlgData_.Create(tabs_);
	tabs_.AddPage(dlgData_, TEXT("��Դ"));
	idleHandlers[ePanel_Data] = &dlgData_;
	//
	dlgChangeHeight_.Create(tabs_);
	tabs_.AddPage(dlgChangeHeight_, TEXT("�߶�"));
	idleHandlers[ePanel_TerrainHeight] = &dlgChangeHeight_;
	//
	dlgTerrainTexture_.Create(tabs_);
	tabs_.AddPage(dlgTerrainTexture_, TEXT("����"));
	idleHandlers[ePanel_TerrainTexture] = &dlgTerrainTexture_;
	//
	dlgPlaceModel.Create(tabs_);
	tabs_.AddPage(dlgPlaceModel, TEXT("���"));
	idleHandlers[ePanel_PlaceModel] = &dlgPlaceModel;
	//
	dlgNav.Create(tabs_);
	tabs_.AddPage(dlgNav, TEXT("����"));
	idleHandlers[ePanel_Nav] = &dlgNav;
	//
	dlgOptions_.Create(tabs_);
	tabs_.AddPage(dlgOptions_, TEXT("ѡ��"));
	idleHandlers[ePanel_Option] = &dlgOptions_;
	//
	panelCurrent_ = ePanel_Data;
	//dlgAnimation_.Create(tabs_);
	//tabs_.AddPage(dlgAnimation_, TEXT("����"));
	//
	UpdateLayout();

	/*
    Note that you need to set m_hWndClient and call CFrameWindowImpl::UpdateLayout() before setting the splitter position. 
    UpdateLayout() resizes the splitter window to its initial size. 
    If you skip that step, the splitter's size isn't under your control and it might be smaller than 200 pixels wide. 
    The end result would be that SetSplitterPos() wouldn't have the effect you wanted.
  */
	verticalSplitter_.SetSplitterPos(1024);
 
	//
	return 0;
}


LRESULT FrameWindow::OnNewscene(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���������������
	if (dlgSceneNew.DoModal())
	{
		scenePath_ = dlgSceneNew.getScenePath();
	}

	return 0;
}

void FrameWindow::onIdle(const float delta)
{
	UIUpdateToolBar(FALSE);
	canvas_.onIdle(delta);
	idleHandlers[panelCurrent_]->onIdle(delta);
}

void FrameWindow::onRefreshLuaScript()
{
	canvas_.onRefreshLuaScript();
}

LRESULT FrameWindow::OnToobarChangeHeight( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	if (getSceneManager() == NULL || getSceneManager()->getTerrain() == NULL)
	{
		FlowText::getSingletonP()->add("û�г����ɹ��༭�����½�������еĳ���", Vector4(1, 1, 1, 1));
		return 0;
	}
	FlowText::getSingletonP()->add("�л����ر����߶�ͼģʽ", Vector4(1, 1, 1, 1));
	getStateManager()->gotoState(eState_TerrainHeight);
	tabSwitch(ID_BUTTON_ChangeHeight);
	tabs_.SetActivePage(ePanel_TerrainHeight);
	return 0;
}

LRESULT FrameWindow::OnToobarChangeTexture( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	if (getSceneManager() == NULL || getSceneManager()->getTerrain() == NULL)
	{
		FlowText::getSingletonP()->add("û�г����ɹ��༭�����½�������еĳ���", Vector4(1, 1, 1, 1));
		return 0;
	}
	FlowText::getSingletonP()->add("�л����ر�ˢ����ģʽ", Vector4(1, 1, 1, 1));
	getStateManager()->gotoState(eState_TerrainTexture);
	UISetCheck(ID_BUTTON_PaintTerrain, true);

	tabs_.SetActivePage(ePanel_TerrainTexture);
	return 0;
}

LRESULT FrameWindow::OnToobarData( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	//getStateManager()->gotoState(eState_Data);
	tabSwitch(ID_BUTTON_Data);
	tabs_.SetActivePage(ePanel_Data);
	return 0;
}

LRESULT FrameWindow::OnToobarOptions( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	//getStateManager()->gotoState(eState_Data);
	tabSwitch(ID_BUTTON_Options);
	tabs_.SetActivePage(4);
	return 0;
}

void FrameWindow::tabSwitch(WORD id)
{
	UISetCheck(ID_BUTTON_Data, false);
	UISetCheck(ID_BUTTON_ChangeHeight, false);
	UISetCheck(ID_BUTTON_PaintTerrain, false);
	UISetCheck(ID_BUTTON_PlaceModel, false);
	UISetCheck(ID_BUTTON_NavigationMesh, false);
	UISetCheck(ID_BUTTON_Options, false);
	//
	UISetCheck(id, true);
}

LRESULT FrameWindow::OnSavescene(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���������������
	if (FileSystem::isDirExist(scenePath_))
	{
		if (IDOK != MessageBox("�Ѵ��ڳ������ƣ�ȷ��Ҫ������", "��ʾ", MB_OKCANCEL))
		{
			return 1;
		}
	}
	else
	{
		FileSystem::createFolder(scenePath_);
	}
	FlowText::getSingletonP()->add("���ڱ��泡��", Vector4(1, 1, 1, 1));
	getSceneManager()->save(scenePath_);
	FlowText::getSingletonP()->add("���泡�����", Vector4(1, 1, 1, 1));
	return 0;
}

LRESULT FrameWindow::OnOpenscene(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���������������
	CFolderDialog d(*this, "�����ļ�");
	//d.SetInitialFolder(TEXT("d:\\work\\zen\\data\\scene"));
	if (d.DoModal(*this))
	{
		scenePath_ = d.m_szFolderPath;
		getSceneManager()->open(d.m_szFolderPath);
	}
	return 0;
}

LRESULT FrameWindow::OnTest( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	scenePath_ = TEXT("\\scene\\1");
	scenePath_ = FileSystem::addDataDir(scenePath_);
	getSceneManager()->open(scenePath_);
	std::string resId(TEXT("\\model\\Character_1015\\Character_1015.entity"));
	getGlobal()->setHero(resId.c_str());
	return 0;
}

LRESULT FrameWindow::OnHero( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	if (getSceneManager() == NULL || getSceneManager()->getTerrain() == NULL)
	{
		FlowText::getSingletonP()->add("û�г����ɹ��༭�����½�������еĳ���", Vector4(1, 1, 1, 1));
		return 0;
	}
	getLuaScript()->doFile("lua/HeroButton.lua");
	return 0;
}

LRESULT FrameWindow::OnPlaceModel( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	if (getSceneManager() == NULL || getSceneManager()->getTerrain() == NULL)
	{
		FlowText::getSingletonP()->add("û�г����ɹ��༭�����½�������еĳ���", Vector4(1, 1, 1, 1));
		return 0;
	}
	getStateManager()->gotoState(eState_PlaceModel);
	FlowText::getSingletonP()->add("�л����������ģʽ", Vector4(1, 1, 1, 1));
	UISetCheck(ID_BUTTON_PlaceModel, true);
	tabSwitch(ID_BUTTON_PlaceModel);
	tabs_.SetActivePage(ePanel_PlaceModel);
	return 1;
}

LRESULT FrameWindow::OnNavigationMesh( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	if (getSceneManager() == NULL || getSceneManager()->getTerrain() == NULL)
	{
		FlowText::getSingletonP()->add("û�г����ɹ��༭�����½�������еĳ���", Vector4(1, 1, 1, 1));
		return 0;
	}
	getStateManager()->gotoState(eState_Nav);
	UISetCheck(ID_BUTTON_NavigationMesh, true);
	FlowText::getSingletonP()->add("�л�������ģʽ", Vector4(1, 1, 1, 1));
	tabs_.SetActivePage(ePanel_Nav);
	return 1;
}
