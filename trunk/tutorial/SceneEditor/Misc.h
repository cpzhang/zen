#pragma once
#include "misc\stdHead.h"
#include "common\atlHead.h"
#include "FreeImage/Dist/FreeImage.h"
HBITMAP generateHBitMap(const std::string& fileName, int width, int height, bool selected);
void findEntityFiles(const std::string PathSelected_, std::vector<std::string>& FilePaths, const std::string& pre);