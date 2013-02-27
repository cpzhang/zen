#pragma once
#include "misc\atlHead.h"
#include <string>
#include "FreeImage/Dist/FreeImage.h"
HBITMAP generateHBitMap(const std::string& fileName, int width, int height, bool selected);