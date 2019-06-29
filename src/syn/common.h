#pragma once

#include "syn/defines.h"

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


// External Vendored Dependencies
#include "boost/callable_traits.hpp"
#include "spdlog/spdlog.h"


// Dependencies
#include "stdext/exception.h"
#include "stdext/introspection.hpp"
#include "stdext/iterators.hpp"
#include "stdext/values.hpp"
#include "stdext/string.hpp"
#include "stdext/to_hex_string.hpp"
#include "stdext/typechange.hpp"
#include "stdext/bindmem.hpp"

#include "stdext/platform_windows.h"

#include "graph/graph.hpp"


// Internal utilities
#include "syn/vendor/plf_colony.hpp"
#include "syn/vendor/signal.hpp"