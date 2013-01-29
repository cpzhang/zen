#pragma once
#ifdef StaticLibrary_ // ¾²Ì¬¿â
	#define ApiFont_
#else // ¶¯Ì¬¿â
	#if defined(ExportDynamicLibFont_) // inside DLL
	#   define ApiFont_ __declspec(dllexport)
	#else // outside DLL
	#   define ApiFont_ __declspec(dllimport)
	#endif
#endif