#include "ParticleCluster.h"
#include "ParticleEmitter.h"
#include "render/TextureManager.h"
#include "render/rendercontext.h"
#include "BoneNode.h"
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
		//
		getRenderContex()->getDxDevice()->SetRenderState(D3DRS_ZENABLE, true);
		getRenderContex()->getDxDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
		//
		getRenderContex()->getDxDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		//
		getRenderContex()->getDxDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		getRenderContex()->getDxDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		getRenderContex()->getDxDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		//
		getRenderContex()->getDxDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		getRenderContex()->getDxDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		getRenderContex()->getDxDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		//
		getRenderContex()->getDxDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		getRenderContex()->getDxDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		//
		switch(mEmitter->mBlendMode)
		{
		case 4:
			{
				getRenderContex()->getDxDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, false);
				getRenderContex()->getDxDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				getRenderContex()->getDxDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}break;
		case 2:
			{
				getRenderContex()->getDxDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, false);
				getRenderContex()->getDxDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				getRenderContex()->getDxDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}break;
		default:
			break;
		}
		//
		Matrix m = Matrix::Identity;
		m.setScale(0.02f, 0.02f, 0.02f);
		getRenderContex()->getDxDevice()->SetTransform(D3DTS_WORLD, &m);
		getRenderContex()->getDxDevice()->SetTransform(D3DTS_VIEW, &getRenderContex()->getViewMatrix());
		getRenderContex()->getDxDevice()->SetTransform(D3DTS_PROJECTION, &getRenderContex()->getProjectionMatrix());
		getRenderContex()->setVertexDeclaration(sVDT_PositionColorTexture::getType());
		getRenderContex()->getDxDevice()->SetTexture(0, mTexture->getDxTexture());
 		ParticleList::iterator it;
 		for (it = mParticles.begin(); it != mParticles.end(); ++it)
 		{
// 			u32 passes = 0;
// 			fx->begin(&passes);
// 			for (u32 i = 0; i != passes; ++i)
// 			{
// 				fx->beginPass(i);
 				it->render();
// 				fx->endPass();
// 			}
// 			fx->end();
 		}
		//
		getRenderContex()->getDxDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	void ParticleCluster::update( float delta, const Matrix& m )
	{
		Matrix view = getRenderContex()->getViewMatrix();
		view.invert();
		Vector3 forward = view.applyToUnitAxisVector( 2 );
		Vector3 up = view.applyToUnitAxisVector( 1 );
		Vector3 right = view.applyToUnitAxisVector( 0 );

		//
		ParticleList::iterator it;
		for (it = mParticles.begin(); it != mParticles.end(); ++it)
		{
			it->update(delta, right, up, forward);
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
		mAT.update(delta * 0.001f);
		mEmitter->spawn(delta, mAT, mParticles);
		//
		//mPVW = cam->getProjectionMatrix() * cam->getViewMatrix();
	}

	void ParticleCluster::_clear()
	{
		mTexture = NULL;
		mEmitter = NULL;
		mBoneID = Bone::tInvalidID;
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
		mAT.end = mEmitter->mTime;
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
// 		//��Ҫalpha blend������ص�Z Write Enable
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