#include "syn/common.h"
#include "syn/syn.h"

using namespace craft;
using namespace syn;

int main(int argc, char** argv)
{
	syn::dll::boot();

	for (;;)
	{
		std::cout << ">>> ";
		std::string input;
		std::getline(std::cin, input);

		bool has_args = true;

		auto cmd_break = input.find(' ');
		if (cmd_break == std::string::npos) has_args = false;
		
		std::string args;
		if (has_args)
		{
			args = input.substr(cmd_break + 1);
			input = input.substr(0, cmd_break);
		}
		else
			cmd_break = input.size();

		std::cout << std::endl;

		auto curcwd = std::filesystem::current_path();
		curcwd = std::filesystem::absolute(curcwd);
		try
		{
			if (input == "dump")
			{
				if (has_args)
				{

				}
				else
				{
					global_store().g().forAllNodes(
						[](Graph::Node const* n)
					{
						std::cout << std::string(80, '=') << std::endl;
						std::cout << global_store().describeNode(n) << std::endl;
					});
				}
			}
			if (input == "symbols")
			{
				std::cout << std::endl << "Symbols: ";
				for (int i = 1; i <= global_store().s().count(); ++i)
				{
					std::cout << global_store().s().getString((Symbol)i);
				}
				std::cout << "." << std::endl;
			}
			else if (input == "load")
			{
				auto abs = std::filesystem::absolute(args);
				std::filesystem::current_path(abs.remove_filename());

				syn::dll::load(abs.string());
				auto last_dll = system().getLastLibraryName();
				auto end = system().getLibraryCount(last_dll);

				for (size_t i =0; i < end; ++i)
					std::cout << global_store().describeNode(system().getLibraryEntry(last_dll, i).asId()) << std::endl;

				std::cout << std::endl << "loaded " << (end) << " types." << std::endl;
			}
			else
				throw stdext::exception("unknown command `{0}`", input);
		}
		catch (std::exception const& ex)
		{
			std::cout << ex.what() << std::endl;
		}
		std::filesystem::current_path(curcwd);

		std::cout << std::endl;
	}
}

// TODO move to tests:
/*
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
	syn::boot();

	_impl::console = spdlog::stdout_color_mt("console");
	// Init ur config
	//types::system().getManager<PObjectContextual>()->
	auto host = instance<entry::ContextHost>::make();
	
	_impl::console->info("Configuring Host");
	
	auto _root_context = std::make_shared<types::Context>(nullptr);

	_root_context->add(host);
	_root_context->fishtacos();
	
}
*/