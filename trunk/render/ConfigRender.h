#pragma once
#ifdef StaticLibrary_ // ��̬��
	#define ApiRender_
#else // ��̬��
	#if defined(ExportDynamicLibRender_) // inside DLL
	#   define ApiRender_ __declspec(dllexport)
	#else // outside DLL
	#   define ApiRender_ __declspec(dllimport)
	#endif
#endif