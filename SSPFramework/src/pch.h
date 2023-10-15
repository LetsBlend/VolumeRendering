#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <codecvt>

#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <map>

#ifdef PLATFROM_WINDOWS
//Windows
#include <Windows.h>
#include <wrl.h> //-> Microsoft Com

//DirectX
#include <d3d11.h>
#include <dxgi1_3.h>
#include <d3dcompiler.h>
#endif

// Mine
#include "Log.h"