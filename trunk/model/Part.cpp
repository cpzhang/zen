#include "Part.h"
#include "tinyXML2/tinyxml2.h"
#include "Mesh.h"
#include "Material.h"
#include "misc/FileSystem.h"
//#include "MeshManager.h"
#include "Skeleton.h"
#include "Skin.h"
#include "BoneNode.h"
#include "render/math.h"
#include "Entity.h"
bool Part::create( const std::string& fileName )
{
	FilePath_ = fileName;
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(fileName.c_str()))
	{
		return false;
	}
	tinyxml2::XMLElement* r = doc.RootElement();
	if (NULL == r)
	{
		return false;
	}
	std::string parentPath = FileSystem::getParent(fileName);
	//parentPath = FileSystem::getParent(parentPath);
	parentPath += "/";
	Name_ = r->Attribute("name");
	{
		tinyxml2::XMLElement* mes = r->FirstChildElement("mesh");
		NameMesh_ = FileSystem::standardFilePath(parentPath + mes->Attribute("file"));
	}

	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("material");
		NameMaterial_ = FileSystem::standardFilePath(parentPath + mat->Attribute("file"));
	}
	
	return true;
}
//void Part::renderSkeleton()
//{
//	if (NULL == Entity_ || NULL == Entity_->getSkeleton())
//	{
//		return;
//	}
//	IDirect3DDevice9* dx = getRenderContex()->getDxDevice();
//	dx->SetRenderState(D3DRS_POINTSIZE, F2DW(8));
//	BoneNodeMapIterator it = Entity_->getSkeleton()->getCommandMapIterator();
//	while(!it.isAtEnd())
//	{
//		BoneNode* n = it.getCurrentValue();
//		BoneNode* p = n->getParentNode();
//		if (NULL != p)
//		{
//			Bone* b = n->getBone();
//			Bone* f = p->getBone();
//			//
//			Vector3 pt[2];
//			pt[0] = Entity_->getSkeleton()->_matricesFull[b->id].applyVector(Vector3::Zero);
//			pt[1] = Entity_->getSkeleton()->_matricesFull[f->id].applyVector(Vector3::Zero);
//			dx->DrawPrimitiveUP(D3DPT_LINELIST, 1, pt, sizeof(Vector3));
//			dx->DrawPrimitiveUP(D3DPT_POINTLIST, 2, pt, sizeof(Vector3));
//		}
//		else
//		{
//			Bone* b = n->getBone();
//			Vector3 pt[2];
//			pt[0] = Entity_->getSkeleton()->_matricesFull[b->id].applyVector(Vector3::Zero);
//			dx->DrawPrimitiveUP(D3DPT_POINTLIST, 1, pt, sizeof(Vector3));
//		}
//		//
//		++it;
//	}
//	//
//	it.toStart();
//	//
//	//Mat4 vm, pm;
//	//fx->getMatrix("gView", &vm);
//	//fx->getMatrix("gProjection", &pm);
//	//while(!it.isAtEnd())
//	//{
//	//	BoneNode* n = it.getCurrentValue();
//	//	{
//	//		Bone* b = n->getBone();
//	//		//
//	//		Vector3 p;
//	//		RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->objectSpace2ScreenSpace(&p, &p, NULL, &pm, &vm, &mSkeleton->_matricesFull[b->id]);
//	//		//
//	//		{
//	//			RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont("freeNormal")->render(p, Vec3(1, 0, 0), Zen::Color::Red, b->name);
//	//		}
//	//	}
//	//	//
//	//	++it;
//	//}
//}
//void Part::render()
//{
//	if (NULL == Material_ || NULL == Material_->getFx())
//	{
//		return;
//	}
//	ID3DXEffect* ef = Material_->getFx()->getDxEffect();
//	if (NULL == ef)
//	{
//		return;
//	}
//	//Handles provide an efficient means for referencing effect parameters, techniques, passes, and annotations with an effect. 
//	D3DXHANDLE p = ef->GetParameterByName(NULL,Fx::getProjectionMatrixName());
//	D3DXHANDLE v = ef->GetParameterByName(NULL,Fx::getViewMatrixName());
//	D3DXHANDLE w = ef->GetParameterByName(NULL,Fx::getWorldMatrixName());	
//	//
//	UINT passes = 0;
//	if (getFxManager()->getShaderType() == eFx_Shader0)
//	{
//		ef->SetTechnique("T0");
//	}
//	else
//	{
//		ef->SetTechnique("T2");
//	}
//	ef->Begin(&passes, D3DXFX_DONOTSAVESTATE);
//	if (passes > 0)
//	{
//		if (v)
//		{
//			ef->SetMatrix(v, &getRenderContex()->getViewMatrix());
//		}
//		if (p)
//		{
//			ef->SetMatrix(p, &getRenderContex()->getProjectionMatrix());
//		}
//		if (w)
//		{
//			Matrix m = Matrix::Identity;
//// 			Matrix v = getRenderContex()->getViewMatrix();
//// 			v.invert();
//// 			m[0][0] = v[0][0];m[0][1] = v[0][1];m[0][2] = v[0][2];
//// 			m[1][0] = v[1][0];m[1][1] = v[1][1];m[1][2] = v[1][2];
//// 			m[2][0] = v[2][0];m[2][1] = v[2][1];m[2][2] = v[2][2];
//			m.setScale(0.01f, 0.01f, 0.01f);
//			ef->SetMatrix(w, &m);
//		}
//		std::vector<Matrix> matrices;
//		if (Entity_ && Entity_->getSkeleton())
//		{
//			for(BoneIDReferenceMap::iterator it = Mesh_->_bones.begin(); it != Mesh_->_bones.end(); ++it)
//			{
//				matrices.push_back(Entity_->getSkeleton()->_matrices[it->first]);
//			}
//		}
//		else
//		{
//			for(BoneIDReferenceMap::iterator it = Mesh_->_bones.begin(); it != Mesh_->_bones.end(); ++it)
//			{
//				matrices.push_back(Matrix::Identity);
//			}
//		}
//		if (!matrices.empty())
//		{
//			ef->SetMatrixArray("gSkinPalette", &matrices[0], matrices.size());
//		}
//		Material_->apply();
//		ef->CommitChanges();
//		for (int p = 0; p != passes; ++p)
//		{
//			ef->BeginPass(p);
//			Mesh_->render();
//			ef->EndPass();
//		}
//		Material_->cancel();
//	}
//	ef->End();
//	//
//	//
//	//renderSkeleton();
//}

void Part::clear_()
{
	Name_.clear();
}

Part::Part()
{
	clear_();
}

void Part::destroy()
{

}

const tstring& Part::getMesh()
{
	return NameMesh_;
}

const tstring& Part::getMaterial()
{
	return NameMaterial_;
}

const tstring& Part::getFilePath()
{
	return FilePath_;
}
//Create_Singleton_Imp(PartManager, ApiModel_)