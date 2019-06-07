#pragma once

#include <map>

#include "graph_util.hpp"

namespace graph
{
    // The type extractor is a templated interface which dispatches (at compile time) to a type extraction method
    // Along with effecient access patterns
    template<typename TGraphBase, typename TTypeExtractor>
    class GraphTyped
        : public TGraphBase
    {
    // This type types:
    public:
        using TypeId = typename TTypeExtractor::Id;

    // Graph types:
    public:
        struct Core
            : public TGraphBase::Core
        {
            TypeId type;
        };

        struct Label
            : public TGraphBase::Label
        {
            std::map<TypeId, size_t> nodes_type_accel;
        };

        struct Node
            : public TGraphBase::Node
        {
            std::map<TypeId, size_t> edges_type_accel;
            std::map<TypeId, size_t> props_type_accel;
        };

        struct Edge
            : public TGraphBase::Edge
        {
            std::map<TypeId, size_t> props_type_accel;
        };
        
    // Actual type:
    public:
        template<typename TFinal>
        class Actual
            : private TGraphBase::template Actual<TFinal>
        {
            using Base = typename TGraphBase::template Actual<TFinal>;

            using Data = typename TFinal::Data;

        public:
            using Label = typename TFinal::Label;
            using Node = typename TFinal::Node;
            using Edge = typename TFinal::Edge;
            using Prop = typename TFinal::Prop;

        // Add functions
        public:
            inline Label* addLabel(Data const& data, TypeId type)
            {
                auto ret = Base::addLabel(data);
                ret->type = type;

                return ret;
            }

            template<typename T>
            inline Label* addLabel(T const& data)
            {
                return addLabel(TTypeExtractor::template type_store<T>(data), TTypeExtractor::template type_typeToValue<T>());
            }

            inline Node* addNode(Data const& data, TypeId type)
            {
                auto ret = Base::addNode(data);
                ret->type = type;

                return ret;
            }

            template<typename T>
            inline Node* addNode(T const& data)
            {
                return addNode(TTypeExtractor::template type_store<T>(data), TTypeExtractor::template type_typeToValue<T>());
            }
            
            // By default edges point from 0-index to all others
            inline Edge* addEdge(Data const& data, TypeId type, std::vector<Node*> const& nodes, bool invert = false)
            {
                auto ret = Base::addEdge(data, nodes, invert);
                ret->type = type;

                return ret;
            }

            template<typename T>
            inline Edge* addEdge(T const& data, std::vector<Node*> const& nodes, bool invert = false)
            {
                return addEdge(TTypeExtractor::template type_store<T>(data), TTypeExtractor::template type_typeToValue<T>(), nodes, invert);
            }
            
            inline Prop* addProp(Data const& data, TypeId type, Node* on_node)
            {
                auto ret = Base::addProp(data, on_node);
                ret->type = type;

                return ret;
            }

            template<typename T>
            inline Prop* addProp(T const& data, Node* on_node)
            {
                return addProp(TTypeExtractor::template type_store<T>(data), TTypeExtractor::template type_typeToValue<T>(), on_node);
            }

            inline Prop* addProp(Data const& data, TypeId type, Edge* on_edge)
            {
                auto ret = Base::addProp(data, on_edge);
                ret->type = type;

                return ret;
            }

            template<typename T>
            inline Prop* addProp(T const& data, Edge* on_edge)
            {
                return addProp(TTypeExtractor::template type_store<T>(data), TTypeExtractor::template type_typeToValue<T>(), on_edge);
            }

        // Getter functions
        public:
            using Base::labelCount;
            using Base::nodeCount;
            using Base::edgeCount;
            using Base::propCount;

            using Base::getNodeLabels;
            using Base::getEdgeNodes;
            using Base::isEdgeInverted;

            TypeId getType(Core* core)
            {
                return core->type;
            }

        // Iteration functions
        public:
            using Base::forAllLabels;
            using Base::forAllNodes;
            using Base::forAllEdges;
            using Base::forAllNodesInLabel;
            using Base::forAllLabelsOnNode;
            using Base::forAllEdgesOnNode;
            using Base::forAllPropsOnNode;

        // Update functions
        public:
            using Base::attachLabel;
            using Base::attachEdge;

            void attachType(Core* core, TypeId type)
            {
                core->type = type;
            }

        // Temporary
        // TODO: Create a graph that uses types to compile time prove properties
        // TODO: move the only functions there
        public:
            template<typename T>
            T const* onlyPropOfTypeOnNode(Node const* node) const
            {
                auto searchType = TTypeExtractor::template type_typeToValue<T>();
                T const* result = nullptr;

                forAllPropsOnNode(node,
                    [&](auto prop) -> bool
                    {
                        if (prop->type == searchType)
                        {
                            result = TTypeExtractor::template type_load<T>(prop->data);
                            return false;
                        }
                        return true;
                    });

                return result;
            }
        };
    };
}