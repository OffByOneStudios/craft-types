#pragma once
#include "syn/syn.h"

namespace syn
{
	/******************************************************************************
	** CppSystem
	******************************************************************************/

	class CppSystem final
	{
	public:
		enum class EntryKind
		{
			StaticDefine,
			Marker,
			Warning
		};

	private:
		struct _Entry
		{
			void* ptr;
			EntryKind kind;
		};

		struct _Entries
		{
			std::string name;
			std::vector<_Entry> entries;
		};

	private:
		// These first for inlined functions
		// Graph for this cpp-system (todo: invert this, graph is also a static)
		TypeStore* _store;

		std::recursive_mutex operation;

		_Entries* _staticEntries;

		_Entries* _currentDllEntries;
		std::vector<_Entries*> _dllEntries;
		std::map<std::string, size_t> _dllNames;
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
		friend inline void ::syn::dll::update();
		friend inline void ::syn::dll::reset();
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
		CULTLANG_SYNDICATE_EXPORTED void _clear();

		//
		// Registry
		//
	private:
		void _addEntry(_Entry &&);

	protected:

	public:
		inline TypeStore& types() { return *_store; }

		CULTLANG_SYNDICATE_EXPORTED void _register(CppDefine const*);

		//
		// Entries and DLLs
		//
	public:
		struct Entry
		{
			EntryKind kind;
			TypePtr type;
			std::string message;
		};

		CULTLANG_SYNDICATE_EXPORTED size_t getLibraryCount() const;
		CULTLANG_SYNDICATE_EXPORTED std::string getLibraryName(size_t dll_index) const;
		CULTLANG_SYNDICATE_EXPORTED std::string getCurrentLibraryName() const;
		CULTLANG_SYNDICATE_EXPORTED size_t getLibraryEntryCount(size_t dll_index) const;
		CULTLANG_SYNDICATE_EXPORTED Entry getLibraryEntry(size_t dll_index, size_t entry_index) const;
	};
}