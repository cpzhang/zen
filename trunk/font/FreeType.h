#pragma once
#include "ConfigFont.h"
#include "dependence/freetype-2.4.4/include/ft2build.h"
#include FT_FREETYPE_H
#include "misc/stdHead.h"
#include "render/vector2.h"
#include "render/vector3.h"
#include "render/vector4.h"
//
enum eFontProperty
{
	//
	eFontProperty_Normal = 0,
	eFontProperty_Offset_1,
	eFontProperty_BG_1,
	eFontProperty_Size,
};
class Texture;
class Fx;
//
struct FTex
{
	FTex()
		:_tex(NULL)
	{

	}
	Texture*		_tex;
	Vector2			_uv0;
	Vector2			_uv2;
	float			_advance;
	float			_bearingX;
	float			_bearingY;
	float			_width;
	float			_height;
};

class ApiFont_ FreeType
{
public:
	FreeType();
	~FreeType();

public:
	virtual bool create(std::string& faceFile, unsigned int fontSize, eFontProperty fontProperty);
	virtual bool destroy();
	virtual bool render(Vector3& basePoint, Vector3& direction, const Vector4& color, std::string& text);
	void onInvalidateDevice();
	void onRestoreDevice();
private:
	//
	unsigned short _computeUnicode(std::string& character);
	FTex* _parse(unsigned short unicode, bool chinese = false);
	void _addCode(unsigned short unicode, bool chinese = false);
	void _renderImpl(FTex* fft, const Vector4& color, Vector3& direction);

	//
private:
	//
	static const unsigned int	_TEXTURE_SIZE;
	unsigned int				_OFFSET_VERTICAL;
	static const float			_INVERSE_TEXTURE_SIZE;
	//
private:
	FT_Library		_library;
	FT_Face			_face;
	unsigned int	_fontSize;
	eFontProperty	_property;

	//
	typedef std::map<unsigned long, FTex*> CodeTexMap;
	CodeTexMap		_codeTex;

	//
	std::vector<Texture*>		_textures;
	Texture*		_activeTex;
	Vector2			_pen;
	float			_baseX;
	Fx*			_fx;
};