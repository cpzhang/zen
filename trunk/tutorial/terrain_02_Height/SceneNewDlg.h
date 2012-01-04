#pragma once
#include "misc/stdHead.h"
#include "misc/atlHead.h"
class CEditImpl : public CWindowImpl<CEditImpl, CEdit>
{
public:
	BEGIN_MSG_MAP(CEditImpl)
	END_MSG_MAP()
};

class SceneNewDlg :
	public CDialogImpl<SceneNewDlg>
	,public CWinDataExchange<SceneNewDlg>

{
private:
	//
	CEditImpl   mEditWidth;
	int			mWidth;
	//
	CEditImpl   mEditHeight;
	int			mHeight;
	//
	CEditImpl	mNameEdit;
	tstring mName;
	//
	CEditImpl	mPathEdit;
	tstring mPath;
private:
	void clear();
public:
	SceneNewDlg();
	~SceneNewDlg();
	enum { IDD = IDD_NewScene};

	BEGIN_MSG_MAP(SceneNewDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BUTTON_CreateScene, BN_CLICKED, OnBnClickedOk)
		COMMAND_HANDLER(IDC_BUTTON_CancelNewScene, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()

	BEGIN_DDX_MAP(SceneNewDlg)
		DDX_CONTROL(IDC_EDIT_ChunksX, mEditWidth)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_ChunksX, mWidth, 1, 200)//做校验
		DDX_CONTROL(IDC_EDIT_ChunksZ, mEditHeight)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_ChunksZ, mHeight, 1, 200)//做校验
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnDataValidateError ( UINT nCtrlID, BOOL bSave, _XData& data );
};