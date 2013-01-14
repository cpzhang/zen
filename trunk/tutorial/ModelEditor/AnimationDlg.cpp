#include "AnimationDlg.h"
#include "Global.h"
#include "EventManager.h"
#include "model/Entity.h"
#include "model/Skeleton.h"
LRESULT AnimationDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = TRUE;
	//
	CenterWindow();
	//
	//getGlobal()->addHandler("AnimationDlg", this);
	//
	//
	properties_ = GetDlgItem(IDC_LIST_Animation);
	//properties_.AddItem( PropCreateSimpleInt(_T("动画"), 0), CCategoryProperty::getNullObject());
	//
	return bHandled;
}

LRESULT AnimationDlg::OnDestroyDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	//getGlobal()->removeHandler("AnimationDlg");
	return 1;
}

AnimationDlg::AnimationDlg()
{
	EventManager::GetInstance().subscribeEvent(eEvent_SelectModel, SubscriberSlot(&AnimationDlg::onModelChanged, this));
//	Entity_ = NULL;
}

AnimationDlg::~AnimationDlg()
{

}

bool AnimationDlg::onModelChanged( const EventArgs& args)
{
	properties_.ResetContent();
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
	/*Skeleton* st = p->getSkeleton();
	for (int i = 0; i != st->getSkinAnimationNumber(); ++i)
	{
		sSkinAnimation* a = st->getSkinAnimation(i);
		if (a)
		{
			properties_.AddString(a->name.c_str());
		}
	}*/
	return true;
}
LRESULT AnimationDlg::OnLbnSelchangeListAnimation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	//if (NULL == Entity_)
	//{
	//	return 0;
	//}
	
	int t = properties_.GetCurSel();
	char buf[256]={0};
	properties_.GetText(t, buf);
	getGlobal()->setAnimation(buf);
	return 0;
}
