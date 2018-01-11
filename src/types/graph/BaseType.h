#pragma once
#include "../common.h"
#include "graph.h"

namespace craft {
namespace types {

	class BaseType abstract
		: public SType
	{
	protected:
		TypeId _identifier;

	// 
	// Lifecycle
	//
	public:
		CRAFT_TYPES_EXPORTED BaseType();
		CRAFT_TYPES_EXPORTED virtual ~BaseType();

	//
	// Identifiable
	//
	public:
		CRAFT_TYPES_EXPORTED virtual std::string identifiableName() const override;
		CRAFT_TYPES_EXPORTED virtual TypeId identifiableId() const override;

	//
	// Parametric
	//
	public:
		CRAFT_TYPES_EXPORTED virtual bool isParametricScope() const override;

		/* This method provides the creation scope of parametric variables.

		*/
		CRAFT_TYPES_EXPORTED virtual std::vector<SType*> const& parametricScopes() const override;

		CRAFT_TYPES_EXPORTED virtual bool isParametricConsumer() const override;

		/* This method provides the for the consumption of parametric variables.

		*/
		CRAFT_TYPES_EXPORTED virtual std::vector<SType*> const& parametricConsumption() const override;

	//
	// Subtype Hierarchy
	//
	public:
		CRAFT_TYPES_EXPORTED virtual bool isSubtypeHierarchyParticipant() const override;
		CRAFT_TYPES_EXPORTED virtual bool canHaveChildren() const override;
		CRAFT_TYPES_EXPORTED virtual bool canHaveAdoptiveParents() const override;

		CRAFT_TYPES_EXPORTED virtual std::vector<SType*> const& parents() const override;
		CRAFT_TYPES_EXPORTED virtual std::set<SType*> const& adoptiveParents() const override;
		CRAFT_TYPES_EXPORTED virtual std::set<SType*> const& children() const override;

		CRAFT_TYPES_EXPORTED virtual void makeChild(TypeGraphSystem*, SType*) override;
		CRAFT_TYPES_EXPORTED virtual void adoptiveParent(TypeGraphSystem*, SType*) override;

	//
	// Algorithms
	//
	public:
		CRAFT_TYPES_EXPORTED virtual void subtypingDo(SubtypeAlgorithmState*) const override;
		CRAFT_TYPES_EXPORTED virtual void validationDo(ValidationAlgorithmState*) const override;
		CRAFT_TYPES_EXPORTED virtual void validationDo(ValidationWithSubtypingAlgorithmState*) const override;

	//
	// Internal
	//
	protected:
		CRAFT_TYPES_EXPORTED virtual void _registerTypeIdentifier(TypeId const&) override;
	};


	class BaseHierarchyType abstract
		: public BaseType
	{
	protected:
		std::vector<SType*> _hierarchy_parents;
		std::set<SType*> _hierarchy_adoptiveParents;
		std::set<SType*> _hierarchy_children;

		// 
		// Lifecycle
		//
	public:
		CRAFT_TYPES_EXPORTED virtual ~BaseHierarchyType();

		//
		// Subtype Hierarchy
		//
	public:
		CRAFT_TYPES_EXPORTED virtual bool isSubtypeHierarchyParticipant() const override;
		CRAFT_TYPES_EXPORTED virtual bool canHaveChildren() const override;
		CRAFT_TYPES_EXPORTED virtual bool canHaveAdoptiveParents() const override;

		CRAFT_TYPES_EXPORTED virtual std::vector<SType*> const& parents() const override;
		CRAFT_TYPES_EXPORTED virtual std::set<SType*> const& adoptiveParents() const override;
		CRAFT_TYPES_EXPORTED virtual std::set<SType*> const& children() const override;

		CRAFT_TYPES_EXPORTED virtual void makeChild(TypeGraphSystem*, SType*) override;
		CRAFT_TYPES_EXPORTED virtual void adoptiveParent(TypeGraphSystem*, SType*) override;

		//
		// Algorithms
		//
	public:
		CRAFT_TYPES_EXPORTED virtual void subtypingDo(SubtypeAlgorithmState*) const override;
		CRAFT_TYPES_EXPORTED virtual void validationDo(ValidationAlgorithmState*) const override;
		CRAFT_TYPES_EXPORTED virtual void validationDo(ValidationWithSubtypingAlgorithmState*) const override;
	};
}}
