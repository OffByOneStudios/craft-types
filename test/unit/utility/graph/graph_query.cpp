#include "catch2/catch.hpp"

#include "shared.h"

#include "types/utility/graph/graph.hpp"

#include <string>

using namespace graph;
using namespace Catch::Matchers;


TEST_CASE( "graph::query() basics", "[graph::GraphQuery]" )
{
    Graph< GraphCore<std::string> > g;

    test_help::fillStrGraphWithNorse(g);

    SECTION( "graph::query() returns working query object" )
    {
        auto q = query(&g);

        CHECK(q->getGraph() == &g);
        CHECK(q->countPipes() == 0);

        auto r = q.run();

        CHECK(r.size() == 0);
    }

    SECTION( "graph::query() ->addPipe() can add GraphQueryPipeEmpty manually" )
    {
        auto q = query(&g);

        q->addPipe(std::make_unique<GraphQueryPipeEmpty<decltype(g)>>());

        CHECK(q->getGraph() == &g);
        CHECK(q->countPipes() == 1);

        auto r = q.run();

        CHECK(r.size() == 0);
    }

    SECTION( "graph:query().v() can add GraphQueryStepVertex (AKA `v`) with syntax" )
    {
        auto q = query(&g)
            .v(findNode(g, "thor"));

        CHECK(q->getGraph() == &g);
        CHECK(q->countPipes() == 1);
        
        auto r = q.run();

        CHECK(r.size() == 1);
    }

    SECTION( "graph::query.e() can add GraphQueryStepEdges with syntax" )
    {
        auto q = query(&g)
            .v(findNode(g, "thor"))
            .e( [](auto n, auto e) { return e->data == "parents"; },
                [](auto e, auto n) { return edgeIsIncoming<decltype(g)>(n, e); });

        CHECK(q->getGraph() == &g);
        CHECK(q->countPipes() == 2);
        
        auto r = q.run();

        CHECK(r.size() == 2);
    }
}
