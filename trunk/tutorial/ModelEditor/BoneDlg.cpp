#include "BoneDlg.h"
#include "Global.h"
#include "eventargs.h"
#include "model/Entity.h"
#include "model/Skeleton.h"
#include "model/EntityInstance.h"
#include "model/BoneNode.h"
LRESULT BoneDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = TRUE;
	//
	CenterWindow();
	//
	bonesList_ = GetDlgItem(IDC_LIST_Bones);
	//
	nodesList_ = GetDlgItem(IDC_LIST_BoneNodes);
	//
	return bHandled;
}

LRESULT BoneDlg::OnDestroyDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 1;
}

BoneDlg::BoneDlg()
{
	ss_ = SubscriberSlot(&BoneDlg::onModelChanged, this);
	EventManager::GetInstance().subscribeEvent(SelectModelEventArgs::tEventName, ss_);
}

BoneDlg::~BoneDlg()
{
	ss_.release();
}

bool BoneDlg::onModelChanged( const EventArgs& args)
{
	nodesList_.ResetContent();
	bonesList_.ResetContent();
	const SelectModelEventArgs* m = (SelectModelEventArgs*)&args;
	if (m->FilePath_.empty())
	{
		return false;
	}
	Entity* p = ModelResManager::getInstance()->get<Entity>(m->FilePath_);
	if (Entity::getNullObject() == p)
	{
		return false;
	}
	Skeleton* st = ModelResManager::getInstance()->get<Skeleton>(p->getSkeleton());
	BoneNodeMapIterator it = st->getCommandMapIterator();
	while (!it.isAtEnd())
	{
		BoneNode* a = it.getCurrentValue();
		if (a)
		{
			bonesList_.AddString(a->getBone()->name.c_str());
		}
		++it;
	}
	return true;
}
LRESULT BoneDlg::OnLbnSelchangeListAnimation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int t = bonesList_.GetCurSel();
	char buf[256]={0};
	bonesList_.GetText(t, buf);
	nodesList_.ResetContent();
	return 0;
}
LRESULT BoneDlg::OnContextMenuListBones( UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
	POINT p;
	p.x = GET_X_LPARAM(lParam); 
	p.y = GET_Y_LPARAM(lParam); 
	//ScreenToClient(&p);
	BOOL bOutSide = true;
	RECT rc;
	bonesList_.GetWindowRect(&rc);
	p.x -= rc.left;
	p.y -= rc.top;
	UINT n = bonesList_.ItemFromPoint(p, bOutSide);
	if (!bOutSide)
	{
		bonesList_.SetCurSel(n);
		char buf[256]={0};
		bonesList_.GetText(n, buf);
		//mCurrentBone = buf;
		//
		CMenu menu;
		menu.LoadMenu(IDR_MENU_BoneListRightMouse);
		CMenuHandle menuPopup = menu.GetSubMenu(0);
		TrackPopupMenu(menuPopup,TPM_RIGHTBUTTON | TPM_VERPOSANIMATION | TPM_VERTICAL,
			GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, *this, NULL);
	}
	return 0;
}
LRESULT BoneDlg::OnReferencemodelBind(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LPCTSTR lpcstrFilter = 
		_T("Model Files (*.mz;*.md)\0*.mz;*.md\0")
		_T("All Files (*.*)\0*.*\0")
		_T("");
	CFileDialog dlg(TRUE, _T("mz;md"), _T("e:\\Rocket20100701\\Bin\\Client\\Data\\md\\0.md"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpcstrFilter);
	int nRet = dlg.DoModal();
	if(nRet == IDOK) 
	{
		
	}
	return 0;
}
