#pragma once
#include "Texture.h"
class TerrainTextureSetter
{
public:
	TerrainTextureSetter( ID3DXEffect* pEffect )
	{			
		setDxEffect( pEffect );
	}

	~TerrainTextureSetter()
	{
		pEffect_ = NULL;
	}

	void setDxEffect( ID3DXEffect* pEffect )
	{
		pEffect_ = pEffect;
		handles_.clear();
		handles_.push_back( pEffect->GetParameterByName(NULL,"Layer0") );
		handles_.push_back( pEffect->GetParameterByName(NULL,"Layer1") );
		handles_.push_back( pEffect->GetParameterByName(NULL,"Layer2") );
		handles_.push_back( pEffect->GetParameterByName(NULL,"Layer3") );		
	}

	void setTextures(const std::vector<Texture*>& textures )
	{
		for ( uint32 i=0; i<textures.size() && i<handles_.size(); i++ )		
		{
			pEffect_->SetTexture(handles_[i], textures[i]->getDxTexture());
		}
		pEffect_->CommitChanges();
	}	

private:
	ID3DXEffect* pEffect_;
	std::vector<D3DXHANDLE>	handles_;
};

