#pragma once
#ifdef StaticLibrary_ // ��̬��
	#define ApiLuaScript_
#else // ��̬��
	#if defined(ExportDynamicLibLuaScript_) // inside DLL
	#   define ApiLuaScript_ __declspec(dllexport)
	#else // outside DLL
	#   define ApiLuaScript_ __declspec(dllimport)
	#endif
#endif