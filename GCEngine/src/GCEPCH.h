#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "GCE/Core/Log.h"
#include "GCE/Debug/Instrumentor.h"

#ifdef GCE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif