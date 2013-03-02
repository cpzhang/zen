#pragma once
#include "ConfigMisc.h"
#include "helper.h"
#include "stdHead.h"
//
class ApiMisc_ FileSystem
{
public:
	//
	static tstring getWindowDirectory();
	static tstring getModuleFileName();
	//data目录
	static void setDataDirectory(const tstring& dir);
	static tstring getDataDirectory();
	static tstring guessDataDirectory();
	static tstring cutDataPath(const tstring& path);
	static tstring addDataDir(const tstring& path);

	//
	static tstring getBinDirectory();
	//
	static tstring getParent(const tstring& dir);
	static tstring removeParent(const tstring& dir);
	//
	static bool isFileExist(const tstring& fileName);
	static bool isDirExist(const tstring& fileName);
	//
	static tstring getFileExtension(const tstring& fileName);
	static tstring removeFileExtension(const tstring& fileName);
	//小写，无'\'，仅'/'
	static tstring standardFilePath(const tstring& path);
	//
	static void createFolder(const tstring& path);
	static tstring toWindowsFilePath(const tstring& p);
private:
	static const unsigned long Max_Path;
	static tstring dataPath_;
};