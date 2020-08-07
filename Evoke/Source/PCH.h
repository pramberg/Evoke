#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <iostream>
#include <utility>
#include <algorithm>
#include <memory>
#include <functional>
#include <filesystem>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <cstdint>
#include <type_traits>

#include "Core/Log.h"
#include "Core/InputIDs.h"

#include <glm/glm.hpp>

#ifdef EV_PLATFORM_WINDOWS
#include <Windows.h>
#endif
