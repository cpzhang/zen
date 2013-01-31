#pragma once
#include "ConfigFont.h"
#include "misc/stdHead.h"
#include "FreeType.h"
//
class ApiFont_ FontManager
{
	//
public:
	static FontManager* getPointer()
	{
		static FontManager t;
		return &t;
	}
	//
	FreeType* createFont(std::string& faceFile, unsigned int fontSize, eFontProperty fp, std::string fontName);
	//
	FreeType* getFont(const std::string& fontName);
	FreeType* getFont();
	//
	void destroy();
	void onInvalidateDevice();
	void onRestoreDevice();
public:
	FontManager();
	~FontManager();
public:
	
private:
	//
	typedef stdext::hash_map<std::string, FreeType*> NameFreetypeMap;
	NameFreetypeMap				_fonts;
};