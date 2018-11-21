#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Declared in system/store.h
	******************************************************************************/

    std::string TypeId::identifier() const
    {
        auto prop = thread_store().graph().onlyPropOfTypeOnNode<Type_Property_NamespaceIdentifier>(_node);
        return prop ? prop->namespace_identifier : "";
    }
}}
