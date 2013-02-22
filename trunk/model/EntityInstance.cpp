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
#include "ParticleEmitter.h"
#include "font/FontManager.h"
#include "font/FreeType.h"
#include "misc/Logger.h"
void EntityInstance::render()
{
	getRenderContex()->getDxDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	getRenderContex()->getDxDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
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
				m.make(Position_, Scale_, AngleY_);
				ef->SetMatrix(w, &m);
			}
			//
			std::vector<Matrix> matrices;
			if(SkinCurrent_ && !MatricesSkin_.empty())
			{
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
			tl->apply(MatricesMaterial_[i], ColorsCurrent_[i]);
			if (getFxManager()->getShaderType() == eFx_Shader0)
			{
				ef->CommitChanges();
				for (int p = 0; p != passes; ++p)
				{
					ef->BeginPass(p);
					ms->render(matrices);
					ef->EndPass();
				}
			}
			else
			{
				if (!matrices.empty())
				{
					ef->SetMatrixArray("gSkinPalette", &matrices[0], matrices.size());
				}
				ef->CommitChanges();
				for (int p = 0; p != passes; ++p)
				{
					ef->BeginPass(p);
					ms->render();
					ef->EndPass();
				}
			}
			tl->cancel();
		}
		ef->End();
	}
	//
	getRenderContex()->getDxDevice()->SetVertexShader(NULL);
	getRenderContex()->getDxDevice()->SetPixelShader(NULL);
	for (size_t i = 0; i != Particles_.size(); ++i)
	{
		ParticleCluster& c = Particles_[i];
		c.render();
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
	//
	Angles_.resize(Materials_.size());
	AnglesCurrent_.resize(Materials_.size(), 0.0f);
	Us_.resize(Materials_.size());
	UsCurrent_.resize(Materials_.size(), 0.0f);
	Vs_.resize(Materials_.size());
	VsCurrent_.resize(Materials_.size(), 0.0f);
	Alphas_.resize(Materials_.size());
	AnglesCurrent_.resize(Materials_.size(), 1.0f);
	Colors_.resize(Materials_.size());
	ColorsCurrent_.resize(Materials_.size(), Vector4::One);
	//
	Rows_.resize(Materials_.size(), 1.0f);
	Cols_.resize(Materials_.size(), 1.0f);
	UVSequenceTimer_.resize(Materials_.size(), 0.0f);
	//
	MatricesMaterial_.resize(Materials_.size(), Matrix::Identity);
	//
	for (size_t i = 0; i != Materials_.size(); ++i)
	{
		Material* m = Materials_[i];
		Angles_[i].init(&m->RotationKFs_, 0.0f);
		Us_[i].init(&m->FlowUKFs_, 0.0f);
		Vs_[i].init(&m->FlowVKFs_, 0.0f);
		Alphas_[i].init(&m->AlphaKFs_, 1.0f);
		Colors_[i].init(&m->ColorKFs_, 1.0f);
	}
	//
	for (size_t i = 0; i != Entity_->getParticleNumber(); ++i)
	{
		ParticleEmitter* p = ModelResManager::getInstance()->get<ParticleEmitter>(Entity_->getParticleName(i));
		p->addReference();
		ParticleCluster c;
		c.create(p);
		if (!Skeleton_->_matrices.empty())
		{
			c.setBoneID(Skeleton_->getBoneID(Entity_->getParticleBoneName(i)));
		}
		Particles_.push_back(c);
	}
	return true;
}

void EntityInstance::destroy()
{
	if (Entity_)
	{
		Entity_->removeReference();
		Entity_ = NULL;
	}
	
	//
	if (Skeleton_)
	{
		Skeleton_->removeReference();
		Skeleton_ = NULL;
	}
	
	//
	for (size_t i = 0; i != Parts_.size(); ++i)
	{
		Part* p = Parts_[i];
		p->removeReference();
		Meshes_[i]->removeReference();
		Materials_[i]->removeReference();
		Angles_[i].clear();
		Us_[i].clear();
		Vs_[i].clear();
		Alphas_[i].clear();
		Colors_[i].clear();
	}
	Parts_.clear();
	Meshes_.clear();
	Materials_.clear();
	//
	UVSequenceTimer_.clear();
	Cols_.clear();
	Rows_.clear();
	//
	if (SkinCurrent_)
	{
		SkinCurrent_->removeReference();
		SkinCurrent_ = NULL;
	}
	//
	for (size_t i = 0; i != Particles_.size(); ++i)
	{
		Particles_[i].getEmitter()->removeReference();
	}
	Particles_.clear();
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
	delta *= Speed_;
	//skin
	if (SkinCurrent_)
	{
		AnimationTime_.update(delta);
		/*{
			if (FontManager::getPointer()->getFont())
			{
				std::ostringstream ss;
				ss<<"begin = "<<AnimationTime_.start<<std::endl;
				ss<<"end = "<<AnimationTime_.end<<std::endl;
				ss<<"current = "<<AnimationTime_.current<<std::endl;
				FontManager::getPointer()->getFont()->render(Vector2(10, 30), Vector4(1, 0, 0, 1), ss.str());
			}
		}*/
		Skeleton_->update(AnimationTime_, SkinCurrent_);
		//
		MatricesSkin_.clear();
		MatricesSkin_.resize(Skeleton_->_matrices.size());
		if (!Skeleton_->_matrices.empty())
		{
			memcpy(&MatricesSkin_[0], &Skeleton_->_matrices[0], Skeleton_->_matrices.size() * sizeof(Matrix));
			/*for (size_t i = 0; i != MatricesSkin_.size(); ++i)
			{
				Matrix& m = MatricesSkin_[i];
				std::ostringstream ss;
				ss<<"current = "<<AnimationTime_.current<<"</br>";
				ss<<m[0][0]<<"  "<<m[0][1]<<"  "<<m[0][2]<<"  "<<m[0][3]<<"</br>";
				ss<<m[1][0]<<"  "<<m[1][1]<<"  "<<m[1][2]<<"  "<<m[1][3]<<"</br>";
				ss<<m[2][0]<<"  "<<m[2][1]<<"  "<<m[2][2]<<"  "<<m[2][3]<<"</br>";
				ss<<m[3][0]<<"  "<<m[3][1]<<"  "<<m[3][2]<<"  "<<m[3][3]<<"</br>";
				if (m[3][1] > 2000)
				{
					breakable;
				}
				Record(ss);
			}*/
		}
	}
	//material
	updateMaterial_(delta);
	//
	//
	for (size_t i = 0; i != Particles_.size(); ++i)
	{
		ParticleCluster& c = Particles_[i];
		if (Skeleton_->_matrices.empty())
		{
			c.update(delta, Matrix::Identity);
		}
		else
		{
			c.update(delta, Skeleton_->_matricesFull[c.getBoneID()]);
		}
	}
}
void EntityInstance::updateMaterial_( float delta )
{
	//
	for (size_t i = 0; i != Materials_.size(); ++i)
	{
		Material* tl = Materials_[i];
		if (NULL == tl)
		{
			continue;
		}
		//
		Matrix m = Matrix::Identity;
		if (tl->mCols > 1 || tl->mRows > 1)
		{
			UVSequenceTimer_[i] += delta;
			if (UVSequenceTimer_[i] >= tl->mChangeInterval)
			{
				UVSequenceTimer_[i] = 0.0f;
				switch(tl->mChangeStyle)
				{
				case 0:
					{
						Rows_[i]++;
						if (Rows_[i] >= tl->mRows)
						{
							Rows_[i] = 0;
							Cols_[i]++;
						}
						if (Cols_[i] >= tl->mCols)
						{
							Cols_[i] = 0;
						}
					}break;
				case 1:
					{

					}break;
				}
			}
			//
			float su = 1.0f / tl->mRows;
			float sv = 1.0f / tl->mCols;
			m._11 = su; m._22 = sv;
			m._31 = su * Rows_[i]; m._32 = sv * Cols_[i];
			MatricesMaterial_[i] = m;
		}
		else if(!Angles_[i].empty() || !Us_[i].empty() || !Vs_[i].empty())
		{
			{
				Angles_[i].update(delta);
				AnglesCurrent_[i] += 0.0001 * delta * Angles_[i].getValue(); 
				float c = cos(AnglesCurrent_[i]);
				float s = sin(AnglesCurrent_[i]);
				m._11 = c;m._12 = s;
				m._12 = -s;m._22 = c;
			}
			{
				Us_[i].update(delta);
				UsCurrent_[i] += 0.0001 * delta * Us_[i].getValue();
			}
			{
				Vs_[i].update(delta);
				VsCurrent_[i] += 0.0001 * delta * Vs_[i].getValue(); 
			}
			Matrix n = Matrix::Identity;
			n._31 = -0.5f;n._32 = -0.5f;
			Matrix ni = Matrix::Identity;
			ni._31 = 0.5f + UsCurrent_[i];
			ni._32 = 0.5f + VsCurrent_[i];
			MatricesMaterial_[i] = n * m * ni;
		}
		//
		ColorsCurrent_[i] = Vector4::One;
		if(!Alphas_[i].empty()){
			Alphas_[i].update(delta);
			ColorsCurrent_[i].w = Alphas_[i].getValue();
		}
		if(!Colors_[i].empty()){
			Colors_[i].update(delta);
			Vector3 c = Colors_[i].getValue();
			ColorsCurrent_[i].x = c.x;
			ColorsCurrent_[i].y = c.y;
			ColorsCurrent_[i].z = c.z;
		}
	}
}
void EntityInstance::setSpeed( float s )
{
	Speed_ = s;
}

void EntityInstance::renderImpT0()
{

}

void EntityInstance::renderImpT2()
{

}

void EntityInstance::setPosition( const Vector3& p )
{
	Position_ = p;
}

void EntityInstance::rotateY( float p )
{
	AngleY_ = p;
}

void EntityInstance::scale( const Vector3& p )
{
	Scale_ = Scale_ * p;
}

EntityInstance* EntityInstance::clone() const
{
	EntityInstance* e = new EntityInstance;
	e->create(FileName_);
	return e;
}

void EntityInstance::release()
{
	destroy();
	delete this;
}

Skeleton* EntityInstance::getSkeleton()
{
	return Skeleton_;
}

std::string EntityInstance::getResId() const
{
	return FileName_;
}

Create_Singleton_Imp(EntityInstanceManager, ApiModel_)