#include "Part.h"
#include "tinyXML2/tinyxml2.h"
#include "Mesh.h"
#include "Material.h"
#include "misc/FileSystem.h"
//#include "MeshManager.h"
#include "Skeleton.h"
#include "Material.h"
#include "render/Fx.h"
#include "render/FxManager.h"
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
	parentPath = FileSystem::getParent(parentPath);
	parentPath += "/";
	Name_ = r->Attribute("name");
	{
		tinyxml2::XMLElement* mes = r->FirstChildElement("mesh");
		Mesh_ = getMeshManager()->get(FileSystem::standardFilePath(parentPath + mes->Attribute("file")));
	}

	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("material");
		Material_ = getMaterialManager()->get(FileSystem::standardFilePath(parentPath + mat->Attribute("file")));
	}
	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("skeleton");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			Skeleton_ = getSkeletonManager()->get(parentPath + skeleton);
		}
	}
	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("boneMapping");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			//mGeometry->setBoneMapping(parentPath + skeleton);
		}
	}
	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("animation");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			//mGeometry->setAnimation(parentPath + skeleton);
		}
	}
	return true;
}

void Part::render()
{
	if (NULL == Material_ || NULL == Material_->getFx())
	{
		return;
	}
	ID3DXEffect* ef = Material_->getFx()->getDxEffect();
	if (NULL == ef)
	{
		return;
	}
	//Handles provide an efficient means for referencing effect parameters, techniques, passes, and annotations with an effect. 
	D3DXHANDLE p = ef->GetParameterByName(NULL,Fx::getProjectionMatrixName());
	D3DXHANDLE v = ef->GetParameterByName(NULL,Fx::getViewMatrixName());
	D3DXHANDLE w = ef->GetParameterByName(NULL,Fx::getWorldMatrixName());	
	//
	UINT passes = 0;
	if (getFxManager()->getShaderType() == eFx_Shader0)
	{
		ef->SetTechnique("T0");
	}
	else
	{
		ef->SetTechnique("T2");
	}
	ef->Begin(&passes, D3DXFX_DONOTSAVESTATE);
	if (passes > 0)
	{
		if (v)
		{
			ef->SetMatrix(v, &getRenderContex()->getViewMatrix());
		}
		if (p)
		{
			ef->SetMatrix(p, &getRenderContex()->getProjectionMatrix());
		}
		if (w)
		{
			Matrix m = Matrix::Identity;
// 			Matrix v = getRenderContex()->getViewMatrix();
// 			v.invert();
// 			m[0][0] = v[0][0];m[0][1] = v[0][1];m[0][2] = v[0][2];
// 			m[1][0] = v[1][0];m[1][1] = v[1][1];m[1][2] = v[1][2];
// 			m[2][0] = v[2][0];m[2][1] = v[2][1];m[2][2] = v[2][2];
			m.setScale(0.01f, 0.01f, 0.01f);
			ef->SetMatrix(w, &m);
		}
		Material_->apply();
		ef->CommitChanges();
		for (int p = 0; p != passes; ++p)
		{
			ef->BeginPass(p);
			Mesh_->render();
			ef->EndPass();
		}
	}
	ef->End();
}

void Part::clear_()
{
	Name_.clear();
	Mesh_ = NULL;
	Material_ = NULL;
	Skeleton_ = NULL;
}

Part::Part()
{
	clear_();
}

void Part::destroy()
{

}

Mesh* Part::getMesh()
{
	return Mesh_;
}

Material* Part::getMaterial()
{
	return Material_;
}

tstring Part::getFilePath()
{
	return FilePath_;
}

Skeleton* Part::getSkeleton()
{
	return Skeleton_;
}

Create_Singleton_Imp(PartManager, ApiModel_)