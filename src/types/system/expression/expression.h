#pragma once
#include "../../common.h"
#include "../../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Expression
	******************************************************************************/

	struct Expression
	{
		virtual ~Expression() = 0;

		virtual Graph::Node kind() const = 0;
		virtual std::string const& displayString() const = 0;
		virtual std::vector<Expression*> const* children() const = 0;

		virtual Expression* clone() const = 0;
	};

	/******************************************************************************
	** ExpressionConcrete
	******************************************************************************/

	struct ExpressionConcrete sealed
		: public Expression
	{
	public:
		Graph::Node node;

		CRAFT_TYPES_EXPORTED ExpressionConcrete(Graph::Node const& node);

	public:
		CRAFT_TYPES_EXPORTED virtual ~ExpressionConcrete();

		CRAFT_TYPES_EXPORTED virtual Graph::Node kind() const override;
		CRAFT_TYPES_EXPORTED virtual std::string const& displayString() const override;
		CRAFT_TYPES_EXPORTED virtual std::vector<Expression*> const* children() const override;

		CRAFT_TYPES_EXPORTED virtual Expression* clone() const override;
	};

	/******************************************************************************
	** ExpressionStore
	******************************************************************************/

	class ExpressionStore final
	{
		Expression* _root;

		bool _building;

	public:
		CRAFT_TYPES_EXPORTED ExpressionStore();
		CRAFT_TYPES_EXPORTED ExpressionStore(ExpressionStore const&);
		CRAFT_TYPES_EXPORTED ~ExpressionStore();

		CRAFT_TYPES_EXPORTED void finish();

		CRAFT_TYPES_EXPORTED Expression const* root();
	};
}}
