#pragma once

#include <map>

#include "graph_util.hpp"

namespace graph
{
    template<typename TGraph>
    void copy_graph(TGraph& dst, TGraph const& src)
    {
        // TODO: need a copy data-section function to copy types and other data
        //std::map<TGraph::Label*, TGraph::Label*> _labelMap;

        src.forAllLabels([&](auto l)
        {
            dst.addLabel(l->data);
        });
    }
}