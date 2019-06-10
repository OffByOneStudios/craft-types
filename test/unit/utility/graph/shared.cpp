#include "shared.h"

using namespace graph;

void test_help::fillStrGraphWithNorse(Graph< GraphCore<std::string> > & g)
{
    // from http://www.veritablehokum.com/comic/the-norse-god-family-tree/

    auto odin = g.addNode("odin");
    auto jord = g.addNode("jord");

    auto thor = g.addNode("thor");
    g.addEdge("parents", { thor, odin, jord });
}



#include "catch2/catch.hpp"

using namespace Catch::Matchers;

TEST_CASE( "test_help works correctly", "[test_help]" )
{
    Graph< GraphCore<std::string> > g;

    test_help::fillStrGraphWithNorse(g);
}