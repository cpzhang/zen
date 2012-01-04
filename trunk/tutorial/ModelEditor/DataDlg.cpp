#include "DataDlg.h"
#include "Global.h"
#include "luaScript/LuaScript.h"
#include "render/FxManager.h"
#include "model/Part.h"
#include "model/Mesh.h"
#include "model/Material.h"
#include "model/Entity.h"
#include "model/PartInstance.h"
LRESULT DataDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = TRUE;
	//
	fileTree_.SubclassWindow( ::GetDlgItem( m_hWnd, IDC_TREE_Data ) );
	//fileTree_.SetRootFolder(getGlobal()->getDataRootDirectory());

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false); 
	//
	CenterWindow();
	//
	getGlobal()->addHandler("DataDlg", this);
	//
	//
	properties_.SubclassWindow(GetDlgItem(IDC_LIST_DataProperty));
	properties_.SetExtendedListStyle(PLS_EX_CATEGORIZED);

	//
	return bHandled;
}

LRESULT DataDlg::OnFileItemSelected( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	LPCSTR FileName = (LPCSTR)wParam;
	//getLuaScript()->doFile(FileName);
	getGlobal()->setCurrentLayer(FileName);
	//
	IFileManager::getFile(FileName)->update(FileName, &properties_);
	return TRUE;
}


void DataDlg::onIdle()
{
}

void DataDlg::onRefreshLuaScript()
{
	fileTree_.SetRootFolder(getGlobal()->getDataRootDirectory());
}

LRESULT DataDlg::OnDestroyDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	getGlobal()->removeHandler("DataDlg");
	return 1;
}

LRESULT OptionsDlg::OnItemChanged( int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/ )
{
	LPNMPROPERTYITEM nmp = (LPNMPROPERTYITEM)pnmh;
	bool pathParasChanged =false;
	if (nmp->prop == properties_.FindProperty(_T("ShaderModel")))
	{
		CComVariant v;
		properties_.GetItemValue(nmp->prop, &v);
		getFxManager()->setShaderType((eFx)v.iVal);
		//
	}
	return 0;
}

LRESULT DataDlg::OnBnClickedButtonDatarefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	//std::string d = fileTree_.ItemToPath(0);
	//fileTree_.SetSelectedPath(d);
	//fileTree_.OnViewRefresh();
	fileTree_.ReLoad();
	//getGlobal()->refreshDataRoot();
	return 0;
}

void FileNull::update( const std::string& fn, CPropertyListCtrl* pl , bool reset /*= true*/, CCategoryProperty* ct /*= NULL*/)
{
	if (reset)
	{
		pl->ResetContent();
	}
}

void FilePart::update( const std::string& fn, CPropertyListCtrl* pl , bool reset /*= true*/, CCategoryProperty* ct /*= NULL*/)
{
	if (reset)
	{
		pl->ResetContent();
	}
	//部件
	Part* p = getPartManager()->get(fn);
	if (NULL == p)
	{
		return;
	}
	Mesh* m = p->getMesh();
	if (m)
	{
		CCategoryProperty* pCategory = NULL;
		HPROPERTY c = pl->AddItem( PropCreateCategory(_T("Mesh")) );
		if (!reset)
		{
			pCategory = (CCategoryProperty*)(c);
			pCategory->SetLevel(1);
			pCategory->setCategory(ct);
		}
		pl->AddItem( PropCreateSimpleString(_T("文件"), FileSystem::cutDataPath(m->getFilePath()).c_str()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("版本"), m->getVersion()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("面数"), m->getFaceNumber()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("顶点"), m->getVertexNumber()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("骨骼"), 0), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("动画"), 0), pCategory);
	}
	//材质
	Material* t = p->getMaterial();
	if (t)
	{
		HPROPERTY c = pl->AddItem( PropCreateCategory(_T("Material")));
		CCategoryProperty* pCategory = NULL;
		if (!reset)
		{
			pCategory = (CCategoryProperty*)(c);
			pCategory->SetLevel(1);
			pCategory->setCategory(ct);
		}
		pl->AddItem( PropCreateSimpleString(_T("文件"), FileSystem::cutDataPath(t->getFilePath()).c_str()), pCategory);
		Fx* f = t->getFx();
		if (f)
		{
			pl->AddItem( PropCreateSimpleString(_T("FX"), FileSystem::cutDataPath(f->getFilePath()).c_str()), pCategory);
		}
	}
}
void FileEntity::update( const std::string& fn, CPropertyListCtrl* pl , bool reset /*= true*/, CCategoryProperty* ct /*= NULL*/)
{
	//清空
	if (reset)
	{
		pl->ResetContent();
	}
	//部件
	Entity* p = getEntityManager()->get(fn);
	if (NULL == p)
	{
		return;
	}
	for (size_t i = 0; i != p->getPartInstanceNumber(); ++i)
	{
		PartInstance* pi = p->getPartInstance(i);
		if (pi)
		{
			Part* pt = pi->getPart();
			if (pt)
			{
				HPROPERTY c = pl->AddItem( PropCreateCategory(pt->getFilePath().c_str()) );
				CCategoryProperty* pCategory = (CCategoryProperty*)(c);
				pCategory->SetLevel(0);
				pCategory->setCategory(NULL);
				IFileManager::getFile(eFile_Part)->update(pt->getFilePath(), pl, false, pCategory);
			}
		}
	}
}
IFile* IFileManager::getFile( const std::string& fn )
{
	eFile f = eFile_Null;
	tstring ext = FileSystem::getFileExtension(fn);
	if (ext == "part")
	{
		f = eFile_Part;
	}
	else if (ext == "entity")
	{
		f = eFile_Entity;
	}
	return getFile(f);
}

IFile* IFileManager::getFile( eFile f )
{
	static FilePart fp;
	static FileNull fn;
	static FileEntity fe;
	IFile* i = &fn;
	switch(f)
	{
	case eFile_Part:
		{
			i = &fp;
		}
		break;
	case eFile_Entity:
		{
			i = &fe;
		}
		break;
	}
	return i;
}
