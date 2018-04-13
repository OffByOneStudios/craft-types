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
		virtual Graph::Node kind() const = 0;
		virtual void* ptr() const = 0;

		virtual std::string displayString() const = 0;
		virtual std::vector<IExpression*> const* children() const = 0;

		virtual void destroy() = 0;
		virtual IExpression* clone() const = 0;

	public:
		inline std::tuple<void*, void*> invokeArg() { return std::make_tuple(kind().ptr(), ptr()); }
	};

	/******************************************************************************
	** ExpressionSpecial
	******************************************************************************/

	struct ExpressionSpecial final
		: public IExpression
	{
	private:
		Graph::Node node;
		friend void __special_init_ExpressionSpecial();
		CRAFT_TYPES_EXPORTED ExpressionSpecial(Graph::Node const& node);
		CRAFT_TYPES_EXPORTED ~ExpressionSpecial();

	public:

		CRAFT_TYPES_EXPORTED virtual Graph::Node kind() const override;
		CRAFT_TYPES_EXPORTED virtual void* ptr() const override;

		CRAFT_TYPES_EXPORTED virtual std::string displayString() const override;
		CRAFT_TYPES_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CRAFT_TYPES_EXPORTED virtual void destroy() override;
		CRAFT_TYPES_EXPORTED virtual IExpression* clone() const override;

	public:
		CRAFT_TYPES_EXPORTED static ExpressionSpecial* Void; // None/Nothing
		CRAFT_TYPES_EXPORTED static ExpressionSpecial* Any;
		CRAFT_TYPES_EXPORTED static ExpressionSpecial* Bottom;
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
		CRAFT_TYPES_EXPORTED ~ExpressionConcrete();

	public:
		CRAFT_TYPES_EXPORTED virtual Graph::Node kind() const override;
		CRAFT_TYPES_EXPORTED virtual void* ptr() const override;

		CRAFT_TYPES_EXPORTED virtual std::string displayString() const override;
		CRAFT_TYPES_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CRAFT_TYPES_EXPORTED virtual void destroy() override;
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
		CRAFT_TYPES_EXPORTED ~ExpressionArrow();

	public:
		CRAFT_TYPES_EXPORTED virtual Graph::Node kind() const override;
		CRAFT_TYPES_EXPORTED virtual void* ptr() const override;

		CRAFT_TYPES_EXPORTED virtual std::string displayString() const override;
		CRAFT_TYPES_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CRAFT_TYPES_EXPORTED virtual void destroy() override;
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
		IExpression* varType;

		CRAFT_TYPES_EXPORTED ExpressionTuple(std::vector<IExpression*> const&, IExpression* varType = nullptr);
		CRAFT_TYPES_EXPORTED ~ExpressionTuple();

	public:
		CRAFT_TYPES_EXPORTED virtual Graph::Node kind() const override;
		CRAFT_TYPES_EXPORTED virtual void* ptr() const override;

		CRAFT_TYPES_EXPORTED virtual std::string displayString() const override;
		CRAFT_TYPES_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CRAFT_TYPES_EXPORTED virtual void destroy() override;
		CRAFT_TYPES_EXPORTED virtual IExpression* clone() const override;
	};

	/******************************************************************************
	** ExpressionStore
	******************************************************************************/

	class ExpressionStore final
	{
		std::shared_ptr<IExpression> _root;

		bool _building;

	public:
		CRAFT_TYPES_EXPORTED ExpressionStore();
		CRAFT_TYPES_EXPORTED ExpressionStore(IExpression* ptr);
		CRAFT_TYPES_EXPORTED ExpressionStore(ExpressionStore const&);
		CRAFT_TYPES_EXPORTED ~ExpressionStore();

		CRAFT_TYPES_EXPORTED void finish();

		CRAFT_TYPES_EXPORTED IExpression const* root()  const;
	};
}}
