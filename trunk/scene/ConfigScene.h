#pragma once
#ifdef StaticLibrary_ // ��̬��
#define ApiScene_
#else // ��̬��
#if defined(ExportDynamicLibScene_) // inside DLL
#   define ApiScene_ __declspec(dllexport)
#else // outside DLL
#   define ApiScene_ __declspec(dllimport)
#endif
#endif