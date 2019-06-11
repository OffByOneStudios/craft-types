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
 *   - GraphQueryPipeUnique
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

            std::map<size_t, typename TGraph::Node const*> labels;

        public:
            typename TGraph::Node const* getLabel(size_t label) const
            {
                auto it = labels.find(label);
                if ( it == labels.end() )
                    return nullptr;
                else
                    return it->second;
            }
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

    // configure
    private:
        std::vector<std::unique_ptr<Pipe>> _pipes;

        size_t _labelCounter;
        std::map<std::string, size_t> _labels;

    // state
    private:
        std::vector<Pipe*> _state;
        TGraph const* _graph;

    public:
        GraphQueryEngine()
            : _pipes()
            , _labelCounter(0)
            , _labels()
            , _state()
            , _graph(nullptr)
        { }
        GraphQueryEngine(TGraph const* g)
            : _pipes()
            , _labelCounter(0)
            , _labels()
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

        inline size_t requireLabel(std::string const& label)
        {
            auto lb = _labels.lower_bound(label);

            if(lb != _labels.end() && !(_labels.key_comp()(label, lb->first)))
            {
                return lb->second;
            }
            else
            {
                auto id = ++_labelCounter;
                _labels.insert(lb, decltype(_labels)::value_type(label, id));
                return id;
            }
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

    enum class GraphQueryPipeEdgesEnum
    {
        All,
        Incoming,
        Outgoing
    };

    template<typename TGraph, GraphQueryPipeEdgesEnum EMode, typename TFuncEdges, typename TFuncEdgeNodes>
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

        static inline bool _modeFilterEdges(typename TGraph::Node const* n, typename TGraph::Edge const* e)
        {
            if constexpr (EMode == GraphQueryPipeEdgesEnum::All)
            {
                return true;
            }
            else if constexpr (EMode == GraphQueryPipeEdgesEnum::Incoming)
            {
                return edgeIsIncoming<TGraph>(n, e);
            }
            else if constexpr (EMode == GraphQueryPipeEdgesEnum::Outgoing)
            {
                return edgeIsOutgoing<TGraph>(n, e);
            }
            else
            {
                static_assert("Bad EMode");
            }
            
        }

        static inline bool _modeFilterEdgeNodes(typename TGraph::Node const* n, typename TGraph::Edge const* e, typename TGraph::Node const* en)
        {
            if constexpr (EMode == GraphQueryPipeEdgesEnum::All)
            {
                return n != en;
            }
            else if constexpr (EMode == GraphQueryPipeEdgesEnum::Incoming)
            {
                return !edgeIsIncoming<TGraph>(en, e);
            }
            else if constexpr (EMode == GraphQueryPipeEdgesEnum::Outgoing)
            {
                return !edgeIsOutgoing<TGraph>(en, e);
            }
            else
            {
                static_assert("Bad EMode");
            }
            
        }

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

                auto n = _gremlin->node;
                _edges = collectEdges(*graph, n,
                    [&](auto e) { return _modeFilterEdges(n, e) && _func_edges(n, e); });
                _edges_it = _edges.begin();
                _nodes_it = _nodes.end();
            }

            // pop to a new edge
            while (_nodes_it == _nodes.end()) 
            {
                if (_edges_it == _edges.end())
                    return Query::PipeResultEnum::Pull;

                // TODO: simplyfy this, we know which nodes on the edge are the ones we want to
                //   traverse, we may want to revert our iterator paradigm too.
                auto n = _gremlin->node;
                auto e = (typename TGraph::Edge const*) *(_edges_it ++);
                _nodes = collectNodes(*graph, e, 
                    [&](auto en) { return _modeFilterEdgeNodes(n, e, en) && _func_edgeNodes(n, e, en); });
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


    template<typename TGraph>
    class GraphQueryPipeLabel
        : public GraphQueryEngine<TGraph>::Pipe
    {
    private:
        using Query = GraphQueryEngine<TGraph>;

    // config
    protected:
        size_t _label;

    // state
    protected:

    public:
        inline GraphQueryPipeLabel(size_t label)
            : _label(label)
        { }

        inline GraphQueryPipeLabel(GraphQueryPipeLabel const&) = default;
        inline GraphQueryPipeLabel(GraphQueryPipeLabel &&) = default;

        inline ~GraphQueryPipeLabel() = default;

    protected:
        inline virtual void cleanup() override { };

        inline virtual typename Query::PipeResult pipeFunc(
            TGraph const* graph,
            std::shared_ptr<typename Query::Gremlin> const& gremlin
        ) override
        {
            if (!gremlin)
                return Query::PipeResultEnum::Pull;
            
            gremlin->labels[_label] = gremlin->node;

            return gremlin;
        }
    };

    
    template<typename TGraph>
    class GraphQueryPipeExcept
        : public GraphQueryEngine<TGraph>::Pipe
    {
    private:
        using Query = GraphQueryEngine<TGraph>;

    // config
    protected:
        size_t _label;

    // state
    protected:

    public:
        inline GraphQueryPipeExcept(size_t label)
            : _label(label)
        { }

        inline GraphQueryPipeExcept(GraphQueryPipeExcept const&) = default;
        inline GraphQueryPipeExcept(GraphQueryPipeExcept &&) = default;

        inline ~GraphQueryPipeExcept() = default;

    protected:
        inline virtual void cleanup() override { };

        inline virtual typename Query::PipeResult pipeFunc(
            TGraph const* graph,
            std::shared_ptr<typename Query::Gremlin> const& gremlin
        ) override
        {
            if (!gremlin)
                return Query::PipeResultEnum::Pull;
            
            if (gremlin->getLabel(_label) == gremlin->node)
                return Query::PipeResultEnum::Pull;

            return gremlin;
        }
    };


    template<typename TGraph, typename RetType>
    class GraphQueryLibraryBase
    {
    protected:
        virtual std::unique_ptr<GraphQueryEngine<TGraph>> & engine() = 0;
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

        template<typename TFuncEdges>
        RetType e(TFuncEdges func_edges)
        {
            auto edgeNodeFunc = [](auto n, auto e, auto ne){ return true; };
            return this->addPipe(std::make_unique<GraphQueryPipeEdges<TGraph, GraphQueryPipeEdgesEnum::All, TFuncEdges, decltype(edgeNodeFunc)>>(std::forward<TFuncEdges>(func_edges), edgeNodeFunc));
        }
        template<typename TFuncEdges, typename TFuncEdgeNodes>
        RetType e(TFuncEdges func_edges, TFuncEdgeNodes func_edgeNodes)
        {
            return this->addPipe(std::make_unique<GraphQueryPipeEdges<TGraph, GraphQueryPipeEdgesEnum::All, TFuncEdges, TFuncEdgeNodes>>(std::forward<TFuncEdges>(func_edges), std::forward<TFuncEdgeNodes>(func_edgeNodes)));
        }

        template<typename TFuncEdges>
        RetType in(TFuncEdges func_edges)
        {
            auto inEdgeNodeFunc = [](auto n, auto e, auto ne){ return true; };
            return this->addPipe(std::make_unique<GraphQueryPipeEdges<TGraph, GraphQueryPipeEdgesEnum::Incoming, TFuncEdges, decltype(inEdgeNodeFunc)>>(std::forward<TFuncEdges>(func_edges), inEdgeNodeFunc));
        }
        template<typename TFuncEdges, typename TFuncEdgeNodes>
        RetType in(TFuncEdges func_edges, TFuncEdgeNodes func_edgeNodes)
        {
            return this->addPipe(std::make_unique<GraphQueryPipeEdges<TGraph, GraphQueryPipeEdgesEnum::Incoming, TFuncEdges, TFuncEdgeNodes>>(std::forward<TFuncEdges>(func_edges), std::forward<TFuncEdgeNodes>(func_edgeNodes)));
        }

        template<typename TFuncEdges>
        RetType out(TFuncEdges func_edges)
        {
            auto outEdgeNodeFunc = [](auto n, auto e, auto ne){ return true; };
            return this->addPipe(std::make_unique<GraphQueryPipeEdges<TGraph, GraphQueryPipeEdgesEnum::Outgoing, TFuncEdges, decltype(outEdgeNodeFunc)>>(std::forward<TFuncEdges>(func_edges), outEdgeNodeFunc));
        }
        template<typename TFuncEdges, typename TFuncEdgeNodes>
        RetType out(TFuncEdges func_edges, TFuncEdgeNodes func_edgeNodes)
        {
            return this->addPipe(std::make_unique<GraphQueryPipeEdges<TGraph, GraphQueryPipeEdgesEnum::Outgoing, TFuncEdges, TFuncEdgeNodes>>(std::forward<TFuncEdges>(func_edges), std::forward<TFuncEdgeNodes>(func_edgeNodes)));
        }
        
        RetType unique()
        {
            return this->addPipe(std::make_unique<GraphQueryPipeUnique<TGraph>>());
        }

        RetType as(std::string const& label)
        {
            return this->addPipe(std::make_unique<GraphQueryPipeLabel<TGraph>>(engine()->requireLabel(label)));
        }

        RetType except(std::string const& label)
        {
            return this->addPipe(std::make_unique<GraphQueryPipeExcept<TGraph>>(engine()->requireLabel(label)));
        }
    };

    template<typename TGraph, template <typename, typename> typename TGraphQueryLibrary> 
    class GraphQuery final
        : public TGraphQueryLibrary<TGraph, GraphQuery<TGraph, TGraphQueryLibrary>>
    {
    private:
        std::unique_ptr<GraphQueryEngine<TGraph>> _engine;

    protected:
        virtual std::unique_ptr<GraphQueryEngine<TGraph>> & engine() override
        {
            return _engine;
        }
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
