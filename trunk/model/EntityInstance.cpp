#include "EntityInstance.h"
#include "tinyXML2/tinyxml2.h"
#include "misc/FileSystem.h"
#include "Entity.h"
#include "Skeleton.h"
#include "Skin.h"
#include "ModelResManager.h"
#include "Material.h"
#include "render/Fx.h"
#include "render/FxManager.h"
#include "render/rendercontext.h"
void EntityInstance::render()
{
	for (size_t i = 0; i != Parts_.size(); ++i)
	{
		Mesh* ms = Meshes_[i];
		Material* tl = Materials_[i];
		if (NULL == ms || NULL == tl || NULL == tl->getFx() || NULL == tl->getFx()->getDxEffect())
		{
			continue;
		}
		ID3DXEffect* ef = tl->getFx()->getDxEffect();
		//
		if (getFxManager()->getShaderType() == eFx_Shader0)
		{
			ef->SetTechnique("T0");
		}
		else
		{
			ef->SetTechnique("T2");
		}
		//Handles provide an efficient means for referencing effect parameters, techniques, passes, and annotations with an effect. 
		D3DXHANDLE p = ef->GetParameterByName(NULL,Fx::getProjectionMatrixName());
		D3DXHANDLE v = ef->GetParameterByName(NULL,Fx::getViewMatrixName());
		D3DXHANDLE w = ef->GetParameterByName(NULL,Fx::getWorldMatrixName());	
		//
		UINT passes = 0;
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
				m.setScale(0.01f, 0.01f, 0.01f);
				ef->SetMatrix(w, &m);
			}
			//
			std::vector<Matrix> matrices;
			if(1){
				for(BoneIDReferenceMap::iterator it = ms->_bones.begin(); it != ms->_bones.end(); ++it)
				{
					matrices.push_back(MatricesSkin_[it->first]);
				}
			}
			else
			{
				for(BoneIDReferenceMap::iterator it = ms->_bones.begin(); it != ms->_bones.end(); ++it)
				{
					matrices.push_back(Matrix::Identity);
				}
			}
			if (!matrices.empty())
			{
				ef->SetMatrixArray("gSkinPalette", &matrices[0], matrices.size());
			}
			tl->apply();
			ef->CommitChanges();
			for (int p = 0; p != passes; ++p)
			{
				ef->BeginPass(p);
				ms->render();
				ef->EndPass();
			}
			tl->cancel();
		}
		ef->End();
	}
}

bool EntityInstance::create( const tstring& resourceId )
{
	//
	Entity_ = ModelResManager::getInstance()->get<Entity>(resourceId);
	Entity_->addReference();
	FileName_ = resourceId;
	for (size_t i = 0; i != Entity_->getPartNumber(); ++i)
	{
		Part* p = ModelResManager::getInstance()->get<Part>(Entity_->getPart(i));
		p->addReference();
		Parts_.push_back(p);
		//
		Mesh* m = ModelResManager::getInstance()->get<Mesh>(p->getMesh());
		m->addReference();
		Meshes_.push_back(m);
		//
		Material* t = ModelResManager::getInstance()->get<Material>(p->getMaterial());
		t->addReference();
		Materials_.push_back(t);
	}
	//
	Skeleton_ = ModelResManager::getInstance()->get<Skeleton>(Entity_->getSkeleton());
	return true;
}

void EntityInstance::destroy()
{
	Entity_->removeReference();
	Entity_ = NULL;
	//
	Skeleton_->removeReference();
	Skeleton_ = NULL;
	//
	for (size_t i = 0; i != Parts_.size(); ++i)
	{
		Part* p = Parts_[i];
		p->removeReference();
		Meshes_[i]->removeReference();
		Materials_[i]->removeReference();
	}
	Parts_.clear();
	Meshes_.clear();
	Materials_.clear();
	//
	if (SkinCurrent_)
	{
		SkinCurrent_->removeReference();
		SkinCurrent_ = NULL;
	}
}

void EntityInstance::setAnimation( const tstring& resourceId )
{
	if (Skeleton_)
	{
		if (SkinCurrent_)
		{
			SkinCurrent_->removeReference();
		}
		const tstring& res = Skeleton_->getSkin(resourceId, AnimationTime_);
		SkinCurrent_ = ModelResManager::getInstance()->get<Skin>(res);
	}
}

void EntityInstance::update( float delta )
{
	if (SkinCurrent_)
	{
		AnimationTime_.update(delta * Speed_);
		Skeleton_->update(AnimationTime_, SkinCurrent_);
		//
		MatricesSkin_.resize(Skeleton_->_matrices.size());
		memcpy(&MatricesSkin_[0], &Skeleton_->_matrices[0], Skeleton_->_matrices.size() * sizeof(Matrix));
	}
}

void EntityInstance::setSpeed( float s )
{
	Speed_ = s;
}

Create_Singleton_Imp(EntityInstanceManager, ApiModel_)