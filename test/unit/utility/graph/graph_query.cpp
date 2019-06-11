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
}


TEST_CASE( "graph::query() syntax queries", "[graph::GraphQuery]" )
{
    Graph< GraphCore<std::string> > g;

    test_help::fillStrGraphWithNorse(g);

    SECTION( "graph::query.e() can add GraphQueryStepEdges by hand" )
    {
        auto q = query(&g)
            .v(findNode(g, "thor"))
            // same as "out"
            .e( [](auto n, auto e) { return edgeIsOutgoing<decltype(g)>(n, e) && e->data == "parents"; },
                &edgeIsIncoming<decltype(g)>);
            

        CHECK(q->getGraph() == &g);
        CHECK(q->countPipes() == 2);
        
        auto r = q.run();

        CHECK(r.size() == 2);
    }

    SECTION( "graph::query.in() can add GraphQueryStepEdges with helper" )
    {
        auto q = query(&g)
            .v(findNode(g, "thor"))
            // TODO fix this to remove edgeIsIncoming
            .in( [](auto n, auto e) { return edgeIsIncoming<decltype(g)>(n, e) && e->data == "parents"; } );

        CHECK(q->getGraph() == &g);
        CHECK(q->countPipes() == 2);
        
        auto r = q.run();

        CHECK(r.size() == 3); // Thor has 3 children
    }

    SECTION( "graph::query.out() can add GraphQueryStepEdges with helper" )
    {
        auto q = query(&g)
            .v(findNode(g, "thor"))
            // TODO fix this to remove edgeIsOutgoing
            .out( [](auto n, auto e) { return edgeIsOutgoing<decltype(g)>(n, e) && e->data == "parents"; } );

        CHECK(q->getGraph() == &g);
        CHECK(q->countPipes() == 2);
        
        auto r = q.run();

        CHECK(r.size() == 2); // Thor has 2 parents
    }

    SECTION( "graph::query.e() can retrieve specific node" )
    {
        auto q = query(&g)
            .v(findNode(g, "thor"))
            .e( [](auto n, auto e) { return edgeIsOutgoing<decltype(g)>(n, e) && e->data == "parents"; },
                [](auto n, auto e) { return e->nodes.size() > 1 && e->nodes[1] == n; });

        CHECK(q->getGraph() == &g);
        CHECK(q->countPipes() == 2);
        
        auto r = q.run();

        REQUIRE(r.size() == 1);
        REQUIRE(r[0]->data == "jord"); // Thor's mom
    }
}