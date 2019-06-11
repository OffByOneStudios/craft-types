#pragma once

#include "types/utility/graph/graph.hpp"

#include <string>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif

namespace test_help
{
    void fillStrGraphWithNorse(graph::Graph< graph::GraphCore<std::string> > & g);
}
