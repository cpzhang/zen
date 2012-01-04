#pragma once
#include "Singleton.h"
#include "Texture.h"
class TextureManager : public Singleton<TextureManager>
{
public:
public:
	typedef std::map< std::string, Texture* > TextureMap;
public:
	Texture* getTexture( const std::string & resourceID)
	{
		Texture* t = find_(resourceID);
		if (t)
		{
			return t;
		}
		t = create_(resourceID);
		return t;
	}
private:
	void add_( Texture* pTexture, std::string resourceID )
	{
		textures_.insert( std::make_pair(resourceID, pTexture ) );
	}

	void create_(const std::string & resourceID)
	{
		ManagedTexture* mt = new ManagedTexture;
		if (mt->load(resourceID))
		{
			add_(mt, resourceID);
		}
		return mt;
	}
	Texture* find_( const std::string & resourceID )
	{
		TextureMap::iterator it;
		it = textures_.find( resourceID );
		if (it == textures_.end())
			return NULL;

		return Texture(it->second);
	}
private:
	TextureMap				textures_;
};