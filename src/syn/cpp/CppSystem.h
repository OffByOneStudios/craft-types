#pragma once
#include "syn/syn.h"

namespace syn
{
	/******************************************************************************
	** CppSystem
	******************************************************************************/

	class CppSystem final
	{
	private:

		struct _Entry
		{
			enum class Kind
			{
				StaticDefine,
				Marker,
				Warning
			};

			void* ptr;
			Kind kind;
		};

		struct _Entries
		{
			std::vector<_Entry> _entries;
		};

	private:
		// These first for inlined functions
		// Graph for this cpp-system (todo: invert this, graph is also a static)
		TypeStore* _store;

		std::recursive_mutex operation;

		_Entries* _static_entries;

		_Entries* _current_dll_entries;
		std::map<std::string, _Entries*> _dll_entries;
		std::set<std::string> _dllsToUpdate;
		std::set<std::string> _dllsThatWereStatic;

		std::string _lastLoadedDll;

		// 
		// Lifecycle
		//
	private:
		CppSystem();
		~CppSystem();

	public:
		CULTLANG_SYNDICATE_EXPORTED static CppSystem& global_instance();

	private:
		friend inline void ::syn::dll::boot();
		friend inline char const* ::syn::dll::_begin(char const*);
		friend inline void ::syn::dll::_finish(char const*, char const*);

		friend inline void ::syn::dll::load(std::string const&);

		void _init_primeInternalEntries();
		void _init_insertEntries(_Entries* entries, size_t start);
		void _init_runEntries(_Entries* entries, size_t start);

		static char const* __dll_region;

		CULTLANG_SYNDICATE_EXPORTED void _init();
		CULTLANG_SYNDICATE_EXPORTED bool _hasInited();
		CULTLANG_SYNDICATE_EXPORTED static char const* _begin(char const* name);
		CULTLANG_SYNDICATE_EXPORTED void _finish(char const* save, char const* name);
		CULTLANG_SYNDICATE_EXPORTED void _update();

		//
		// Registry
		//
	private:
		void _addEntry(_Entry &&);

	protected:

	public:
		inline TypeStore& types() { return *_store; }

		CULTLANG_SYNDICATE_EXPORTED void _register(CppDefine const*);

		CULTLANG_SYNDICATE_EXPORTED std::string getLastLibraryName();
		CULTLANG_SYNDICATE_EXPORTED size_t getLibraryCount(std::string const& dll);
		CULTLANG_SYNDICATE_EXPORTED TypePtr getLibraryEntry(std::string const& dll, size_t index);
	};
}