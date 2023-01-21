#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <random>
#include <cmath>
//#include <ranges>

#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "ARC/Types/Pointer.h"
#include "ARC/Types/Vector.h"
#include "ARC/Types/Delegate.h"

#include "ARC/Helpers/Helpers.h"
#include "ARC/Core/Log.h"
#include "ARC/Core/Macros.h"

#include "ARC/Profiling/Timer.h"
#ifdef ARC_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
