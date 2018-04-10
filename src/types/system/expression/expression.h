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

		virtual std::string displayString() const = 0;
		virtual std::vector<IExpression*> const* children() const = 0;

		virtual IExpression* clone() const = 0;

	public:
		inline std::tuple<void*, void*> invokeArg() { return std::make_tuple(kind().ptr(), ptr()); }
	};

	/******************************************************************************
	** ExpressionConcrete
	******************************************************************************/

	struct ExpressionConcrete final
		: public IExpression
	{
	public:
		Graph::Node node;

		CRAFT_TYPES_EXPORTED ExpressionConcrete(Graph::Node const& node);

	public:
		CRAFT_TYPES_EXPORTED virtual ~ExpressionConcrete();

		CRAFT_TYPES_EXPORTED virtual Graph::Node kind() const override;
		CRAFT_TYPES_EXPORTED virtual void* ptr() const override;

		CRAFT_TYPES_EXPORTED virtual std::string displayString() const override;
		CRAFT_TYPES_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CRAFT_TYPES_EXPORTED virtual IExpression* clone() const override;
	};

	/******************************************************************************
	** ExpressionArrow
	******************************************************************************/

	struct ExpressionArrow final
		: public IExpression
	{
	public:
		IExpression* input;
		IExpression* output;

		CRAFT_TYPES_EXPORTED ExpressionArrow(IExpression* input, IExpression* output);

	public:
		CRAFT_TYPES_EXPORTED virtual ~ExpressionArrow();

		CRAFT_TYPES_EXPORTED virtual Graph::Node kind() const override;
		CRAFT_TYPES_EXPORTED virtual void* ptr() const override;

		CRAFT_TYPES_EXPORTED virtual std::string displayString() const override;
		CRAFT_TYPES_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CRAFT_TYPES_EXPORTED virtual IExpression* clone() const override;
	};

	/******************************************************************************
	** ExpressionTuple
	******************************************************************************/

	struct ExpressionTuple final
		: public IExpression
	{
	public:
		std::vector<IExpression*> entries;

		CRAFT_TYPES_EXPORTED ExpressionTuple(std::vector<IExpression*> const&);

	public:
		CRAFT_TYPES_EXPORTED virtual ~ExpressionTuple();

		CRAFT_TYPES_EXPORTED virtual Graph::Node kind() const override;
		CRAFT_TYPES_EXPORTED virtual void* ptr() const override;

		CRAFT_TYPES_EXPORTED virtual std::string displayString() const override;
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
		CRAFT_TYPES_EXPORTED ExpressionStore(IExpression* ptr);
		CRAFT_TYPES_EXPORTED ExpressionStore(ExpressionStore const&);
		CRAFT_TYPES_EXPORTED ~ExpressionStore();

		CRAFT_TYPES_EXPORTED void finish();

		CRAFT_TYPES_EXPORTED IExpression const* root();
	};
}}
