#pragma once
#include "../../common.h"
#include "../../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Expression
	******************************************************************************/

	struct IExpression
	{
		virtual ~IExpression() = 0;

		virtual Graph::Node kind() const = 0;
		virtual void* ptr() const = 0;

		virtual std::string const& displayString() const = 0;
		virtual std::vector<IExpression*> const* children() const = 0;

		virtual IExpression* clone() const = 0;

	public:
		inline std::tuple<void*, void*> invokeArg() { return std::make_tuple(kind().ptr(), ptr()); }
	};

	/******************************************************************************
	** ExpressionConcrete
	******************************************************************************/

	struct ExpressionConcrete sealed
		: public IExpression
	{
	public:
		Graph::Node node;

		CRAFT_TYPES_EXPORTED ExpressionConcrete(Graph::Node const& node);

	public:
		CRAFT_TYPES_EXPORTED virtual ~ExpressionConcrete();

		CRAFT_TYPES_EXPORTED virtual Graph::Node kind() const override;
		CRAFT_TYPES_EXPORTED virtual void* ptr() const override;

		CRAFT_TYPES_EXPORTED virtual std::string const& displayString() const override;
		CRAFT_TYPES_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CRAFT_TYPES_EXPORTED virtual IExpression* clone() const override;
	};

	/******************************************************************************
	** ExpressionStore
	******************************************************************************/

	class ExpressionStore final
	{
		IExpression* _root;

		bool _building;

	public:
		CRAFT_TYPES_EXPORTED ExpressionStore();
		CRAFT_TYPES_EXPORTED ExpressionStore(ExpressionStore const&);
		CRAFT_TYPES_EXPORTED ~ExpressionStore();

		CRAFT_TYPES_EXPORTED void finish();

		CRAFT_TYPES_EXPORTED IExpression const* root();
	};
}}
