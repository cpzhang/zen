#include "AnimationDlg.h"
#include "Global.h"
#include "eventargs.h"
#include "model/Entity.h"
#include "model/Skeleton.h"
#include "model/EntityInstance.h"
LRESULT AnimationDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = TRUE;
	//
	CenterWindow();
	//
	properties_ = GetDlgItem(IDC_LIST_Animation);
	//
	radius_.SubclassWindow(GetDlgItem(IDC_SLIDER_AnimationSpeed));
	radius_.SetRange(1, 100);
	radius_.SetPos(50);
	radius_.registerCallback(SubscriberSlot(&AnimationDlg::upateRadius, this));
	//
	return bHandled;
}
bool AnimationDlg::upateRadius( const EventArgs& args)
{
	const TrackBarCtrlWrapperEventArgs* m = (TrackBarCtrlWrapperEventArgs*)&args;
	if (getGlobal()->getHero())
	{
		getGlobal()->getHero()->setSpeed(m->nPercent * 0.02);
	}
	return true;
}
LRESULT AnimationDlg::OnDestroyDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 1;
}

AnimationDlg::AnimationDlg()
{
	ss_ = SubscriberSlot(&AnimationDlg::onModelChanged, this);
	EventManager::GetInstance().subscribeEvent(SelectModelEventArgs::tEventName, ss_);
}

AnimationDlg::~AnimationDlg()
{
	ss_.release();
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
	Skeleton* st = ModelResManager::getInstance()->get<Skeleton>(p->getSkeleton());
	for (int i = 0; i != st->getSkinAnimationNumber(); ++i)
	{
		sSkinAnimation* a = st->getSkinAnimation(i);
		if (a)
		{
			properties_.AddString(a->name.c_str());
		}
	}
	return true;
}
LRESULT AnimationDlg::OnLbnSelchangeListAnimation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int t = properties_.GetCurSel();
	char buf[256]={0};
	properties_.GetText(t, buf);
	getGlobal()->setAnimation(buf);
	return 0;
}
