#pragma once
#include <vector>
#include <map>
#include <stack>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <hash_map>

#include <stdio.h>
#include <ctype.h>
#include <locale>

#include "tchar.h"
#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

typedef std::vector<tstring> tstringVec;