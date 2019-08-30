#include "syn/syn.h"
#include "SymbolTable.h"

using namespace syn;

/******************************************************************************
** SymbolTable
******************************************************************************/

SymbolTable::SymbolTable()
    : _id(0)
    , _strToId()
    , _idToStr()
{ }

std::string SymbolTable::getString(Symbol sym) const
{
    return _idToStr.at(sym);
}

Symbol SymbolTable::getSymbol(std::string const& str) const
{
    return _strToId.at(str);
}

Symbol SymbolTable::require(std::string const& str)
{
    decltype(_strToId)::iterator lb = _strToId.lower_bound(str);

    if(lb != _strToId.end() && !(_strToId.key_comp()(str, lb->first)))
    {
        return lb->second;
    }
    else
    {
        auto sym = (Symbol)++_id;

        _strToId.insert(lb, decltype(_strToId)::value_type(str, sym));
        _idToStr[sym] = str;

        return sym;
    }
}

size_t SymbolTable::count() const
{
    return _id;
}

/******************************************************************************
** TypeStore
******************************************************************************/

/*
CRAFT_TYPE_DEFINE(syn::TypeStore)
{
	_.identify_byFullCppName("syn::TypeStore");
	_.defaults();
}
*/

/******************************************************************************
** Symbol
******************************************************************************/
syn::Define<syn::Symbol> syn::type_define<syn::Symbol>::Definition(
	[](auto _) {
		_.name("Symbol");
        
        _.detectLifecycle();
	});