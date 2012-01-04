#pragma once
class TerrainRender
{
public:
	void draw()
	{
		if (blocks_.size() && renderer_)
		{
			renderer_->draw(blocks_);
		}
	}
public:
	class Renderer
	{
	public:
	public:
		void draw(BlockVector& blocks)
		{
			renderBlocks( blocks );
		}
	private:
		void renderBlocks( BlockVector& blocks )
		{
			ID3DXEffect* pEffect = material_->getManagedEffect()->getDXEffect();
			D3DXHANDLE wvp = pEffect->GetParameterByName(NULL,"worldViewProj");
			D3DXHANDLE vp = pEffect->GetParameterByName(NULL,"viewProj");
			D3DXHANDLE world = pEffect->GetParameterByName(NULL,"world");	
			D3DXHANDLE tex = pEffect->GetParameterByName(NULL,"textransform");

			if (material_->begin())
			{
				BlockVector::iterator it = blocks.begin();
				BlockVector::iterator end = blocks.end();

				while (it != end)
				{
					Matrix& m = it->first;

					/* for shader technique */
					Matrix full(m);
					full.postMultiply(Moo::rc().viewProjection());
					if (wvp)
						pEffect->SetMatrix( wvp, &full );

					/* for texture stage renderer*/
					if (world)
						pEffect->SetMatrix( world, &m );
					Matrix tt;
					this->textureTransform(m,tt);
					if (tex)
						pEffect->SetMatrix( tex, &tt );

					if (vp)
						pEffect->SetMatrix( vp, &Moo::rc().viewProjection() );

					pEffect->CommitChanges();

					for (uint32 i=0; i<material_->nPasses(); i++)
					{
						if (material_->beginPass(i))
						{
							it->second->draw(textureSetter_);
							material_->endPass();
						}
					}

					it++;			
				}				

				material_->end();		
			}
			blocks.clear();

			getRenderContex()->getDxDevice()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			getRenderContex()->getDxDevice()->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		}
		void setInitialRenderStates()
		{
			getRenderContex()->getDxDevice()->setVertexDeclaration( pDecl_);
			ID3DXEffect* pEffect = material_->getManagedEffect()->getDXEffect()
			SAFE_SET( pEffect, Matrix, "view", &getRenderContex()->getViewMatrix() );
			SAFE_SET( pEffect, Matrix, "proj", &getRenderContex()->getProjectionMatrix());
		}
	private:
		IDirect3DVertexDeclaration9*	pDecl_;
		EffectMaterial*		material_;

	};
private:
	typedef std::pair<Matrix, TerrainBlock*> BlockInPlace;
	typedef std::vector<BlockInPlace>	BlockVector;	
	BlockVector blocks_;
	Renderer* renderer_;
};