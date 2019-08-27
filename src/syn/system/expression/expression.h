#pragma once
#include "syn/syn.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Expression
	******************************************************************************/

	struct IExpression
	{
		virtual TypeId kind() const = 0;
		virtual void* ptr() const = 0;

		virtual std::string displayString() const = 0;
		virtual std::vector<IExpression*> const* children() const = 0;

		virtual void destroy() = 0;
		virtual IExpression* clone() const = 0;

	public:
		inline std::tuple<void*, void*> invokeArg() const { return std::make_tuple((Graph::Node*)kind(), ptr()); }
	};

	/******************************************************************************
	** Expression Specials
	******************************************************************************/

	struct ExpressionAny final
		: public IExpression
	{
	private:
		ExpressionAny() = default;
		~ExpressionAny() = default;
	public:
		CULTLANG_SYNDICATE_EXPORTED static ExpressionAny Value;

		CULTLANG_SYNDICATE_EXPORTED virtual TypeId kind() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual void* ptr() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual std::string displayString() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual void destroy() override;
		CULTLANG_SYNDICATE_EXPORTED virtual IExpression* clone() const override;
	};

	struct ExpressionVoid final
		: public IExpression
	{
	private:
		ExpressionVoid() = default;
		~ExpressionVoid() = default;
	public:
		CULTLANG_SYNDICATE_EXPORTED static ExpressionVoid Value;

		CULTLANG_SYNDICATE_EXPORTED virtual TypeId kind() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual void* ptr() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual std::string displayString() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual void destroy() override;
		CULTLANG_SYNDICATE_EXPORTED virtual IExpression* clone() const override;
	};

	struct ExpressionBottom final
		: public IExpression
	{
	private:
		ExpressionBottom() = default;
		~ExpressionBottom() = default;
	public:
		CULTLANG_SYNDICATE_EXPORTED static ExpressionBottom Value;

		CULTLANG_SYNDICATE_EXPORTED virtual TypeId kind() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual void* ptr() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual std::string displayString() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual void destroy() override;
		CULTLANG_SYNDICATE_EXPORTED virtual IExpression* clone() const override;
	};

	/******************************************************************************
	** ExpressionConcrete
	******************************************************************************/

	struct ExpressionConcrete final
		: public IExpression
	{
	public:
		Graph::Node* node;

		CULTLANG_SYNDICATE_EXPORTED ExpressionConcrete(TypeId node);
		CULTLANG_SYNDICATE_EXPORTED ~ExpressionConcrete();

	public:
		CULTLANG_SYNDICATE_EXPORTED virtual TypeId kind() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual void* ptr() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual std::string displayString() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual void destroy() override;
		CULTLANG_SYNDICATE_EXPORTED virtual IExpression* clone() const override;
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

		CULTLANG_SYNDICATE_EXPORTED ExpressionArrow(IExpression* input, IExpression* output);
		CULTLANG_SYNDICATE_EXPORTED ~ExpressionArrow();

	public:
		CULTLANG_SYNDICATE_EXPORTED virtual TypeId kind() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual void* ptr() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual std::string displayString() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual void destroy() override;
		CULTLANG_SYNDICATE_EXPORTED virtual IExpression* clone() const override;
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

		CULTLANG_SYNDICATE_EXPORTED ExpressionTuple(std::vector<IExpression*> const&, IExpression* varType = nullptr);
		CULTLANG_SYNDICATE_EXPORTED ~ExpressionTuple();

	public:
		CULTLANG_SYNDICATE_EXPORTED virtual TypeId kind() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual void* ptr() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual std::string displayString() const override;
		CULTLANG_SYNDICATE_EXPORTED virtual std::vector<IExpression*> const* children() const override;

		CULTLANG_SYNDICATE_EXPORTED virtual void destroy() override;
		CULTLANG_SYNDICATE_EXPORTED virtual IExpression* clone() const override;
	};

	/******************************************************************************
	** ExpressionStore
	******************************************************************************/

	class ExpressionStore final
	{
		std::shared_ptr<IExpression> _root;

		bool _building;

	public:
		CULTLANG_SYNDICATE_EXPORTED ExpressionStore();
		CULTLANG_SYNDICATE_EXPORTED ExpressionStore(IExpression* ptr);
		CULTLANG_SYNDICATE_EXPORTED ExpressionStore(ExpressionStore const&);
		CULTLANG_SYNDICATE_EXPORTED ~ExpressionStore();

		CULTLANG_SYNDICATE_EXPORTED void finish();

		CULTLANG_SYNDICATE_EXPORTED IExpression const* root()  const;
	};
}}
