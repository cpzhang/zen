#pragma once
#include "misc/eventmanager.h"
// arguments
class SelectModelEventArgs : public EventArgs
{
public:
	tstring FilePath_;
	static std::string tEventName;
};
