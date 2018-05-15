#include "types/common.h"
#include "types/core.h"

#ifdef _MSC_VER

#include "Windows.h"

extern "C" BOOLEAN __stdcall _DllMainCRTStartup(LPVOID var1, DWORD var2, LPVOID var3);

extern "C" BOOLEAN __stdcall _craft_types_DLLMAIN(LPVOID var1, DWORD var2, LPVOID var3)
{
	if (var2 == DLL_PROCESS_ATTACH)
	{
		auto save = ::craft::types::_dll_begin(CRAFT_CURRENT_PACKAGE_NAMESPACE "/" CRAFT_CURRENT_PACKAGE_ID "/" CRAFT_CURRENT_ARTIFACT_NAME);
		auto ret = _DllMainCRTStartup(var1, var2, var3);
		::craft::types::_dll_finish(save);
		return ret;
	}
	else
		return _DllMainCRTStartup(var1, var2, var3);
}

#endif

#ifdef __GNUC__
__attribute__((constructor)) void _craft_types_DLLMAIN()
{
	/*
	auto save = ::craft::types::_dll_begin(CRAFT_CURRENT_PACKAGE_NAMESPACE "/" CRAFT_CURRENT_PACKAGE_ID "/" CRAFT_CURRENT_ARTIFACT_NAME);
	
	::craft::types::_dll_finish(save);
	return ret;
	*/
}
#endif 