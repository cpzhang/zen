#pragma once
#ifdef StaticLibrary_ // ¾²Ì¬¿â
	#define ApiLuaScript_
#else // ¶¯Ì¬¿â
	#if defined(ExportDynamicLibLuaScript_) // inside DLL
	#   define ApiLuaScript_ __declspec(dllexport)
	#else // outside DLL
	#   define ApiLuaScript_ __declspec(dllimport)
	#endif
#endif