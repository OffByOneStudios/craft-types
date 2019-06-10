#pragma once

#include <memory>
#include <variant>
#include <vector>

#include "graph_util.hpp"

/*
 * This file contains the query system and is laid out as follows:
 * 
 * - GraphQuery
 * - GraphQueryStepEmpty
 * - GraphQueryStepVertex
 * - query namespace
 *   > Contains helpers to use the query system.
 */

namespace graph
{
    template<typename TGraph>
    class GraphQuery final
    {
    public:
        using Graph = TGraph;

        class State final
        {
            public:
        };

        class Gremlin
        {
        public:
            typename TGraph::Node const* node;
        };

        enum class PipeResultEnum
        {
            Done,
            Pull
        };

        using PipeResult = std::variant<PipeResultEnum, Gremlin*>;

        class Step
        {
        protected:
            friend class GraphQuery;

        public:
            virtual ~Step() = default;

        protected:
            inline virtual Step* init() const { return const_cast<Step*>(this); };

            virtual PipeResult pipeFunc(TGraph const* graph, Gremlin* gremlin) = 0;
        };

    private:

        std::vector<std::unique_ptr<Step>> _steps;
        TGraph const* _graph;

    public:
        GraphQuery()
            : _steps()
            , _graph(nullptr)
        { }
        GraphQuery(TGraph const* g)
            : _steps()
            , _graph(g)
        { }

        ~GraphQuery()
        { }

    public:
        inline void reset()
        {
            
        }

        inline void setGraph(TGraph const* g)
        {
            reset();
            _graph = g;
        }

        inline void addStep(std::unique_ptr<Step> && step)
        {
            _steps.push_back(std::move(step));
        }

        
    };


    template<typename TGraph>
    class GraphQueryStepEmpty
        : public GraphQuery<TGraph>::Step
    {
    private:
        using Query = GraphQuery<TGraph>;

    public:
        inline GraphQueryStepEmpty() = default;
        inline GraphQueryStepEmpty(GraphQueryStepEmpty const&) = default;
        inline GraphQueryStepEmpty(GraphQueryStepEmpty &&) = default;

        inline ~GraphQueryStepEmpty() = default;

    protected:
        inline virtual typename Query::PipeResult pipeFunc(
            TGraph const* graph,
            typename Query::Gremlin* gremlin
        ) override
        {
            return gremlin != nullptr
                ? Query::PipeResult(gremlin)
                : Query::PipeResult(Query::PipeResultEnum::Pull);
        }
    };


    template<typename TGraph>
    class GraphQueryStepVertex
        : public GraphQuery<TGraph>::Step
    {
    private:
        using Query = GraphQuery<TGraph>;

    // config
    protected:
        std::vector<typename TGraph::Node const*> _nodes;

    // state
    protected:
        typename decltype(_nodes)::iterator _it;

    public:
        inline GraphQueryStepVertex(std::vector<typename TGraph::Node const*> nodes)
            : _nodes(nodes)
            , _it(_nodes.end())
        { }

        inline GraphQueryStepVertex(GraphQueryStepVertex const&) = default;
        inline GraphQueryStepVertex(GraphQueryStepVertex &&) = default;

        inline ~GraphQueryStepVertex() = default;

    protected:
        virtual typename Query::Step* init() const override
        {
            auto res = new GraphQueryStepVertex(_nodes);
            res->_it = res->_nodes.begin();

            return res;
        };

        inline virtual typename Query::PipeResult pipeFunc(
            TGraph const* graph,
            typename Query::Gremlin* gremlin
        ) override
        {
            if (_it == _nodes.end())
                return Query::PipeResultEnum::Done;
            else
                return gremlin; // TODO makeNewWithGremlinState
        }
    };


    template<typename TGraph, typename TFunc>
    class GraphQueryStepEdges
        : public GraphQuery<TGraph>::Step
    {
    private:
        using Query = GraphQuery<TGraph>;

    // config
    protected:
        TFunc _func;

    // state
    protected:
        typename Query::Gremlin* _gremlin;
        std::vector<typename TGraph::Edge const*> _edges;

    public:
        inline GraphQueryStepEdges(TFunc func)
            : _func(func)
        { }

        inline GraphQueryStepEdges(GraphQueryStepEdges const&) = default;
        inline GraphQueryStepEdges(GraphQueryStepEdges &&) = default;

        inline ~GraphQueryStepEdges() = default;

    protected:
        virtual typename Query::Step* init() const override
        {
            auto res = new GraphQueryStepEdges(*this);

            return res;
        };

        inline virtual typename Query::PipeResult pipeFunc(
            TGraph const* graph,
            typename Query::Gremlin* gremlin
        ) override
        {
            if (gremlin == nullptr && _edges.size() == 0)
                return Query::PipeResultEnum::Pull;

            if (_edges.size() == 0)
            {
                _gremlin = gremlin;

                edges = collectEdges(g, gremlin->node, _func);
            }
        }
    };


    namespace query
    {
        namespace _impl
        {
            template<template <typename> class TStep, typename... TArgs>
            struct step_builder
            {
                std::tuple<TArgs...> args;

                step_builder(TArgs&& ... args_)
                {
                    args = std::forward_as_tuple(args_ ...);
                }

                ~step_builder() = default;
            };

            template<template <typename...> class TStep, typename... TTemplateArgs>
            struct step_type_alias_wrapper
            {
                template<typename TGraph>
                class alias final : public TStep<TGraph, TTemplateArgs...>
                {
                    public:
                        using TStep<TGraph, TTemplateArgs...>::TStep;
                };
            };
        }

        template<typename TGraph, template <typename> class TStep, typename ...TArgs>
        std::unique_ptr<GraphQuery<TGraph>> operator |(std::unique_ptr<GraphQuery<TGraph>>&& q, _impl::step_builder<TStep, TArgs...> && step)
        {
            q->addStep(
                std::make_unique<TStep<TGraph>>(
                    std::move(std::make_from_tuple<TStep<TGraph>>(step.args))
                ));

            return std::move(q);
        }

        template<typename TGraph>
        std::unique_ptr<GraphQuery<TGraph>> q()
        {
            return std::make_unique<GraphQuery<TGraph>>();
        }

        template<typename TGraph>
        std::unique_ptr<GraphQuery<TGraph>> q(TGraph const& g)
        {
            return std::make_unique<GraphQuery<TGraph>>(&g);
        }

        template<typename ...TArgs>
        auto v(TArgs... args)
        {
            return _impl::step_builder<GraphQueryStepVertex, TArgs...>(std::forward<TArgs>(args)...);
        }

        template<typename TFunc, typename ...TArgs>
        auto in(TFunc func, TArgs... args)
        {
            return _impl::step_builder<_impl::step_type_alias_wrapper<GraphQueryStepEdges, TFunc>::alias, TArgs...>(std::forward<TArgs>(args)...);
        }
    }
}

