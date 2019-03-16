#pragma once

#include "types/defines.h"

// C++
#include <string>
#include <filesystem>
#include <regex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <stack>
#include <queue>
#include <set>
#include <chrono>
#include <memory>
#include <mutex>
#include <type_traits>
#include <functional>

#ifndef _WIN32
#include <dlfcn.h>
#else 

#endif



// Deps
#include "boost/callable_traits.hpp"
#include "spdlog/spdlog.h"

// Internal utilities
#include "utility/all.h"