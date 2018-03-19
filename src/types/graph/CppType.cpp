#include "../common.h"
#include "../core.h"
#include "CppType.h"

using namespace craft;
using namespace craft::types;


CppType::CppType(cpp::TypePtr desc)
{
	_desc = desc;
}

CppType::~CppType()
{

}