#pragma once
#ifdef StaticLibrary_ // ��̬��
	#define ApiMisc_
#else // ��̬��
	#if defined(ExportDynamicLibMisc_) // inside DLL
	#   define ApiMisc_ __declspec(dllexport)
	#else // outside DLL
	#   define ApiMisc_ __declspec(dllimport)
	#endif
#endif