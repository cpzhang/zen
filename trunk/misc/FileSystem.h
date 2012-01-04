#pragma once
#include "ConfigMisc.h"
#include "helper.h"
#include "stdHead.h"
//
class ApiMisc_ FileSystem
{
public:
	static void getWindowDirectory(tstring& dir);
	static void getModuleFileName(tstring& file);
	static void getDataDirectory(tstring& dir);
	static void setDataDirectory(const tstring& dir);
	static tstring getDataDirectory();
	static void getBinDirectory(tstring& dir);
	static tstring getParent(const tstring& dir);
	static tstring removeParent(const tstring& dir);
	//
	static bool isFileExist(tstring& fileName);
	static tstring getFileExtension(const tstring& fileName);
	static tstring removeFileExtension(const tstring& fileName);
	static tstring standardFilePath(tstring& path);
	static void createFolder(tstring& path);
	//
	static tstring cutDataPath(const tstring& path);
private:
	static const unsigned long Max_Path;
	static tstring dataPath_;
};