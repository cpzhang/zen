#include "FreeType.h"
#include "render/Fx.h"
#include "render/FxManager.h"
#include "render/Texture.h"
#include "render/TextureManager.h"
#include "misc/FileSystem.h"
#include "render/Colour.h"
//
FreeType::FreeType()
	: _activeTex(0), _baseX(0), _fx(0)
{
	
}

FreeType::~FreeType()
{

}

bool FreeType::create( std::string& faceFile, unsigned int fontSize, eFontProperty fontProperty )
{
	int err = 0;
	//
	err = FT_Init_FreeType(&_library);
	if (err != 0)
	{
		return false;
	}

	//
	err = FT_New_Face(_library, faceFile.c_str(), 0, &_face);
	if (err != 0)
	{
		std::string data = FileSystem::getDataDirectory() + "\\";
		data += faceFile;
		err = FT_New_Face(_library, data.c_str(), 0, &_face);
		if (err != 0)
		{
			return false;
		}
	}

	//
	//if (FT_Set_Pixel_Sizes(_face, 0, fontSize) != 0)
	//这种方式较上面的更为清晰
	if (FT_Set_Char_Size (_face, 0, FT_F26Dot6 (fontSize * 64), 96, 96) != 0)
	{
		return false;
	}

	//
	//
	_property = fontProperty;
	_fontSize = fontSize;
	_OFFSET_VERTICAL = (unsigned int)(_fontSize * 1.2) + 2;
	//
	onRestoreDevice();
	
	//
	return true;
}


bool FreeType::destroy()
{
	if (_activeTex)
	{
		_activeTex = 0;
	}
	if (_fx)
	{
		_fx->destroy();
//			delete _fx;
		_fx = 0;
	}
	for (size_t i = 0; i != _textures.size(); ++i)
	{
		Texture* t = _textures[i];
		if (t)
		{
			t->destroy();
			delete t;
		}
	}
	_textures.clear();
	_codeTex.clear();
	
	return true;
}	


bool FreeType::render( Vector2& basePoint, const Vector4& color, std::string& text )
{
	if (text.size() == 0)
	{
		return true;
	}
	//
	unsigned short unicode = 0;
	FTex* fTex = 0;
	_baseX = basePoint.x;
	unsigned int spaceOffset = _fontSize * 0.5;
	basePoint.y += _fontSize;
	//
	for (size_t i = 0; i < text.size(); ++i)
	{
		bool chinese = false;
		if (text[i] < 0)
		{
			unicode = _computeUnicode(text.substr(i, 2));
			++i;
			chinese = true;
		}
		else
		{
			unicode = _computeUnicode(text.substr(i, 1));
		}

		//
		fTex = _parse(unicode, chinese);
		if (NULL == fTex)
		{
			// '\n'
			if (unicode == 10)
			{
				basePoint.y += _fontSize;
				_baseX = basePoint.x;
			} 
			else
			{
				_baseX += spaceOffset;
			}
		}
		else
		{
			_renderImpl(fTex, color, basePoint);
		}

		//
		unicode = 0;
		fTex = 0;
	}

	//
	return true;
}

void FreeType::render()
{
	if (_caches.empty())
	{
		return;
	}
	//渲染缓存的字，根据纹理与颜色分类，渲染批次可减少很多
	getRenderContex()->setVertexDeclaration(sVDT_PositionTTexture::getType());
	u32 passes = 0;
	_fx->getDxEffect()->Begin(&passes, 0);
	for (u32 i = 0; i != passes; ++i)
	{
		_fx->getDxEffect()->BeginPass(i);
		if (i != passes - 1)
		{
			for (FontCacheMHashMap::iterator it = _caches.begin(); it != _caches.end(); ++it)
			{
				Texture* tex = it->first;
				_fx->getDxEffect()->SetTexture("g_MeshTexture", tex->getDxTexture());
				ColorVertexMHashMap& cv = it->second;
				for (ColorVertexMHashMap::iterator ju = cv.begin(); ju != cv.end(); ++ju)
				{
					Vector4 color = Colour::getVector4Normalised(ju->first);
					_fx->getDxEffect()->SetVector("g_diffuse", &color);
					//
					_fx->getDxEffect()->CommitChanges();
					//
					std::vector<sVDT_PositionTTexture>& vs = ju->second;
					getRenderContex()->drawPrimitiveUP(D3DPT_TRIANGLELIST, vs.size()/3, &vs[0], sVDT_PositionTTexture::getSize());
				}
			}
		}
		_fx->getDxEffect()->EndPass();
	}
	_fx->getDxEffect()->End();
	//
	getRenderContex()->setVertexDeclaration(eVertexDeclarationType_Null);
	//
	_caches.clear();
}

void FreeType::_renderImpl( FTex* fft, const Vector4& color, Vector2& basePoint )
{
	static unsigned short sIndices[6] = {0, 1, 2, 0, 2, 3};
	//
	//
	sVDT_PositionTTexture vertices[4];
	vertices[0].position_ = Vector3(_baseX + fft->_bearingX, basePoint.y - fft->_bearingY, 0.f);
	vertices[0].texture_ = fft->_uv0;

	vertices[1].position_ = Vector3(_baseX + fft->_bearingX + fft->_width, basePoint.y - fft->_bearingY,	0.f);
	vertices[1].texture_ = Vector2(fft->_uv2.x, fft->_uv0.y);

	vertices[2].position_ = Vector3(_baseX + fft->_bearingX + fft->_width,	basePoint.y + fft->_height - fft->_bearingY, 0.f);
	vertices[2].texture_ = fft->_uv2;

	vertices[3].position_ = Vector3(_baseX + fft->_bearingX, basePoint.y + fft->_height - fft->_bearingY, 0.f);
	vertices[3].texture_ = Vector2(fft->_uv0.x, fft->_uv2.y);
	//缓存起来
	_caches[fft->_tex][color.getARGB()].push_back(vertices[0]);
	_caches[fft->_tex][color.getARGB()].push_back(vertices[1]);
	_caches[fft->_tex][color.getARGB()].push_back(vertices[2]);
	//
	_caches[fft->_tex][color.getARGB()].push_back(vertices[0]);
	_caches[fft->_tex][color.getARGB()].push_back(vertices[2]);
	_caches[fft->_tex][color.getARGB()].push_back(vertices[3]);
	//
	_baseX += fft->_width;
}

unsigned short FreeType::_computeUnicode( std::string& character)
{
	wchar_t wc = 0;
	::MultiByteToWideChar(CP_ACP, 0, character.c_str(), -1, &wc, 1);

	//
	return wc;
}

FTex* FreeType::_parse( unsigned short unicode , bool chinese/* = false*/)
{
	CodeTexMap::iterator it = _codeTex.find(unicode);
	if (it == _codeTex.end())
	{
		_addCode(unicode, chinese);
	}
	return _codeTex[unicode];
}

void FreeType::_addCode( unsigned short unicode , bool chinese /*= false*/)
{
	//
	if (FT_Load_Char(_face, unicode,
		FT_LOAD_DEFAULT |
		FT_LOAD_MONOCHROME |
		FT_LOAD_TARGET_LIGHT |
		FT_LOAD_RENDER |
		FT_LOAD_TARGET_NORMAL |
		FT_LOAD_FORCE_AUTOHINT))
	{
		return;
	}

	//
	if ( _face->glyph->bitmap.buffer == 0)
	{
		return;
	}

	//
	FT_GlyphSlot slot = _face->glyph;
	FT_Bitmap bitmap = slot->bitmap;
	
	//
	FTex* fft = new FTex;
	fft->_width = slot->metrics.width >> 6;
	fft->_height = slot->metrics.height >> 6;
	//if (chinese)
	if(unicode > 255)
	{
		fft->_bearingX = 0;
		fft->_bearingY = slot->metrics.horiBearingY >> 6;
	} 
	else
	{
		fft->_bearingX = slot->metrics.horiBearingX >> 6;
		fft->_bearingY = slot->metrics.horiBearingY >> 6;
	}
	fft->_advance = slot->metrics.horiAdvance >> 6;
	u8* buffer = 0;

	unsigned char alphaAddon = 0;
	switch(_property)
	{
// 			
// 		case eFontProperty_BG_1:
// 			{
// 				fft->_width += 2;
// 				fft->_height -= 2;
// 			}
// 			break;
		//
	case eFontProperty_Offset_1:
		{
			fft->_bearingX += 1;
			fft->_bearingY -= 1;
		}
	case eFontProperty_Normal:
	default:
		{
			buffer = new u8[bitmap.width * bitmap.rows];
			memset(buffer, 0, sizeof(u8) * bitmap.width * bitmap.rows);
			switch (bitmap.pixel_mode)
			{
			case FT_PIXEL_MODE_MONO:
				{
					for (int i = 0; i < bitmap.rows; ++i)
					{
						unsigned char *src = bitmap.buffer + (i * bitmap.pitch);
						for (int j = 0; j < bitmap.width; ++j)
							buffer [j + i * bitmap.rows] = (src[j/8] & (0x80 >> (j & 7))) ? 0xFF : 0x00;
					}
				}
				break;
			case FT_PIXEL_MODE_GRAY:
				{
					//
					for (int i = 0; i < bitmap.rows; ++i)
						for (int j = 0; j < bitmap.width; ++j)
						{
								buffer[i * bitmap.width + j] = bitmap.buffer[i * bitmap.pitch + j];
						}
				}
			}
		}	
		break;
	}

	//
	if (NULL == _activeTex)
	{
		_activeTex = TextureManager::getInstancePtr()->createTexture(_TEXTURE_SIZE, _TEXTURE_SIZE, 1, 0, D3DFMT_A8, D3DPOOL_MANAGED);
		if (NULL == _activeTex)
		{
			return;
		}
		_textures.push_back(_activeTex);
		//
		_pen.x = 0;
		_pen.y = 0;
	}

	//
	if (_pen.x + fft->_width > _TEXTURE_SIZE)
	{
		_pen.x = 0;
		_pen.y += _OFFSET_VERTICAL;
		if (_pen.y + _OFFSET_VERTICAL > _TEXTURE_SIZE)
		{
			_activeTex = TextureManager::getInstancePtr()->createTexture(_TEXTURE_SIZE, _TEXTURE_SIZE, 1, 0, D3DFMT_A8, D3DPOOL_MANAGED);
			if (NULL == _activeTex)
			{
				return;
			}
			_textures.push_back(_activeTex);
			//
			_pen.y = 0;
		}
		else
		{
		}
	}

	int offsetX = 0;
	int offsetY = 0;
	if (_property == eFontProperty_BG_1)
	{
		for (size_t x = 0; x <= 2; ++x)
		for (size_t y = 0; y <= 2; ++y)
		{
			_activeTex->setSubData(_pen.x + x , _pen.y + y, fft->_width, fft->_height, buffer, D3DFMT_A8, fft->_width);
		}
		//
		//fft->_width += 2.0f;
		//fft->_height += 2.0f;
		offsetY = 2;
		offsetX = 2;
		//fft->_bearingX -= 1;
		//fft->_bearingY -= 1;
	}
	else
	{
		//
		_activeTex->setSubData(_pen.x, _pen.y,
			fft->_width, fft->_height,
			buffer, D3DFMT_A8, fft->_width);
	}
	//
	if (buffer)
	{
		delete[] buffer;
		buffer = 0;
	}

	//
	fft->_uv0 = Vector2(_pen.x * _INVERSE_TEXTURE_SIZE, _pen.y * _INVERSE_TEXTURE_SIZE);
	
	//
	//_pen.y += 2;

	//
	fft->_uv2 = Vector2((_pen.x + fft->_width + offsetX)  * _INVERSE_TEXTURE_SIZE, (_pen.y + fft->_height + offsetY) * _INVERSE_TEXTURE_SIZE);
	fft->_tex = _activeTex;
	_codeTex[unicode] = fft;
	//
	_pen.x += (fft->_width + offsetX);
	_pen.x += 2;
}

void FreeType::onInvalidateDevice()
{
	if (_activeTex)
	{
		_activeTex = 0;
	}
	_baseX = 0;
	_pen.x = 0;
	_pen.y = 0;
	for (size_t i = 0; i != _textures.size(); ++i)
	{
		Texture* t = _textures[i];
		if (t)
		{
			t->destroy();
			delete t;
		}
	}
	_textures.clear();
	_codeTex.clear();
}

void FreeType::onRestoreDevice()
{
	_fx = getFxManager()->getFx("shader\\Freetype.fx");
}

const unsigned int FreeType::_TEXTURE_SIZE(256);
const float FreeType::_INVERSE_TEXTURE_SIZE(1.0f / _TEXTURE_SIZE);