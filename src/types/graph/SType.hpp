#pragma once
#include "../common.h"
#include "graph.h"

namespace craft {
namespace types {

	// Not current included, this is for exstensible types.
	class SType
	{
	// 
	// Lifecycle
	//
	public:
		virtual ~SType() = default;
	
		// Avoid string compare, provides a "last resort" type disambiguation.
		virtual char const* kindName() const = 0;

	//
	// Identifiable
	//
	public:
		virtual bool isIdentifiable() const = 0;
		virtual std::string identifiableName() const = 0;
		virtual TypeId identifiableId() const = 0;

	//
	// Expression
	//
	public:
		virtual bool isExpressionLeaf() const = 0;
		virtual bool isExpressionComplex() const = 0;

		/* This method provides raw access to the structure of expression children.

		*/
		virtual std::vector<SType*> const& expression() const = 0;

	//
	// Parametric
	//
	public:
		virtual bool isParametricScope() const = 0;

		/* This method provides the creation scope of parametric variables.

		*/
		virtual std::vector<SType*> const& parametricScopes() const = 0;

		virtual bool isParametricConsumer() const = 0;

		/* This method provides the for the consumption of parametric variables.

		*/
		virtual std::vector<SType*> const& parametricConsumption() const = 0;

	//
	// Subtype Hierarchy
	//
	public:
		virtual bool isSubtypeHierarchyParticipant() const = 0;
		virtual bool canHaveChildren() const = 0;
		virtual bool canHaveAdoptiveParents() const = 0;

		virtual std::vector<SType*> const& parents() const = 0;
		virtual std::set<SType*> const& adoptiveParents() const = 0;
		virtual std::set<SType*> const& children() const = 0;

		virtual void makeChild(TypeGraphSystem*, SType*) = 0;
		virtual void adoptiveParent(TypeGraphSystem*, SType*) = 0;

	//
	// Algorithms
	//
	public:
		virtual void subtypingDo(SubtypeAlgorithmState*) const = 0;
		virtual void validationDo(ValidationAlgorithmState*) const = 0;
		virtual void validationDo(ValidationWithSubtypingAlgorithmState*) const = 0;

	//
	// Internal
	//
	protected:
		friend class TypeGraphSystem;

		virtual void _registerTypeIdentifier(TypeId const&) = 0;
	};
}}
