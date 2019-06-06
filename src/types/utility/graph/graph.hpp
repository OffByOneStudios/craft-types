#pragma once

#include <map>
#include <vector>
#include <deque>

/*
    This header only graph library has some very specific design constraints:

    * Highly configurable, it should be usable in a lot of circumstances.
        * C friendly potential.
    * Supports node labels, properties, and sorted n-ary edges.
    * Supports parent graphs, which it can override locally.
    * This necessitates component style storage.
    * Supports type based helper lookups.
*/

#include "graph_util.hpp"

#include "graph_core.hpp"
#include "graph_typed.hpp"
#include "graph_todo.hpp"

#include "graph_algo.hpp"
#include "graph_query.hpp"

#include "graph_final.hpp"