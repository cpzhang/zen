#pragma once
#include "misc/stdHead.h"
#include "misc/atlHead.h"
#include "CEditImpl.h"
#include "resource.h"
class SceneNewDlg :
	public CDialogImpl<SceneNewDlg>
	,public CWinDataExchange<SceneNewDlg>

{
private:
	//
	CEditImpl   mEditN;
	int			mN;
	//
	CEditImpl   mEditWidth;
	int			mWidth;
	//
	CEditImpl   mEditHeight;
	int			mHeight;
	//
	CEditImpl	mNameEdit;
	//tstring mName;
	CString mcstrName;
	//TCHAR tName[512];
	//
	CEditImpl	mPathEdit;
	tstring		mPath;
	TCHAR tPath[512];
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
		COMMAND_HANDLER(IDC_BUTTON_ScenePath, BN_CLICKED, OnBnClickedButtonScenepath)
	END_MSG_MAP()

	BEGIN_DDX_MAP(SceneNewDlg)
		DDX_CONTROL(IDC_EDIT_ChunksX, mEditWidth)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_ChunksX, mWidth, 1, 200)//做校验
		DDX_CONTROL(IDC_EDIT_ChunksZ, mEditHeight)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_ChunksZ, mHeight, 1, 200)//做校验
		DDX_CONTROL(IDC_EDIT_N, mEditN)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_N, mN, 0, 5)//做校验
		DDX_CONTROL(IDC_ScenePath, mPathEdit)//功能等价于SubclassWindow，接管消息回调
		//DDX_TEXT(IDC_ScenePath, tPath)
		DDX_CONTROL(IDC_SceneName, mNameEdit)//功能等价于SubclassWindow，接管消息回调
		DDX_TEXT(IDC_SceneName, mcstrName)
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnDataValidateError ( UINT nCtrlID, BOOL bSave, _XData& data );
	LRESULT OnBnClickedButtonScenepath(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
	tstring getScenePath()
	{
		return mPath;
	}
	tstring getSceneName()
	{
		return mcstrName.GetBuffer(0);
	}
};