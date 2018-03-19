#include "common.h"
#include "core.h"

using namespace craft::types;


/******************************************************************************
** BasicObjectFungible
******************************************************************************/

Signal<void(instance<> origInst, instance<> newInst)>& BasicObjectFungible::objectFungible_onUpdate()
{
	return _fungible_signal;
}
