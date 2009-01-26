#include "FileSystem.h"
#include "windowsHead.h"
//
tstring FileSystem::getWindowDirectory()
{
	tstring dir;
	dir.resize(Max_Path);
	::GetWindowsDirectory((LPTSTR)dir.c_str(), Max_Path);
	return dir;
}

tstring FileSystem::getModuleFileName()
{
	tstring file;
	file.resize(Max_Path);
	// If this parameter is NULL, GetModuleFileName retrieves the path of the executable file of the current process.
	::GetModuleFileName(NULL, (LPTSTR)(file.c_str()), Max_Path);
	return file;
}

tstring FileSystem::getDataDirectory()
{
	return dataPath_;
}
bool FileSystem::isDirExist( const tstring& fileName )
{
	DWORD fileAttr = GetFileAttributes(fileName.c_str());
	//存在，且为文件夹
	return (INVALID_FILE_ATTRIBUTES != fileAttr) && 
		(fileAttr & FILE_ATTRIBUTE_DIRECTORY);
}

bool FileSystem::isFileExist(const tstring& fileName )
{
	DWORD fileAttr = GetFileAttributes(fileName.c_str());
	//存在，且不为文件夹
	return (INVALID_FILE_ATTRIBUTES != fileAttr) && 
		!(fileAttr & FILE_ATTRIBUTE_DIRECTORY);
}

tstring FileSystem::getBinDirectory( )
{
	tstring dir = getModuleFileName();
	dir = standardFilePath(dir);
	dir = getParent(dir);
	dir = getParent(dir);
	return dir;
}

tstring FileSystem::getParent( const tstring& dir )
{
	tstring path(standardFilePath(dir));
	if (path[path.size() -1] == '\\')
	{
		path = path.substr(0, path.size() - 1);
	}
	size_t pos = path.find_last_of('\\');
	if (pos != tstring::npos)
	{
		return path.substr(0, pos);
	}
	return TEXT("");
}

tstring FileSystem::getFileExtension( const tstring& fileName )
{
	size_t pos = fileName.find_last_of('.');
	if (pos != tstring::npos)
	{
		return fileName.substr(pos + 1, fileName.size() - pos - 1);
	}
	return TEXT("");
}

tstring FileSystem::removeParent( const tstring& dir )
{
	tstring path(standardFilePath(dir));
	size_t pos = path.find_last_of('\\');
	
	if (pos != tstring::npos)
	{
		return path.substr(pos + 1, path.size() - pos - 1);
	}
	return dir;
}

tstring FileSystem::removeFileExtension( const tstring& fileName )
{
	size_t pos = fileName.find_last_of('.');
	if (pos != tstring::npos)
	{
		return fileName.substr(0, pos);
	}
	return fileName;
}
tstring FileSystem::toWindowsFilePath(const tstring& p)
{
	tstring path(p);
	for (size_t i = 0; i != path.size(); ++i)
	{
		if (path[i] == '/')
		{
			path[i] = '\\';
		}
	}
	return path;
}
tstring FileSystem::standardFilePath(const tstring& p)
{
	tstring path(p);
	for (size_t i = 0; i != path.size(); ++i)
	{
		if (path[i] == '/')
		{
			path[i] = '\\';
		}
	}
	return path;
}
void FileSystem::createFolder(tstring& path)
{
	tstring folder = standardFilePath(path);
	size_t pos = folder.find_first_of(TEXT("\\"));
	while(pos != tstring::npos)
	{
		tstring f = folder.substr(0, pos);
		CreateDirectory(f.c_str(), NULL);
		pos = folder.find_first_of(TEXT("\\"), pos + 1);
	}
}

void FileSystem::setDataDirectory( const tstring& dir )
{
	//最后带'\'
	dataPath_ = standardFilePath(dir);
}

tstring FileSystem::cutDataPath( const tstring& path )
{
	return path.substr(dataPath_.size() + 1, path.size() - dataPath_.size() - 1);
}

tstring FileSystem::guessDataDirectory()
{
	tstring d = getBinDirectory();
	d = getParent(d);
	return d + "\\data";
}

tstring FileSystem::addDataDir( const tstring& path )
{
	std::string p;
	if (!path.empty())
	{
		if (path[0] == '\\')
		{
			p = getDataDirectory() + path;
		}
		else
		{
			p = getDataDirectory() + "\\" + path;
		}
	}
	return p;
}


tstring FileSystem::dataPath_;

const unsigned long FileSystem::Max_Path = 300;