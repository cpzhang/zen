#include "TerrainTextureSetter.h"
#include "Chunk.h"
#include "render/Texture.h"
#include "render/Fx.h"
TerrainTextureSetter::TerrainTextureSetter()
{
	clear_();
}

TerrainTextureSetter::~TerrainTextureSetter()
{
	clear_();
}

void TerrainTextureSetter::setDxEffect( Fx* f)
{
	pEffect_ = f->getDxEffect();
	handles_.clear();
	handles_.push_back( pEffect_->GetParameterByName(NULL,"gLayer0") );
	handles_.push_back( pEffect_->GetParameterByName(NULL,"gLayer1") );
	handles_.push_back( pEffect_->GetParameterByName(NULL,"gLayer2") );
}

void TerrainTextureSetter::apply( Chunk* ck )
{
	for ( u32 i=0; i < eTerrainLayer_Size; i++ )		
	{
		pEffect_->SetTexture(handles_[i], ck->getLayer((eTerrainLayer)i)->getDxTexture());
	}
}

void TerrainTextureSetter::clear_()
{
	pEffect_ = NULL;
	handles_.clear();
}
