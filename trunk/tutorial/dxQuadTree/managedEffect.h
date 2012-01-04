#pragma once
class ManagedEffect
{
public:
	ManagedEffect()
	{
		clear_();
	}
private:
	void clear_()
	{
		pEffect_ = NULL;
		hCurrentTechnique_ = 0;	
	}
public:
	ID3DXEffect* getDXEffect() 
	{
		return pEffect_;
	}
	bool ManagedEffect::load( const std::string& effectResource )
	{
		resourceID_ = effectResource;
		return D3DXCreateEffectFromFile(getRenderContex()->device(), TEXT("e:/ZenBin/data/shader/Position.fx"), NULL, NULL, D3DXSHADER_DEBUG, NULL, &pEffect_, NULL);
	}
	D3DXHANDLE getCurrentTechnique() const
	{
		return hCurrentTechnique_;
	}

private:
	ID3DXEffect* pEffect_;
	D3DXHANDLE	hCurrentTechnique_;
	std::string resourceID_;
};