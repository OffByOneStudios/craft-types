#include "common.h"
#include "core.h"

using namespace craft::types;

TypeId craft::types::_details::type_impl::_id = 1;
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
	assert(_initersForFeatures.size() != 0);
	assert(_initersForTypes.size() != 0);

	for (auto i : _initersForFeatures)
	{
		i.second();
	}
	_initersForFeatures.clear();

	for (auto i : _initersForTypes)
	{
		i.second(*this);
	}
	_initersForTypes.clear();
}

unsigned char __offset_table[craft::types::_details::_offset_table_size] = {};
unsigned char* craft::types::_details::_offset_table = __offset_table;

void ::craft::types::_details::_register_type_init(TypeId id, _fn_register_type_init _init)
{
	system()._initersForTypes[id] = _init;
}

void ::craft::types::_details::_register_feature_init(FeatureId id, _fn_register_feature_init _init)
{
	system()._initersForFeatures[id] = _init;
}

/******************************************************************************
** BasicObjectFungible
******************************************************************************/

Signal<void(instance<> origInst, instance<> newInst)>& BasicObjectFungible::objectFungible_onUpdate()
{
	return _fungible_signal;
}
