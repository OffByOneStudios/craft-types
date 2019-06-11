#include "shared.h"

using namespace graph;

void test_help::fillStrGraphWithNorse(Graph< GraphCore<std::string> > & g)
{
    // from http://www.veritablehokum.com/comic/the-norse-god-family-tree/

    /* Data Model:
     * - [Edges]
     *   - parents: where possible is organized (child, mother, father)
     */ 

    // Helper temporaries
    typename Graph< GraphCore<std::string> >::Edge* e;

    // Asgardians
    auto odin = g.addNode("odin");

    auto jord = g.addNode("jord");

    auto frigg = g.addNode("frigg");

    e = g.addEdge("married", { odin, frigg });



    auto nanna = g.addNode("nanna");

    auto baldr = g.addNode("baldr");

    auto hodr = g.addNode("hodr");
    e = g.addEdge("parents", { hodr, frigg, odin });

    auto bragi = g.addNode("bragi");
    e = g.addEdge("parents", { bragi, frigg, odin });

    auto idunn = g.addNode("idunn");

    e = g.addEdge("married", { baldr, nanna });



    auto forseti = g.addNode("forseti");
    e = g.addEdge("parents", { forseti, nanna, baldr });

    auto sif = g.addNode("sif");

    auto thor = g.addNode("thor");
    e = g.addEdge("parents", { thor, jord, odin });

    auto jarnsaxa = g.addNode("jarnsaxa");

    e = g.addEdge("married", { thor, sif });




    auto skadi = g.addNode("skadi");

    auto njord = g.addNode("njord");

    auto ullr = g.addNode("ullr");
    e = g.addEdge("parents", { ullr, sif, odin });

    auto modi = g.addNode("modi");
    e = g.addEdge("parents", { modi, sif, thor });

    auto thrud = g.addNode("thrud");
    e = g.addEdge("parents", { thrud, sif, thor });

    auto magni = g.addNode("magni");
    e = g.addEdge("parents", { magni, jarnsaxa, thor });



    auto gerdr = g.addNode("gerdr");

    auto freyr = g.addNode("freyr");
    e = g.addEdge("parents", { freyr, skadi, njord });

    auto freya = g.addNode("freya");
    e = g.addEdge("parents", { freya, skadi, njord });

    auto odr = g.addNode("odr");

    e = g.addEdge("married", { freyr, gerdr });
    e = g.addEdge("married", { odr, freya });
}



#include "catch2/catch.hpp"

using namespace Catch::Matchers;

TEST_CASE( "test_help works correctly", "[test_help]" )
{
    Graph< GraphCore<std::string> > g;

    test_help::fillStrGraphWithNorse(g);
}