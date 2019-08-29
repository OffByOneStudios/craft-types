#include "syn/syn.h"

#ifdef _MSC_VER
#include "Windows.h"

extern "C" BOOLEAN __stdcall _DllMainCRTStartup(LPVOID var1, DWORD var2, LPVOID var3);

extern "C" BOOLEAN __stdcall syn_DLLMAIN(LPVOID var1, DWORD var2, LPVOID var3)
{
	if (var2 == DLL_PROCESS_ATTACH)
	{
		const char* pkg = 
			#ifdef CULT_CURRENT_PACKAGE
			CULT_CURRENT_PACKAGE
			#else
			"<undefined>"
			#endif
			;
		auto save = ::syn::dll::_begin(pkg);
		auto ret = _DllMainCRTStartup(var1, var2, var3);
		::syn::dll::_finish(save);
		return ret;
	}
	else
		return _DllMainCRTStartup(var1, var2, var3);
}
#endif

#ifdef __GNUC__
__attribute__((visibility ("internal"))) 
__attribute__((constructor (1000)))
// runs after other static initalizers
// we use a "clean up after other initializers" strategy here.
void _syn_DLLMAIN_finish()
{
	const char* pkg = 
		#ifdef CULT_CURRENT_PACKAGE
		CULT_CURRENT_PACKAGE
		#else
		"<undefined>"
		#endif
		;
	::syn::dll::_finish(nullptr, pkg);
}
#endif
