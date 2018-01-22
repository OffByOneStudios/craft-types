#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** PObjectContextual
	******************************************************************************/

	class Context;

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class PObjectContextual abstract
		: public Provider
	{
		CRAFT_TYPES_EXPORTED CRAFT_PROVIDER_DECLARE(PObjectContextual, "types.context", SingletonProviderManager);

	public:
		virtual void expand(instance<>, std::shared_ptr<Context>) = 0;
		virtual void contextualize(instance<>, std::shared_ptr<Context>) = 0;
		virtual void finalize(instance<>, std::shared_ptr<Context>) = 0;
	};

	/******************************************************************************
	** ObjectConversion
	******************************************************************************/

	template <typename TType>
	class ObjectContextualWrapper
		: public Implements<PObjectContextual>::For<TType>
	{
	public:
		typedef void (TType::*t_expand)(std::shared_ptr<Context> c);
		typedef void (TType::*t_context)(std::shared_ptr<Context> c);
		typedef void (TType::*t_finalize)(std::shared_ptr<Context> c);

	private:
		t_expand _expand_func;
		t_context _context_func;
		t_finalize _finalize_func;

		inline virtual void expand(instance<> i, std::shared_ptr<Context> c)
		{
			if (_expand_func == nullptr) return;
			(i.asType<TType>().get()->*(this->_expand_func))(c);
		}

		inline virtual void contextualize(instance<> i, std::shared_ptr<Context> c)
		{
			if (_context_func == nullptr) return;
 		  (i.asType<TType>().get()->*(this->_context_func))(c);
		}

		inline virtual void finalize(instance<> i, std::shared_ptr<Context> c)
		{
			if (_finalize_func == nullptr) return;
			(i.asType<TType>().get()->*(this->_finalize_func))(c);
		}
	public:

		ObjectContextualWrapper()
		{
			_expand_func = nullptr;
			_context_func = nullptr;
			_finalize_func = nullptr;
		}

		inline ObjectContextualWrapper<TType>* withExpander(t_expand f) { _expand_func = f; return this; }
		inline ObjectContextualWrapper<TType>* withContextualizer(t_context f) { _context_func = f; return this; }
		inline ObjectContextualWrapper<TType>* withFinalizer(t_finalize f) { _finalize_func = f; return this; }
	};
}}
