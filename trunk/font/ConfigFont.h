#pragma once
#ifdef StaticLibrary_ // ��̬��
	#define ApiFont_
#else // ��̬��
	#if defined(ExportDynamicLibFont_) // inside DLL
	#   define ApiFont_ __declspec(dllexport)
	#else // outside DLL
	#   define ApiFont_ __declspec(dllimport)
	#endif
#endif