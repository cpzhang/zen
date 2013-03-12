#pragma once
#include "misc/eventmanager.h"
#include "render/vector3.h"
// arguments
class SelectModelEventArgs : public EventArgs
{
public:
	tstring FilePath_;
	static std::string tEventName;
};
class PlaceModelDlgPositionChangedEventArgs : public EventArgs
{
public:
	Vector3 Postion_;
	static std::string tEventName;
};
class PlaceModelDlgFileChangedEventArgs : public EventArgs
{
public:
	static std::string tEventName;
};
