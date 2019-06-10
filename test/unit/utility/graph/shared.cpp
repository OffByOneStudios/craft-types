#include "shared.h"

using namespace graph;
using namespace graph::query;

void test_help::fillStrGraphWithNorse(Graph< GraphCore<std::string> > & g)
{
    auto thor = g.addNode("thor");
}



#include "catch2/catch.hpp"

using namespace Catch::Matchers;

TEST_CASE( "test_help works correctly", "[test_help]" )
{
    Graph< GraphCore<std::string> > g;

    test_help::fillStrGraphWithNorse(g);
}