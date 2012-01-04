#pragma once
#ifdef StaticLibrary_ // ¾²Ì¬¿â
	#define ApiRender_
#else // ¶¯Ì¬¿â
	#if defined(ExportDynamicLibRender_) // inside DLL
	#   define ApiRender_ __declspec(dllexport)
	#else // outside DLL
	#   define ApiRender_ __declspec(dllimport)
	#endif
#endif