#pragma once

#include "Pine/Core/PlatformDetection.h"

#ifdef HZ_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Pine/Core/Core.h"

#include "Pine/Core/Log.h"

#include "Pine/Debug/Instrumentor.h"

#ifdef PN_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
