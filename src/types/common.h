#pragma once

#include "types/defines.h"

// C++
#include <string>
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


// Vendor
#ifndef _WIN32
#include "mpir/mpir-x86_64-pc-linux-gnu.h"
#include "mpir/mpirxx.h"
#else 
#include "mpir/mpir.h"
#include "mpir/mpirxx.h"
#endif 

// Deps
#include "util/all.h"
#include "util/vendor/plf_colony.hpp"
