#pragma once
#include "misc/Singleton.h"
#include "Texture.h"
#include "misc/stdHead.h"
#include "misc/dxHead.h"
#include "misc/helper.h"
#include "ConfigRender.h"
class ApiRender_ TextureManager : public Singleton<TextureManager>
{
public:
public:
	typedef std::map< tstring, Texture* > TextureMap;
public:
	Texture* createTexture(u32 Width,
		u32 Height,
		u32 MipLevels,
		DWORD Usage,
		D3DFORMAT Format,
		D3DPOOL Pool);
	Texture* getTexture( const tstring & resourceID);
	void destroy();
private:
	void add_( Texture* pTexture, tstring resourceID );

	Texture* create_(const tstring & resourceID);
	Texture* find_( const tstring & resourceID );
private:
	TextureMap				textures_;
};

ApiRender_ TextureManager* createTextureManager();

ApiRender_ TextureManager* getTextureManager();

ApiRender_ void destroyTextureManager();