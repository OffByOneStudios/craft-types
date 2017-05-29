#include "craft/common.h"
#include "core.h"

using namespace craft;

TypeId craft::_details::type_impl::_id = 1;
FeatureId Feature::_id = 1;

Object::~Object() { }

/******************************************************************************
** Types
******************************************************************************/

Types& Types::instance()
{
	static Types* _types_instance = new Types();
	return *_types_instance;
}

Types::Types()
{

}

void Types::init()
{
	for (auto i : _initers)
	{
		i.second(*this);
	}
}

unsigned char __offset_table[craft::_details::_offset_table_size] = {};
unsigned char* craft::_details::_offset_table = __offset_table;

void ::craft::_details::_register_type_init(TypeId id, _fn_register_type_init _init)
{
	types()._initers[id] = _init;
}

/******************************************************************************
** BasicObjectFungible
******************************************************************************/

Signal<void(instance<> origInst, instance<> newInst)>& BasicObjectFungible::objectFungible_onUpdate()
{
	return _fungible_signal;
}
