#include "TextureManager.h"
#include "ManagedTexture.h"
Texture* TextureManager::create_( const tstring & resourceID )
{
	Texture* mt = new ManagedTexture;
	if (mt->load(resourceID))
	{
		add_(mt, resourceID);
	}
	else
	{
		mt->destroy();
		delete mt;
		mt = Texture::getNullObject();
	}
	return mt;
}

Texture* TextureManager::find_( const tstring & resourceID )
{
	TextureMap::iterator it;
	it = textures_.find( resourceID );
	if (it == textures_.end())
		return NULL;

	return it->second;
}

void TextureManager::add_( Texture* pTexture, tstring resourceID )
{
	textures_.insert( std::make_pair(resourceID, pTexture ) );
}

void TextureManager::destroy()
{
	TextureMap::iterator it = textures_.begin();
	for (; it != textures_.end(); ++it)
	{
		Texture* t = it->second;
		if (t)
		{
			t->destroy();
			delete t;
		}
	}
	textures_.clear();
}

Texture* TextureManager::getTexture( const tstring & resourceID )
{
	Texture* t = find_(resourceID);
	if (t)
	{
		return t;
	}
	t = create_(resourceID);
	return t;
}

ApiRender_ TextureManager* createTextureManager()
{
	new TextureManager;
	return TextureManager::getInstancePtr();
}

ApiRender_ TextureManager* getTextureManager()
{
	return TextureManager::getInstancePtr();
}

ApiRender_ void destroyTextureManager()
{
	getTextureManager()->destroy();
	delete TextureManager::getInstancePtr();
}
