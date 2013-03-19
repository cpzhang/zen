#pragma once
#include "common/atlHead.h"
#include "misc/EventManager.h"
// arguments
class TrackBarCtrlWrapperEventArgs : public EventArgs
{
public:
	float nPercent;
};

class TrackBarCtrlWrapper : public CWindowImpl<TrackBarCtrlWrapper, CTrackBarCtrl>
{
public:
	BEGIN_MSG_MAP(TrackBarCtrlWrapper)
		MESSAGE_HANDLER(WM_HSCROLL + OCM__BASE, OnNMThemeChangedSlider)//������û�д���reflect���Ӵ�������Ϣ�����OCM__BASE
	END_MSG_MAP()
public:
	void registerCallback(SubscriberSlot slot)
	{
		slot_ = slot;
	}
	LRESULT OnNMThemeChangedSlider( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		int nPercent;
		switch ((int)LOWORD(wParam)) 
		{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			nPercent = (short)HIWORD(wParam);
			bHandled = TRUE;
			break;
		case SB_ENDSCROLL:
		case SB_LINELEFT:
		case SB_LINERIGHT:
		case SB_PAGELEFT:
		case SB_PAGERIGHT:
			nPercent = GetPos();
			bHandled = TRUE;
			break;
		default:
			break;
		}
		if (bHandled)
		{
			TrackBarCtrlWrapperEventArgs args;
			args.nPercent = nPercent;
			slot_(args);
		}
		return bHandled;
	}
private:
	SubscriberSlot slot_;
};