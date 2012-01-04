#pragma once
#ifdef StaticLibrary_ // ¾²Ì¬¿â
#define ApiScene_
#else // ¶¯Ì¬¿â
#if defined(ExportDynamicLibScene_) // inside DLL
#   define ApiScene_ __declspec(dllexport)
#else // outside DLL
#   define ApiScene_ __declspec(dllimport)
#endif
#endif