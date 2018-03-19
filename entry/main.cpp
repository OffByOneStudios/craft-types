#include "types/common.h"
#include "types/core.h"

using namespace craft;
using namespace craft::types;



namespace _impl
{
	std::shared_ptr<spdlog::logger> console;


}

namespace entry
{
	class ThingOne
		: public virtual Object
	{
		CRAFT_OBJECT_DECLARE(entry::ThingOne);

	public:
		std::string fishtacos;

		ThingOne(std::string f)
			: fishtacos(f)
		{

		}

		void expand(std::shared_ptr<Context> c)
		{
			_impl::console->info("expand");
			return;
		}
		void contextualize(std::shared_ptr<Context>)
		{
			_impl::console->info("contextualize");
		}

		void finalize(std::shared_ptr<Context>)
		{
			_impl::console->info("finalize");
		}
	};

	class ContextHost
		: public virtual Object
	{
		CRAFT_OBJECT_DECLARE(entry::ContextHost);

	private:
		// SubContexts
		std::shared_ptr<types::Context> _example_context_0;
		std::shared_ptr<types::Context> _example_context_1;

	public:
		ContextHost()
		{
			
		}

		void expand(std::shared_ptr<Context> c)
		{
			_impl::console->info("expand");
			
			_example_context_0 = types::Context::makeChildContext(c, craft_instance());
			_example_context_1 = types::Context::makeChildContext(c, craft_instance());

			c->add(instance<ThingOne>::make("Thing1"));
			_example_context_0->add(instance<ThingOne>::make("Thing2"));
			_example_context_1->add(instance<ThingOne>::make("Thing3"));
		}

		void contextualize(std::shared_ptr<Context>)
		{
			_impl::console->info("contextualize");
		}

		void finalize(std::shared_ptr<Context>)
		{
			_impl::console->info("finalize");
		}
	};
}


CRAFT_DEFINE(entry::ThingOne)
{
	_.use<PObjectContextual>().byConfiguring<ObjectContextualWrapper>()
		->withExpander(&entry::ThingOne::expand)
		->withFinalizer(&entry::ThingOne::finalize)
		->withContextualizer(&entry::ThingOne::contextualize);

	_.defaults();
}

CRAFT_DEFINE(entry::ContextHost)
{
	_.use<PObjectContextual>().byConfiguring<ObjectContextualWrapper>()
		->withExpander(&entry::ContextHost::expand)
		->withFinalizer(&entry::ContextHost::finalize)
		->withContextualizer(&entry::ContextHost::contextualize);
	_.defaults();
}

int main(int argc, char** argv)
{
	craft::types::boot_with_cpp();

	_impl::console = spdlog::stdout_color_mt("console");
	// Init ur config
	//types::system().getManager<PObjectContextual>()->
	auto host = instance<entry::ContextHost>::make();
	
	_impl::console->info("Configuring Host");
	
	auto _root_context = std::make_shared<types::Context>(nullptr);

	_root_context->add(host);
	_root_context->fishtacos();
	
}
