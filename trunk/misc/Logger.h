#pragma once
#include "ConfigMisc.h"
#include "stdHead.h"
class ApiMisc_ Logger
{
public:
	Logger();
	~Logger();

public:
	enum eLevel
	{
		eLevel_Record,
		eLevel_Warning,
		eLevel_Error
	};
public:
	static Logger* getSingletonPointer()
	{
		static Logger t;
		return &t;
	}
public:
	void add(const std::string& content, eLevel level, bool newLine = true)
	{
		return add(content.c_str(), level, newLine);
	}
	void add(const char* content, eLevel level, bool newLine = true);
	void add(std::ostringstream& content, eLevel level, bool newLine = true);
	void add(int content, eLevel level, bool newLine = true);
	void addDate();
	tstring getPath();
private:
	std::ofstream		_log;
	tstring			_path;
};

#define Record(content)\
	Logger::getSingletonPointer()->addDate();\
	Logger::getSingletonPointer()->add(__FILE__, Logger::eLevel_Record, false);\
	Logger::getSingletonPointer()->add(__LINE__, Logger::eLevel_Record);\
	Logger::getSingletonPointer()->add(content, Logger::eLevel_Record);


#define Warning(content)\
	Logger::getSingletonPointer()->addDate();\
	Logger::getSingletonPointer()->add(__FILE__, Logger::eLevel_Warning, false);\
	Logger::getSingletonPointer()->add(__LINE__, Logger::eLevel_Warning);\
	Logger::getSingletonPointer()->add(content, Logger::eLevel_Warning);


#define Error(content)\
	Logger::getSingletonPointer()->addDate();\
	Logger::getSingletonPointer()->add(__FILE__, Logger::eLevel_Error, false);\
	Logger::getSingletonPointer()->add(__LINE__, Logger::eLevel_Error);\
	Logger::getSingletonPointer()->add(content, Logger::eLevel_Error);