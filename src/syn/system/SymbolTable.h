#pragma once
#include "syn/syn.h"

namespace syn
{
	/******************************************************************************
	** SymbolTable
	******************************************************************************/

	class SymbolTable
	{
        private:
            uintptr_t _id;

            // TODO replace this implementation
            std::map<std::string, Symbol> _strToId;
            std::map<Symbol, std::string> _idToStr;

        public:
            SymbolTable();

            size_t count() const;

            std::string getString(Symbol) const;
            Symbol getSymbol(std::string const&) const;

            Symbol require(std::string const&);
        
        public:

        template<typename TStore, TStore default_value>
        class Lookup
        {
            private:
                std::map<Symbol, TStore> _map;
                SymbolTable& _table;

            public:
                Lookup(SymbolTable& table)
                    : _table(table)
                { }

                bool has(Symbol sym)
                {
                    auto it = _map.find(sym);

                    return it != _map.end();
                }

                TStore lookup(Symbol sym)
                {
                    auto it = _map.find(sym);

                    return it != _map.end()
                        ? *it
                        : default_value;
                }

                void set(Symbol sym, TStore const& value)
                {
                    _map[sym] = value;
                }

                void erase(Symbol sym)
                {
                    _map.erase(sym);
                }

                void clear()
                {
                    _map.clear();
                }
        };
	};
}
