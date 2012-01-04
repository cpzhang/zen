#pragma once
#ifdef StaticLibrary_ // ¾²Ì¬¿â
	#define ApiMisc_
#else // ¶¯Ì¬¿â
	#if defined(ExportDynamicLibMisc_) // inside DLL
	#   define ApiMisc_ __declspec(dllexport)
	#else // outside DLL
	#   define ApiMisc_ __declspec(dllimport)
	#endif
#endif