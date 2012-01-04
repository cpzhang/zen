#include "ParticleCluster.h"
#include "ParticleEmitter.h"
#include "render/TextureManager.h"
//#include "Material.h"
//#include "Effect.h"
//#include "TextureManager.h"
//#include "Texture.h"
//#include "Camera.h"

	ParticleCluster::ParticleCluster()
	{
		_clear();
	}

	ParticleCluster::~ParticleCluster()
	{
		_clear();
	}

	void ParticleCluster::render()
	{
// 		mMaterial->apply();
// 		Effect* fx = mMaterial->getEffect();
// 		fx->setTexture("g_MeshTexture", mTexture);
// 		fx->setMatrix("g_mWorldViewProjection", mPVW);
// 		//
// 		ParticleList::iterator it;
// 		for (it = mParticles.begin(); it != mParticles.end(); ++it)
// 		{
// 			u32 passes = 0;
// 			fx->begin(&passes);
// 			for (u32 i = 0; i != passes; ++i)
// 			{
// 				fx->beginPass(i);
// 				it->render();
// 				fx->endPass();
// 			}
// 			fx->end();
// 		}
	}

	void ParticleCluster::update(float delta, const AnimationTime& at)
	{
		//
		ParticleList::iterator it;
		for (it = mParticles.begin(); it != mParticles.end(); ++it)
		{
			it->update(delta);
		}

		//
		for (it = mParticles.begin(); it != mParticles.end(); )
		{
			Particle& p = *it;
			if (!p.isAlive())
			{
				it = mParticles.erase(it);
			}
			else
			{
				++it;
			}
		}

		//
		mEmitter->spawn(delta, at, mParticles);
		//
		//mPVW = cam->getProjectionMatrix() * cam->getViewMatrix();
	}

	void ParticleCluster::_clear()
	{
		mTexture = NULL;
		mEmitter = NULL;
	}

	bool ParticleCluster::create( ParticleEmitter* e )
	{
		mEmitter = e;
		if (NULL == mEmitter)
		{
			return false;
		}
		mTexture = getTextureManager()->getTexture(e->mTextureFile);
		if (NULL == mTexture)
		{
			return false;
		}
// 		mMaterial = MaterialManager::getInstancePtr()->createMaterial(eMaterialType_Vertex);
// 		if (NULL == mMaterial)
// 		{
// 			return false;
// 		}
// 		mMaterial->mVertexDeclaration = eVertexDeclarationType_PositionColorTexture;
// 		mMaterial->setEffect("shader/Particle.fx");
// 		if (NULL == mMaterial->getEffect())
// 		{
// 			return false;
// 		}
// 		mMaterial->mZEnable = eZBufferType_True;
// 		//需要alpha blend，必须关掉Z Write Enable
// 		mMaterial->mZWriteEnable = false;
// 		switch(e->mBlendMode)
// 		{
// 		case 3:
// 		case 4:
// 			{
// 				mMaterial->mSrcBlend = D3DBLEND_SRCALPHA;
// 				mMaterial->mDestBlend = D3DBLEND_ONE;
// 				mMaterial->mAlphaTestEnable = false;
// 			}
// 			break;
// 		}
		return true;
	}

	void ParticleCluster::destroy()
	{
// 		if (mTexture)
// 		{
// 			mTexture->release();
// 		}
// 		if (mMaterial)
// 		{
// 			mMaterial->destroy();
// 			delete mMaterial;
// 		}
		_clear();
	}