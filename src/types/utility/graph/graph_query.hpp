#pragma once

#include <memory>
#include <variant>
#include <vector>
#include <set>

#include "graph_util.hpp"

/*
 * This file contains the query system and is laid out as follows:
 * 
 * - GraphQueryEngine
 * - [pipes]
 *   - GraphQueryPipeEmpty
 *   - GraphQueryPipeVertex
 *   - GraphQueryPipeEdges
 * - [syntax]
 *   - GraphQueryLibraryBase
 *   - GraphQueryLibraryCore
 *   - GraphQuery
 *   - query()
 */

namespace graph
{
    template<typename TGraph>
    class GraphQueryEngine final
    {
    public:
        using Graph = TGraph;

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

        using PipeResult = std::variant<PipeResultEnum, std::shared_ptr<Gremlin>>;

        class Pipe
        {
        protected:
            friend class GraphQueryEngine<TGraph>;

        public:
            virtual ~Pipe() = default;

        protected:
            inline virtual Pipe* init() const { return const_cast<Pipe*>(this); };
            inline virtual void cleanup() { delete this; };

            virtual PipeResult pipeFunc(TGraph const* graph, std::shared_ptr<Gremlin> const& gremlin) = 0;
        };

    public:
        static inline std::shared_ptr<Gremlin> makeGremlin(typename TGraph::Node const* node)
        {
            std::shared_ptr<Gremlin> ret = std::make_shared<Gremlin>();
            ret->node = node;

            return ret;
        }
        static inline std::shared_ptr<Gremlin> makeGremlin(typename TGraph::Node const* node, std::shared_ptr<Gremlin> const& original)
        {
            if (!original)
                return makeGremlin(node);

            std::shared_ptr<Gremlin> ret = std::make_shared<Gremlin>(*original);
            ret->node = node;

            return ret;
        }
        static inline std::shared_ptr<Gremlin> gotoVertex(std::shared_ptr<Gremlin> const& current, typename TGraph::Node const* node)
        {
            return makeGremlin(node, current);
        }

        static inline std::shared_ptr<Gremlin> nullGremlin()
        {
            return std::shared_ptr<Gremlin>();
        }

    private:

        std::vector<std::unique_ptr<Pipe>> _pipes;
        std::vector<Pipe*> _state;
        TGraph const* _graph;

    public:
        GraphQueryEngine()
            : _pipes()
            , _state()
            , _graph(nullptr)
        { }
        GraphQueryEngine(TGraph const* g)
            : _pipes()
            , _state()
            , _graph(g)
        { }

        ~GraphQueryEngine()
        {
            reset();
        }

    public:
        inline void reset()
        {
            for (auto pipe : _state)
            {
                pipe->cleanup();
            }
            _state.clear();
        }

        inline void setGraph(TGraph const* g)
        {
            reset();
            _graph = g;
        }

        inline TGraph const* getGraph() const
        {
            return _graph;
        }

        inline void addPipe(std::unique_ptr<Pipe> && pipe)
        {
            _pipes.push_back(std::move(pipe));
        }

        inline size_t countPipes() const
        {
            return _pipes.size();
        }

        inline std::vector<typename TGraph::Node const*> run()
        {
            // Create state if there is none (or it is stale)
            if (_state.size() != _pipes.size())
            {
                reset();
                _state.reserve(_pipes.size());
                for (auto const& pipe : _pipes)
                {
                    _state.push_back(pipe->init());
                }
            }

            // The results of the query
            std::vector<std::shared_ptr<Gremlin>> results;

            // We operate "one to the right" because we need sentinals on both sides.
            const size_t max = _state.size();
            size_t done = 0;
            size_t pc = max;

            PipeResult maybe_gremlin = nullGremlin();

            while (done < max)
            {
                auto gremlin = std::holds_alternative<std::shared_ptr<Gremlin>>(maybe_gremlin)
                    ? std::get<std::shared_ptr<Gremlin>>(maybe_gremlin)
                    : nullGremlin();

                maybe_gremlin = _state[pc - 1]->pipeFunc(_graph, gremlin);

                auto maybe_gremlin_is_enum = std::holds_alternative<PipeResultEnum>(maybe_gremlin);
                if (maybe_gremlin_is_enum)
                {
                    auto result_enum = std::get<PipeResultEnum>(maybe_gremlin);
                    maybe_gremlin = nullGremlin();
                    
                    // Pipe needs more input
                    if (result_enum == PipeResultEnum::Pull)
                    {
                        // Try the previous pipe
                        if (pc - 1 > done)
                        {
                            pc --;
                            continue;
                        }
                        // , the previous pipe is done then so is this one
                        else
                            done = pc;
                    }
                    // Pipe is finished
                    else if (result_enum == PipeResultEnum::Done)
                    {
                        done = pc;
                    }
                }

                // Move to next pipe
                pc ++; 

                if (pc > max)
                {
                    // check for a gremlin that popped out
                    if (!maybe_gremlin_is_enum)
                    {
                        auto gremlin = std::get<std::shared_ptr<Gremlin>>(maybe_gremlin);
                        if (gremlin)
                        {
                            results.push_back(gremlin);
                        }
                    }

                    // step back
                    maybe_gremlin = nullGremlin();
                    pc --;
                }
            }

            // TODO build a better way to map projected reuslts
            // (perhaps by mutating query template and templating this function?)

            std::vector<typename TGraph::Node const*> ret;
            ret.reserve(results.size());
            for (auto grem : results)
            {
                ret.push_back(grem->node);
            }

            return ret;
        }
    };


    template<typename TGraph>
    class GraphQueryPipeEmpty
        : public GraphQueryEngine<TGraph>::Pipe
    {
    private:
        using Query = GraphQueryEngine<TGraph>;

    public:
        inline GraphQueryPipeEmpty() = default;
        inline GraphQueryPipeEmpty(GraphQueryPipeEmpty const&) = default;
        inline GraphQueryPipeEmpty(GraphQueryPipeEmpty &&) = default;

        inline ~GraphQueryPipeEmpty() = default;

    protected:
        inline virtual void cleanup() override { };

        inline virtual typename Query::PipeResult pipeFunc(
            TGraph const* graph,
            std::shared_ptr<typename Query::Gremlin> const& gremlin
        ) override
        {
            return gremlin
                ? typename Query::PipeResult(gremlin)
                : typename Query::PipeResult(Query::PipeResultEnum::Pull);
        }
    };


    template<typename TGraph>
    class GraphQueryPipeVertex
        : public GraphQueryEngine<TGraph>::Pipe
    {
    private:
        using Query = GraphQueryEngine<TGraph>;

    // config
    protected:
        std::vector<typename TGraph::Node const*> _nodes;

    // state
    protected:
        typename decltype(_nodes)::iterator _it;

    public:
        inline GraphQueryPipeVertex(std::vector<typename TGraph::Node const*> nodes)
            : _nodes(nodes)
            , _it(_nodes.end())
        { }
        inline GraphQueryPipeVertex(typename TGraph::Node const* node)
            : _nodes({node})
            , _it(_nodes.end())
        { }

        inline GraphQueryPipeVertex(GraphQueryPipeVertex const&) = default;
        inline GraphQueryPipeVertex(GraphQueryPipeVertex &&) = default;

        inline ~GraphQueryPipeVertex() = default;

    protected:
        virtual typename Query::Pipe* init() const override
        {
            auto res = new GraphQueryPipeVertex(_nodes);
            res->_it = res->_nodes.begin();

            return res;
        };

        inline virtual typename Query::PipeResult pipeFunc(
            TGraph const* graph,
            std::shared_ptr<typename Query::Gremlin> const& gremlin
        ) override
        {
            if (_it == _nodes.end())
                return Query::PipeResultEnum::Done;
            else
                return GraphQueryEngine<TGraph>::makeGremlin(*(_it ++), gremlin);
        }
    };


    template<typename TGraph, typename TFuncEdges, typename TFuncEdgeNodes>
    class GraphQueryPipeEdges
        : public GraphQueryEngine<TGraph>::Pipe
    {
    private:
        using Query = GraphQueryEngine<TGraph>;

    // config
    protected:
        TFuncEdges _func_edges;
        TFuncEdgeNodes _func_edgeNodes;

    // state
    protected:
        std::shared_ptr<typename Query::Gremlin> _gremlin;
        std::vector<typename TGraph::Edge const*> _edges;
        typename decltype(_edges)::const_iterator _edges_it;
        std::vector<typename TGraph::Node const*> _nodes;
        typename decltype(_nodes)::const_iterator _nodes_it;

    public:
        inline GraphQueryPipeEdges(TFuncEdges const& func_edges, TFuncEdgeNodes const& func_edgeNodes)
            : _func_edges(func_edges)
            , _func_edgeNodes(func_edgeNodes)
            , _gremlin()
            , _edges()
            , _edges_it(_edges.end())
            , _nodes()
            , _nodes_it(_nodes.end())
        { }

        inline GraphQueryPipeEdges(GraphQueryPipeEdges const&) = default;
        inline GraphQueryPipeEdges(GraphQueryPipeEdges &&) = default;

        inline ~GraphQueryPipeEdges() = default;

    protected:
        virtual typename Query::Pipe* init() const override
        {
            auto res = new GraphQueryPipeEdges(*this);

            return res;
        };

        inline virtual typename Query::PipeResult pipeFunc(
            TGraph const* graph,
            std::shared_ptr<typename Query::Gremlin> const& gremlin
        ) override
        {
            auto empty = _edges_it == _edges.end() && _nodes_it == _nodes.end();
            if (!gremlin && empty)
                return Query::PipeResultEnum::Pull;

            // get a collection of edges on this node
            if (empty) 
            {
                _gremlin = gremlin;

                _edges = collectEdges(*graph, _gremlin->node, _func_edges);
                _edges_it = _edges.begin();
                _nodes_it = _nodes.end();
            }

            // pop to a new edge
            while (_nodes_it == _nodes.end()) 
            {
                if (_edges_it == _edges.end())
                    return Query::PipeResultEnum::Pull;

                _nodes = collectNodes(*graph, (typename TGraph::Edge const*) *(_edges_it ++), _func_edgeNodes);
                _nodes_it = _nodes.begin();
            }

            return GraphQueryEngine<TGraph>::gotoVertex(_gremlin, (typename TGraph::Node const*) *(_nodes_it ++));
        }
    };


    template<typename TGraph>
    class GraphQueryPipeUnique
        : public GraphQueryEngine<TGraph>::Pipe
    {
    private:
        using Query = GraphQueryEngine<TGraph>;

    // config
    protected:

    // state
    protected:
        std::set<typename TGraph::Node const*> _seen;

    public:
        inline GraphQueryPipeUnique()
            : _seen()
        { }

        inline GraphQueryPipeUnique(GraphQueryPipeUnique const&) = default;
        inline GraphQueryPipeUnique(GraphQueryPipeUnique &&) = default;

        inline ~GraphQueryPipeUnique() = default;

    protected:
        virtual typename Query::Pipe* init() const override
        {
            auto res = new GraphQueryPipeUnique();

            return res;
        };

        inline virtual typename Query::PipeResult pipeFunc(
            TGraph const* graph,
            std::shared_ptr<typename Query::Gremlin> const& gremlin
        ) override
        {
            if (!gremlin)
                return Query::PipeResultEnum::Pull;
            if (_seen.count(gremlin->node))
                return Query::PipeResultEnum::Pull;
            _seen.insert(gremlin->node);
            return gremlin;
        }
    };


    template<typename TGraph, typename RetType>
    class GraphQueryLibraryBase
    {
    protected:
        virtual RetType addPipe(std::unique_ptr<typename GraphQueryEngine<TGraph>::Pipe> && Pipe) = 0;
    };

    template<typename TGraph, typename RetType>
    class GraphQueryLibraryCore
        : protected GraphQueryLibraryBase<TGraph, RetType>
    {
    public:
        template<typename ...TArgs>
        RetType v(TArgs... args)
        {
            return this->addPipe(std::make_unique<GraphQueryPipeVertex<TGraph>>(std::forward<TArgs>(args)...));
        }

        template<typename TFuncEdges, typename TFuncEdgeNodes>
        RetType e(TFuncEdges func_edges, TFuncEdgeNodes func_edgeNodes)
        {
            return this->addPipe(std::make_unique<GraphQueryPipeEdges<TGraph, TFuncEdges, TFuncEdgeNodes>>(std::forward<TFuncEdges>(func_edges), std::forward<TFuncEdgeNodes>(func_edgeNodes)));
        }

        template<typename TFuncEdges>
        RetType in(TFuncEdges func_edges)
        {
            auto inEdgeNodeFunc = &edgeIsOutgoing<TGraph>;
            return this->addPipe(std::make_unique<GraphQueryPipeEdges<TGraph, TFuncEdges, decltype(inEdgeNodeFunc)>>(std::forward<TFuncEdges>(func_edges), inEdgeNodeFunc));
        }

        template<typename TFuncEdges>
        RetType out(TFuncEdges func_edges)
        {
            auto outEdgeNodeFunc = &edgeIsIncoming<TGraph>;
            return this->addPipe(std::make_unique<GraphQueryPipeEdges<TGraph, TFuncEdges, decltype(outEdgeNodeFunc)>>(std::forward<TFuncEdges>(func_edges), outEdgeNodeFunc));
        }
        
        RetType unique()
        {
            return this->addPipe(std::make_unique<GraphQueryPipeUnique<TGraph>>());
        }
    };

    template<typename TGraph, template <typename, typename> typename TGraphQueryLibrary> 
    class GraphQuery final
        : public TGraphQueryLibrary<TGraph, GraphQuery<TGraph, TGraphQueryLibrary>>
    {
    private:
        std::unique_ptr<GraphQueryEngine<TGraph>> _engine;

    protected:
        inline virtual GraphQuery addPipe(std::unique_ptr<typename GraphQueryEngine<TGraph>::Pipe> && Pipe) override
        {
            _engine->addPipe(std::move(Pipe));

            return std::move(*this);
        }

    public:
        GraphQuery(TGraph const* g)
        {
            _engine = std::make_unique<GraphQueryEngine<TGraph>>(g);
        }

        ~GraphQuery() = default;

        GraphQuery(GraphQuery&& that) = default;
        GraphQuery& operator=(GraphQuery&& other) = default;

        GraphQuery(GraphQuery const& that) = delete;
        GraphQuery& operator=(const GraphQuery& other) = delete;

        inline GraphQueryEngine<TGraph>* operator->()
        {
            return _engine.operator->();
        }

        inline std::vector<typename TGraph::Node const*> run()
        {
            // TODO template magic to pass query template outs
            return _engine->run();
        }
    };

    template<typename TGraph, template <typename, typename> typename TGraphQueryLibrary = GraphQueryLibraryCore>
    GraphQuery<TGraph, TGraphQueryLibrary> query(TGraph const* g)
    {
        return GraphQuery<TGraph, TGraphQueryLibrary>(g);
    }
}
