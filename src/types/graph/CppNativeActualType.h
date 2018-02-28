#pragma once
#include "../common.h"
#include "graph.h"

namespace craft {
namespace types {

	class CppNativeActualType final
		: BaseHierarchyType
	{
		size_t _size;
		std::string _name;

		bool _is_final;

	// 
	// Lifecycle
	//
	public:
		CRAFT_TYPES_EXPORTED virtual ~CppNativeActualType();
		CRAFT_TYPES_EXPORTED virtual char const* kindName() const;

	protected:
		CRAFT_TYPES_EXPORTED CppNativeActualType();

	// 
	// CppNativeActualType - Static Helpers
	//
	public:
		template<typename T>
		static CppNativeActualType* make()
		{
			static_assert(std::is_compound<T>::value, "Is compound, should probably not be given a native cpp type.");
			static_assert(!std::is_abstract<T>::value, "Is abstract, use CppNativeAbstractType instead.");
			
			auto ret = new CppNativeActualType();

			ret->_size = sizeof(T);
			ret->_name = typeid(*pb).name();
			ret->_is_final = std::is_final<T>::value;
		}

	//
	// Identifiable
	//
	public:
		CRAFT_TYPES_EXPORTED virtual bool isIdentifiable() const = 0;
		CRAFT_TYPES_EXPORTED virtual std::string identifiableName() const = 0;
		CRAFT_TYPES_EXPORTED virtual TypeId identifiableId() const = 0;

	//
	// Expression
	//
	public:
		CRAFT_TYPES_EXPORTED virtual bool isExpressionLeaf() const = 0;
		CRAFT_TYPES_EXPORTED virtual bool isExpressionComplex() const = 0;

		/* This method provides raw access to the structure of expression children.

		*/
		CRAFT_TYPES_EXPORTED virtual std::vector<SType*> const& expression() const = 0;

	//
	// Parametric
	//
	public:
		CRAFT_TYPES_EXPORTED virtual bool isParametricScope() const = 0;

		/* This method provides the creation scope of parametric variables.

		*/
		CRAFT_TYPES_EXPORTED virtual std::vector<SType*> const& parametricScopes() const = 0;

		CRAFT_TYPES_EXPORTED virtual bool isParametricConsumer() const = 0;

		/* This method provides the for the consumption of parametric variables.

		*/
		CRAFT_TYPES_EXPORTED virtual std::vector<SType*> const& parametricConsumption() const = 0;

	//
	// Subtype Hierarchy
	//
	public:
		CRAFT_TYPES_EXPORTED virtual bool isSubtypeHierarchyParticipant() const = 0;
		CRAFT_TYPES_EXPORTED virtual bool canHaveChildren() const = 0;
		CRAFT_TYPES_EXPORTED virtual bool canHaveAdoptiveParents() const = 0;

	//
	// Algorithms
	//
	public:
		CRAFT_TYPES_EXPORTED virtual void subtypingDo(SubtypeAlgorithmState*) const = 0;
		CRAFT_TYPES_EXPORTED virtual void validationDo(ValidationAlgorithmState*) const = 0;
		CRAFT_TYPES_EXPORTED virtual void validationDo(ValidationWithSubtypingAlgorithmState*) const = 0;

	//
	// Internal
	//
	protected:
		friend class TypeGraphSystem;

		CRAFT_TYPES_EXPORTED virtual void _registerTypeIdentifier(TypeId const&);
	};
}}
