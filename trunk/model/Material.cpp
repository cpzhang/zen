#include "Material.h"
#include "misc/FileSystem.h"
#include "tinyXML2/tinyxml2.h"
#include "render/FxManager.h"
#include "render/TextureManager.h"
#include "render/Texture.h"
#include "render/rendercontext.h"
bool Material::create( const std::string& fileName )
{
	std::string name;
	std::string shaderFileName;
	std::string textureVariableName;
	std::string textureFileName;
	FilePath_ = fileName;
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(fileName.c_str()))
	{
		return false;
	}
	tinyxml2::XMLElement* r = doc.RootElement();
	if (NULL == r)
	{
		return false;
	}
	Name_ = r->Attribute("name");
	//Fx
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("Fx");
		FxFileName_ = mat->Attribute("file");
	}
	//Texture
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("Texture");
		textureVariableName = mat->Attribute("name");
		textureFileName = mat->Attribute("file");
		FileSystem::standardFilePath(textureFileName);
		std::string imagePath = FileSystem::getParent(fileName);
		imagePath = FileSystem::getParent(imagePath);
		imagePath += "/";
		if (textureFileName.find('.') != std::string::npos)
		{
			TextureFileName_ = imagePath + textureFileName;
			TextureVariableName_ = textureVariableName;
		}
	}
	//RenderState
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("RenderState");
		while(mat)
		{
			sRenderState tss;
			mat->QueryIntAttribute("state", (int*)&tss.State_);
			mat->QueryIntAttribute("value", (int*)&tss.Value_);
			RenderStates_.push_back(tss);
			mat = mat->NextSiblingElement("RenderState");
		}
	}
	//TextureStageState
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("TextureStageState");
		while(mat)
		{
			sTextureStageState tss;
			mat->QueryIntAttribute("stage", (int*)&tss.stage_);
			mat->QueryIntAttribute("type", (int*)&tss.type_);
			mat->QueryIntAttribute("value", (int*)&tss.value_);
			TextureStageStates_.push_back(tss);
			mat = mat->NextSiblingElement("TextureStageState");
		}		
	}
	return true;
}

void Material::destroy()
{

}

void Material::clear_()
{
	if (!tRSDIntialized_)
	{
		initRenderStateDefault();
	}
	Fx_ = NULL;
	//
}

Fx* Material::getFx()
{
	if (Fx_ == NULL && !FxFileName_.empty())
	{
		Fx_ = getFxManager()->getFx(FxFileName_);
	}
	return Fx_;
}

void Material::apply()
{
	IDirect3DDevice9* dx = getRenderContex()->getDxDevice();
	//RenderState，保存时，已过滤掉默认值
	for (size_t i = 0; i != RenderStates_.size(); ++i)
	{
		sRenderState& tss = RenderStates_[i];
		dx->SetRenderState(tss.State_, tss.Value_);
	}
	//Texture
	Fx_->getDxEffect()->SetTexture(TextureVariableName_.c_str(), getTextureManager()->getTexture(TextureFileName_)->getDxTexture());
	//固定管道渲染 D3DTA_CURRENT
	if (getFxManager()->getShaderType() == eFx_Shader0)
	{
		for (size_t i = 0; i != TextureStageStates_.size(); ++i)
		{
			sTextureStageState& tss = TextureStageStates_[i];
			dx->SetTextureStageState(tss.stage_, tss.type_, tss.value_);
		}
	}
}

Material::Material()
{
	clear_();
}

Material::~Material()
{

}

void Material::cancel()
{
	IDirect3DDevice9* dx = getRenderContex()->getDxDevice();
	//RenderState,恢复默认值
	for (size_t i = 0; i != RenderStates_.size(); ++i)
	{
		sRenderState& tss = RenderStates_[i];
		dx->SetRenderState(tss.State_, tRenderStateDefault_[tss.State_]);
	}
	//Texture
	Fx_->getDxEffect()->SetTexture(TextureVariableName_.c_str(), NULL);
	//固定管道渲染
	if (getFxManager()->getShaderType() == eFx_Shader0)
	{
		for (size_t i = 0; i != TextureStageStates_.size(); ++i)
		{
			sTextureStageState& tss = TextureStageStates_[i];
			if (tss.type_ == D3DTSS_COLOROP || tss.type_ == D3DTSS_ALPHAOP)
			{
				dx->SetTextureStageState(tss.stage_, tss.type_, tss.stage_ == 0 ? D3DTOP_MODULATE : D3DTOP_DISABLE);
			}
			else if (tss.type_ == D3DTSS_ALPHAOP)
			{
				dx->SetTextureStageState(tss.stage_, tss.type_, tss.stage_ == 0 ? D3DTOP_SELECTARG1 : D3DTOP_DISABLE);
			}
			else if(tss.type_ == D3DTSS_TEXCOORDINDEX)
			{
				dx->SetTextureStageState(tss.stage_, tss.type_, tss.stage_);
			}
			else
			{
				dx->SetTextureStageState(tss.stage_, tss.type_, tTextureStageStateDefault_[tss.type_]);
			}
		}
	}
}

void Material::initRenderStateDefault()
{
	if (tRSDIntialized_)
	{
		return;
	}
	tRenderStateDefault_.resize(D3DRS_BLENDOPALPHA + 1);
	tRenderStateDefault_[D3DRS_ZENABLE]                      = D3DZB_TRUE;
	tRenderStateDefault_[D3DRS_FILLMODE]                     = D3DFILL_SOLID;
	tRenderStateDefault_[D3DRS_SHADEMODE]                    = D3DSHADE_GOURAUD;
	tRenderStateDefault_[D3DRS_ZWRITEENABLE]                 = TRUE;
	tRenderStateDefault_[D3DRS_ALPHATESTENABLE]              = FALSE;
	tRenderStateDefault_[D3DRS_LASTPIXEL]                    = TRUE;
	tRenderStateDefault_[D3DRS_SRCBLEND]                     = D3DBLEND_ONE;
	tRenderStateDefault_[D3DRS_DESTBLEND]                    = D3DBLEND_ZERO;
	tRenderStateDefault_[D3DRS_CULLMODE]                     = D3DCULL_CCW;
	tRenderStateDefault_[D3DRS_ZFUNC]                        = D3DCMP_LESSEQUAL;
	tRenderStateDefault_[D3DRS_ALPHAREF]                     = 0;
	tRenderStateDefault_[D3DRS_ALPHAFUNC]                    = D3DCMP_ALWAYS;
	tRenderStateDefault_[D3DRS_DITHERENABLE]                 = FALSE;
	tRenderStateDefault_[D3DRS_ALPHABLENDENABLE]             = FALSE;
	tRenderStateDefault_[D3DRS_FOGENABLE]                    = FALSE;
	tRenderStateDefault_[D3DRS_SPECULARENABLE]               = FALSE;
	tRenderStateDefault_[D3DRS_FOGCOLOR]                     = 0;
	tRenderStateDefault_[D3DRS_FOGTABLEMODE]                 = D3DFOG_NONE;
	tRenderStateDefault_[D3DRS_FOGSTART]                     = 0.0f;
	tRenderStateDefault_[D3DRS_FOGEND]                       = 1.0f;
	tRenderStateDefault_[D3DRS_FOGDENSITY]                   = 1.0f;
	tRenderStateDefault_[D3DRS_RANGEFOGENABLE]               = FALSE;
	tRenderStateDefault_[D3DRS_STENCILENABLE]                = FALSE;
	tRenderStateDefault_[D3DRS_STENCILFAIL]                  = D3DSTENCILOP_KEEP;
	tRenderStateDefault_[D3DRS_STENCILZFAIL]                 = D3DSTENCILOP_KEEP;
	tRenderStateDefault_[D3DRS_STENCILPASS]                  = D3DSTENCILOP_KEEP;
	tRenderStateDefault_[D3DRS_STENCILFUNC]                  = D3DCMP_ALWAYS;
	tRenderStateDefault_[D3DRS_STENCILREF]                   = 0;
	tRenderStateDefault_[D3DRS_STENCILMASK]                  = 0xFFFFFFFF;
	tRenderStateDefault_[D3DRS_STENCILWRITEMASK]             = 0xFFFFFFFF;
	tRenderStateDefault_[D3DRS_TEXTUREFACTOR]                = 0xFFFFFFFF;
	tRenderStateDefault_[D3DRS_WRAP0]                        = 0;
	tRenderStateDefault_[D3DRS_WRAP1]                        = 0;
	tRenderStateDefault_[D3DRS_WRAP2]                        = 0;
	tRenderStateDefault_[D3DRS_WRAP3]                        = 0;
	tRenderStateDefault_[D3DRS_WRAP4]                        = 0;
	tRenderStateDefault_[D3DRS_WRAP5]                        = 0;
	tRenderStateDefault_[D3DRS_WRAP6]                        = 0;
	tRenderStateDefault_[D3DRS_WRAP7]                        = 0;
	tRenderStateDefault_[D3DRS_CLIPPING]                     = TRUE;
	tRenderStateDefault_[D3DRS_LIGHTING]                     = TRUE;
	tRenderStateDefault_[D3DRS_AMBIENT]                      = 0;
	tRenderStateDefault_[D3DRS_FOGVERTEXMODE]                = D3DFOG_NONE;
	tRenderStateDefault_[D3DRS_COLORVERTEX]                  = TRUE;
	tRenderStateDefault_[D3DRS_LOCALVIEWER]                  = TRUE;
	tRenderStateDefault_[D3DRS_NORMALIZENORMALS]             = FALSE;
	tRenderStateDefault_[D3DRS_DIFFUSEMATERIALSOURCE]        = D3DMCS_COLOR1;
	tRenderStateDefault_[D3DRS_SPECULARMATERIALSOURCE]       = D3DMCS_COLOR2;
	tRenderStateDefault_[D3DRS_AMBIENTMATERIALSOURCE]        = D3DMCS_MATERIAL;
	tRenderStateDefault_[D3DRS_EMISSIVEMATERIALSOURCE]       = D3DMCS_MATERIAL;
	tRenderStateDefault_[D3DRS_VERTEXBLEND]                  = D3DVBF_DISABLE;
	tRenderStateDefault_[D3DRS_CLIPPLANEENABLE]              = 0;
	tRenderStateDefault_[D3DRS_POINTSIZE]                    = 64;
	tRenderStateDefault_[D3DRS_POINTSIZE_MIN]                = 1.0f;
	tRenderStateDefault_[D3DRS_POINTSPRITEENABLE]            = FALSE;
	tRenderStateDefault_[D3DRS_POINTSCALEENABLE]             = FALSE;
	tRenderStateDefault_[D3DRS_POINTSCALE_A]                 = 1.0f;
	tRenderStateDefault_[D3DRS_POINTSCALE_B]                 = 0.0f;
	tRenderStateDefault_[D3DRS_POINTSCALE_C]                 = 0.0f;
	tRenderStateDefault_[D3DRS_MULTISAMPLEANTIALIAS]         = TRUE;
	tRenderStateDefault_[D3DRS_MULTISAMPLEMASK]              = 0xFFFFFFFF;
	tRenderStateDefault_[D3DRS_PATCHEDGESTYLE]               = D3DPATCHEDGE_DISCRETE;
	tRenderStateDefault_[D3DRS_DEBUGMONITORTOKEN]            = D3DDMT_ENABLE;
	tRenderStateDefault_[D3DRS_POINTSIZE_MAX]                = 64.0;
	tRenderStateDefault_[D3DRS_INDEXEDVERTEXBLENDENABLE]     = FALSE;
	tRenderStateDefault_[D3DRS_COLORWRITEENABLE]             = 0x0000000F;
	tRenderStateDefault_[D3DRS_TWEENFACTOR]                  = 0.0f;
	tRenderStateDefault_[D3DRS_BLENDOP]                      = D3DBLENDOP_ADD;
	tRenderStateDefault_[D3DRS_POSITIONDEGREE]               = D3DDEGREE_CUBIC;
	tRenderStateDefault_[D3DRS_NORMALDEGREE]                 = D3DDEGREE_LINEAR;
	tRenderStateDefault_[D3DRS_SCISSORTESTENABLE]            = FALSE;
	tRenderStateDefault_[D3DRS_SLOPESCALEDEPTHBIAS]          = 0;
	tRenderStateDefault_[D3DRS_ANTIALIASEDLINEENABLE]        = FALSE;
	tRenderStateDefault_[D3DRS_MINTESSELLATIONLEVEL]         = 1.0f;
	tRenderStateDefault_[D3DRS_MAXTESSELLATIONLEVEL]         = 1.0f;
	tRenderStateDefault_[D3DRS_ADAPTIVETESS_X]               = 0.0f;
	tRenderStateDefault_[D3DRS_ADAPTIVETESS_Y]               = 0.0f;
	tRenderStateDefault_[D3DRS_ADAPTIVETESS_Z]               = 1.0f;
	tRenderStateDefault_[D3DRS_ADAPTIVETESS_W]               = 0.0f;
	tRenderStateDefault_[D3DRS_ENABLEADAPTIVETESSELLATION]   = FALSE;
	tRenderStateDefault_[D3DRS_TWOSIDEDSTENCILMODE]          = FALSE;
	tRenderStateDefault_[D3DRS_CCW_STENCILFAIL]              = D3DSTENCILOP_KEEP;
	tRenderStateDefault_[D3DRS_CCW_STENCILZFAIL]             = D3DSTENCILOP_KEEP;
	tRenderStateDefault_[D3DRS_CCW_STENCILPASS]              = D3DSTENCILOP_KEEP;
	tRenderStateDefault_[D3DRS_CCW_STENCILFUNC]              = D3DCMP_ALWAYS;
	tRenderStateDefault_[D3DRS_COLORWRITEENABLE1]            = 0x0000000f;
	tRenderStateDefault_[D3DRS_COLORWRITEENABLE2]            = 0x0000000f;
	tRenderStateDefault_[D3DRS_COLORWRITEENABLE3]            = 0x0000000f;
	tRenderStateDefault_[D3DRS_BLENDFACTOR]                  = 0xffffffff;
	tRenderStateDefault_[D3DRS_SRGBWRITEENABLE]              = 0;
	tRenderStateDefault_[D3DRS_DEPTHBIAS]                    = 0;
	tRenderStateDefault_[D3DRS_WRAP8]                        = 0;
	tRenderStateDefault_[D3DRS_WRAP9]                        = 0;
	tRenderStateDefault_[D3DRS_WRAP10]                       = 0;
	tRenderStateDefault_[D3DRS_WRAP11]                       = 0;
	tRenderStateDefault_[D3DRS_WRAP12]                       = 0;
	tRenderStateDefault_[D3DRS_WRAP13]                       = 0;
	tRenderStateDefault_[D3DRS_WRAP14]                       = 0;
	tRenderStateDefault_[D3DRS_WRAP15]                       = 0;
	tRenderStateDefault_[D3DRS_SEPARATEALPHABLENDENABLE]     = FALSE;
	tRenderStateDefault_[D3DRS_SRCBLENDALPHA]                = D3DBLEND_ONE;
	tRenderStateDefault_[D3DRS_DESTBLENDALPHA]               = D3DBLEND_ZERO;
	tRenderStateDefault_[D3DRS_BLENDOPALPHA]                 = D3DBLENDOP_ADD;
	//
	tTextureStageStateDefault_.resize( D3DTSS_CONSTANT + 1);

	tTextureStageStateDefault_[D3DTSS_COLOROP]                 = D3DTOP_MODULATE;//D3DTOP_DISABLE
	tTextureStageStateDefault_[D3DTSS_COLORARG1]               = D3DTA_TEXTURE;
	tTextureStageStateDefault_[D3DTSS_COLORARG2]               = D3DTA_CURRENT;
	tTextureStageStateDefault_[D3DTSS_ALPHAOP]                 = D3DTOP_SELECTARG1;//D3DTOP_DISABLE
	tTextureStageStateDefault_[D3DTSS_ALPHAARG1]               = D3DTA_TEXTURE;
	tTextureStageStateDefault_[D3DTSS_ALPHAARG2]               = D3DTA_CURRENT;
	tTextureStageStateDefault_[D3DTSS_BUMPENVMAT00]            = 0.0;
	tTextureStageStateDefault_[D3DTSS_BUMPENVMAT01]            = 0.0;
	tTextureStageStateDefault_[D3DTSS_BUMPENVMAT10]            = 0.0;
	tTextureStageStateDefault_[D3DTSS_BUMPENVMAT11]            = 0.0;
	tTextureStageStateDefault_[D3DTSS_TEXCOORDINDEX]           = 0;//1..7
	tTextureStageStateDefault_[D3DTSS_BUMPENVLSCALE]           = 0.0;
	tTextureStageStateDefault_[D3DTSS_BUMPENVLOFFSET]          = 0.0;
	tTextureStageStateDefault_[D3DTSS_TEXTURETRANSFORMFLAGS]   = D3DTTFF_DISABLE;
	tTextureStageStateDefault_[D3DTSS_COLORARG0]               = D3DTA_CURRENT;
	tTextureStageStateDefault_[D3DTSS_ALPHAARG0]               = D3DTA_CURRENT;
	tTextureStageStateDefault_[D3DTSS_RESULTARG]               = D3DTA_CURRENT;
	tTextureStageStateDefault_[D3DTSS_CONSTANT]                = 0xffffffff;
}

tstring Material::getFilePath()
{
	return FilePath_;
}

std::vector<DWORD> Material::tTextureStageStateDefault_;

bool Material::tRSDIntialized_(FALSE);

std::vector<DWORD> Material::tRenderStateDefault_;

Create_Singleton_Imp(MaterialManager, ApiModel_)