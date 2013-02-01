#include "DataDlg.h"
#include "Global.h"
#include "luaScript/LuaScript.h"
#include "render/FxManager.h"
#include "model/Part.h"
#include "model/Mesh.h"
#include "model/Material.h"
#include "model/Entity.h"
#include "model/Skeleton.h"
#include "model/Part.h"
#include "EventManager.h"
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
	//properties_.AddItem( PropCreateSimpleInt(_T("动画"), 0), CCategoryProperty::getNullObject());
	//
	return bHandled;
}

LRESULT DataDlg::OnFileItemSelected( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	LPCSTR FileName = (LPCSTR)wParam;
	SelectFile(FileName);
	
	return TRUE;
}


void DataDlg::SelectFile( const tstring& s )
{
	//getLuaScript()->doFile(FileName);
	getGlobal()->setCurrentLayer(s);
	//
	IFileManager::getFile(s)->update(s, &properties_);
}

void DataDlg::onIdle(const float delta)
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
	/*
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
	if (ct == NULL)
	{
		ct = CCategoryProperty::getNullObject();
	}
	//Mesh
	Mesh* m = p->getMesh();
	if (m)
	{
		CCategoryProperty* pCategory = NULL;
		HPROPERTY c = pl->AddItem( PropCreateCategory(_T("Mesh")), ct);
		pCategory = (CCategoryProperty*)(c);
		pCategory->setCategory(ct);
		if (!reset)
		{
			pCategory->SetLevel(1);
		}
		pl->AddItem( PropCreateSimpleString(_T("文件"), FileSystem::cutDataPath(m->getFilePath()).c_str()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("版本"), m->getVersion()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("面数"), m->getFaceNumber()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("顶点"), m->getVertexNumber()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("动画"), 0), pCategory);
	}
	//材质
	Material* t = p->getMaterial();
	if (t)
	{
		HPROPERTY c = pl->AddItem( PropCreateCategory(_T("Material")), ct);
		CCategoryProperty* pCategory = NULL;
		pCategory = (CCategoryProperty*)(c);
		pCategory->setCategory(ct);
		if (!reset)
		{
			pCategory->SetLevel(1);
		}
		pl->AddItem( PropCreateSimpleString(_T("文件"), FileSystem::cutDataPath(t->getFilePath()).c_str()), pCategory);
		Fx* f = t->getFx();
		if (f)
		{
			pl->AddItem( PropCreateSimpleString(_T("FX"), FileSystem::cutDataPath(f->getFilePath()).c_str()), pCategory);
		}
	}
	*/
}
void FileEntity::update( const std::string& fn, CPropertyListCtrl* pl , bool reset /*= true*/, CCategoryProperty* ct /*= NULL*/)
{
	/*
	//清空
	if (reset)
	{
		pl->ResetContent();
	}
	//部件
	Entity* p = ModelResManager::getInstance()->get<Entity>(fn);
	if (NULL == p)
	{
		return;
	}
	if (ct == NULL)
	{
		ct = CCategoryProperty::getNullObject();
	}
	for (size_t i = 0; i != p->getPartNumber(); ++i)
	{
		Part* pt = p->getPart(i);
		if (pt)
		{
			HPROPERTY c = pl->AddItem( PropCreateCategory(pt->getFilePath().c_str()), ct);
			CCategoryProperty* pCategory = (CCategoryProperty*)(c);
			pCategory->SetLevel(0);
			//pCategory->setCategory(NULL);
			IFileManager::getFile(eFile_Part)->update(pt->getFilePath(), pl, false, pCategory);
		}
	}
	//Skeleton
	Skeleton* st = p->getSkeleton();
	if (st)
	{
		HPROPERTY c = pl->AddItem( PropCreateCategory(_T("Skeleton")), ct);
		CCategoryProperty* pCategory = NULL;
		pCategory = (CCategoryProperty*)(c);
		pCategory->setCategory(ct);
		if (!reset)
		{
			pCategory->SetLevel(1);
		}
		pl->AddItem( PropCreateSimpleString(_T("文件"), FileSystem::cutDataPath(st->getFilePath()).c_str()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("骨骼"), st->getNumBoneNodes()), pCategory);
		pl->AddItem( PropCreateSimpleInt(_T("蒙皮"), st->getSkinAnimationNumber()), pCategory);
		for (int i = 0; i != st->getSkinAnimationNumber(); ++i)
		{
			sSkinAnimation* a = st->getSkinAnimation(i);
			if (a)
			{
				pl->AddItem( PropCreateSimpleString(a->name.c_str(), a->skinFilePath.c_str()), pCategory);
			}
		}

	}
*/
}
IFile* IFileManager::getFile( const std::string& fn )
{
	eFile f = eFile_Null;
	tstring ext = FileSystem::getFileExtension(fn);
	SelectModelEventArgs args;
	if (ext == "part")
	{
		f = eFile_Part;
	}
	else if (ext == "entity")
	{
		f = eFile_Entity;
		args.FilePath_ = fn;
	}
	//
	EventManager::GetInstance().fireEvent(eEvent_SelectModel, args);
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
