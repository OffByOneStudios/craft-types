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
		do
		{
			std::getline(std::cin, input);
		} while (input.size() == 0);
		

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
			else if (input == "system-entries")
			{
				auto dll_count = system().getLibraryCount();
				for (int i = 0; i < dll_count; ++i)
				{
					auto dll_name = system().getLibraryName(i);
					std::cout << std::endl << "=== DLL: " << dll_name << std::endl;

					auto entry_count = system().getLibraryEntryCount(i);
					for (int j = 0; j < entry_count; ++j)
					{
						auto entry = system().getLibraryEntry(i, j);
						switch (entry.kind)
						{
							case CppSystem::EntryKind::Marker:
								std::cout << "INFO: " << entry.message << std::endl;
								break;
							case CppSystem::EntryKind::Warning:
								std::cout << "WARN: " << entry.message << std::endl;
								break;
							case CppSystem::EntryKind::StaticDefine:
								std::cout << "TYPE: " << entry.type << std::endl;
								break;
						}
					}
				}
			}
			else if (input == "symbols")
			{
				std::cout << std::endl << "Symbols:";
				for (int i = 1; i <= global_store().s().count(); ++i)
				{
					std::cout << " " << global_store().s().getString((Symbol)i);
				}
				std::cout << "." << std::endl;
			}
			else if (input == "load")
			{
				auto abs = std::filesystem::absolute(args);
				auto filename = abs.filename();

				std::cout << std::endl << "changing directory to " << abs.string() << std::endl;
				abs.remove_filename();
				std::filesystem::current_path(abs);

				std::cout << std::endl << "attempting load of " << filename.string() << std::endl;
				syn::dll::load(filename.string());

				auto last_dll_index = system().getLibraryCount() - 1;
				auto last_dll_name = system().getLibraryName(last_dll_index);
				auto last_dll_count = system().getLibraryEntryCount(last_dll_index);

				std::cout << std::endl << "loaded " << last_dll_name << " with " << last_dll_count << " types." << std::endl;
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