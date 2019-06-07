
#include <functional>

/******************************************************************************
** bind extension
******************************************************************************/

namespace stdext
{
    namespace pl = std::placeholders;
    
	template<typename TThis, typename TRet, typename TClass>
	inline std::function<TRet()> 
		bindmem(TThis _this, TRet(TClass::*f)())
	{
		return std::bind(f, static_cast<TClass*>(_this));
	}

	template<typename TThis, typename TRet, typename TClass
		, typename TParam1>
	inline std::function<TRet(TParam1)> 
		bindmem(TThis _this, TRet(TClass::*f)(TParam1))
	{
		return std::bind(f, static_cast<TClass*>(_this), pl::_1);
	}

	template<typename TThis, typename TRet, typename TClass
		, typename TParam1, typename TParam2>
	inline std::function<TRet(TParam1, TParam2)> 
		bindmem(TThis _this, TRet(TClass::*f)(TParam1, TParam2))
	{
        return std::bind(f, static_cast<TClass*>(_this), pl::_1, pl::_2);
	}

	template<typename TThis, typename TRet, typename TClass
		, typename TParam1, typename TParam2, typename TParam3>
	inline std::function<TRet(TParam1, TParam2, TParam3)>
		bindmem(TThis _this, TRet(TClass::*f)(TParam1, TParam2, TParam3))
	{
        return std::bind(f, static_cast<TClass*>(_this), pl::_1, pl::_2, pl::_3);
	}

	template<typename TThis, typename TRet, typename TClass
		, typename TParam1, typename TParam2, typename TParam3, typename TParam4>
	inline std::function<TRet(TParam1, TParam2, TParam3, TParam4)> 
		bindmem(TThis _this, TRet(TClass::*f)(TParam1, TParam2, TParam3, TParam4))
	{
		return std::bind(f, static_cast<TClass*>(_this), pl::_1, pl::_2, pl::_3, pl::_4);
	}
}