#pragma once
#include "render/Texture.h"
#include "misc/stdHead.h"
#include "misc/dxHead.h"
class Chunk;
class ID3DXEffect;
class Fx;
class TerrainTextureSetter
{
public:
	TerrainTextureSetter( );

	~TerrainTextureSetter();

	void setDxEffect( Fx* f);

	void apply(Chunk* ck);
private:
	void clear_();
private:
	ID3DXEffect* pEffect_;
	std::vector<D3DXHANDLE>	handles_;
};

