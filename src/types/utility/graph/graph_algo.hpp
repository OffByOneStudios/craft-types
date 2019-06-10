#pragma once

#include <map>

#include "graph_util.hpp"

/*
 * This file contains the various minor algorithms and is laid out as follows:
 * 
 * - edgeIsIncoming
 * - findNode
 * - collectEdges
 * - copyGraph (TODO)
 */

namespace graph
{
    template<typename TGraph>
    typename bool edgeIsIncoming(TGraph const& g, typename TGraph::Node const* n, typename TGraph::Edge const* e)
    {
        const auto inverseFlagValue = TGraph::MetaFlags::Flag_InverseEdge;

        return  (e->nodes[0] != n) != (((uint64_t)e->flags & (uint64_t)inverseFlagValue) == (uint64_t)inverseFlagValue);
    }
    template<typename TGraph>
    typename bool edgeIsOutgoing(TGraph const& g, typename TGraph::Node const* n, typename TGraph::Edge const* e)
    {
        const auto inverseFlagValue = TGraph::MetaFlags::Flag_InverseEdge;

        return  (e->nodes[0] != n) != (((uint64_t)e->flags & (uint64_t)inverseFlagValue) == (uint64_t)inverseFlagValue);
    }

    template<typename TGraph>
    typename TGraph::Node const* findNode(TGraph const& g, typename TGraph::Data const& v)
    {
        typename TGraph::Node const* res = nullptr;
        g.forAllNodes([&](auto n)
        {
            if (n->data == v)
                res = n;
            return n->data != v;
        });
        return res;
    }

    template<typename TGraph, typename Func>
    typename std::vector<typename TGraph::Edge const*> collectEdges(TGraph const& g, typename TGraph::Node const* n, Func func)
    {
        typename std::vector<typename TGraph::Edge const*> res;
        g.forAllEdgesOnNode([&](auto e)
        {
            if (func(e))
                res.push_back(e);
        });
        return res;
    }

    template<typename TGraph>
    void copyGraph(TGraph& dst, TGraph const& src)
    {
        // TODO: need a copy data-section function to copy types and other data
        //std::map<TGraph::Label*, TGraph::Label*> _labelMap;

        src.forAllLabels([&](auto l)
        {
            dst.addLabel(l->data);
        });
    }
}
