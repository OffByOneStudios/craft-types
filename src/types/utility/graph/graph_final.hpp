#pragma once

#include <map>
#include <vector>
#include <deque>

namespace graph
{
    template<typename TGraphBase>
    class GraphFinalizer final
        : public TGraphBase
    {
    public:
        struct Label
            : public TGraphBase::Core
            , public TGraphBase::Label
        { };

        struct Node
            : public TGraphBase::Core
            , public TGraphBase::Node
        { };

        struct Edge
            : public TGraphBase::Core
            , public TGraphBase::Edge
        { };

        struct Prop
            : public TGraphBase::Core
            , public TGraphBase::Prop
        { };
    };

    // This type realizes the graph
    template<typename TGraphBase>
    class Graph
        : public TGraphBase::template Actual<GraphFinalizer<TGraphBase>>
    {

    };
}