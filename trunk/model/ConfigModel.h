#pragma once
#ifdef StaticLibrary_ // ¾²Ì¬¿â
	#define ApiModel_
#else // ¶¯Ì¬¿â
	#if defined(ExportDynamicLibModel_) // inside DLL
	#   define ApiModel_ __declspec(dllexport)
	#else // outside DLL
	#   define ApiModel_ __declspec(dllimport)
	#endif
#endif

const int MODEL_NAME_NODE_SIZE             = 80;

class ApiModel_ IRender
{
public:
	virtual ~IRender(){}
	virtual void render() = 0;
};