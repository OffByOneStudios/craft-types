#include "catch2/catch.hpp"

#include "shared.h"

#include "types/utility/graph/graph.hpp"

#include <string>

using namespace graph;
using namespace graph::query;
using namespace Catch::Matchers;


TEST_CASE( "graph::query basics", "[graph::GraphQuery]" )
{
    Graph< GraphCore<std::string> > g;

    test_help::fillStrGraphWithNorse(g);

    SECTION( "graph::query::q returns empty" )
    {
        q(g);
    }

    SECTION( "graph::query::q can add GraphQueryStepEmpty manually" )
    {
        auto query = q(g);

        query->addStep(std::make_unique<GraphQueryStepEmpty<decltype(g)>>());
    }

    SECTION( "graph::query::q can add GraphQueryStepVertex (AKA `v`) with syntax" )
    {
        auto query = q(g) | v(std::vector { findNode(g, "thor") });
    }

    SECTION( "graph::query::q can add GraphQueryStepEdges (AKA `in`, `out`) with syntax" )
    {
        //auto query = q(g) | in(edgeIsIncoming<decltype(g)>);
    }
}
