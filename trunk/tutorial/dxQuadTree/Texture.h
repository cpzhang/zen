#pragma once
class Texture
{
public:
	Texture()
	{
	}
	~Texture()
	{
	}
public:
		virtual IDirect3DTexture9*	getDxTexture( ) = 0;
private:
};

