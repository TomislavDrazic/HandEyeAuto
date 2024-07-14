#ifndef __NXLIBDYNAMIC_SRC_H__
#define __NXLIBDYNAMIC_SRC_H__

#include "nxLibDynamic.h"

#ifndef NXLIB_ATTR_UNUSED
#	if defined(__GNUC__)
#		define NXLIB_ATTR_UNUSED __attribute__((unused))
#	else
#		define NXLIB_ATTR_UNUSED
#	endif
#endif

#ifdef _WIN32
#	define NOMINMAX
#	pragma warning(push)
#	pragma warning(disable : 4255)
#	pragma warning(disable : 4668)
#	pragma warning(disable : 5039)
#	pragma warning(disable : 5105)
#	include <windows.h>
#	pragma warning(pop)
#	undef NOMINMAX
#else
#	include <dlfcn.h>
#	define HMODULE void*
#endif

HMODULE nxLibDynamicHandle = 0;

#define NXLIB_MACRO(TYPE, NAME, DESC, PARS) __##NAME _##NAME;
#include "nxLibDynamicMacro.h"
#undef NXLIB_MACRO

void NXLIB_ATTR_UNUSED nxLibLoadDynamic(NXLIBERR* result, NXLIBSTR dllPath)
{
	if (nxLibDynamicHandle != 0) {
		if (result) *result = NxLibLibraryAlreadyLoaded;
		return;
	}
#ifdef _WIN32
	nxLibDynamicHandle = LoadLibrary(dllPath);
#else
	nxLibDynamicHandle = dlopen(dllPath, RTLD_LAZY);
#endif
	if (nxLibDynamicHandle == 0) {
		if (result) *result = NxLibCouldNotLoadLibrary;
		return;
	}

#ifdef _WIN32
#	define NXLIB_MACRO(TYPE, NAME, DESC, PARS) _##NAME = (__##NAME)(void*)GetProcAddress(nxLibDynamicHandle, #    NAME);
#else
#	define NXLIB_MACRO(TYPE, NAME, DESC, PARS)                                                   \
		_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wpedantic\"") _##NAME = \
		    (__##NAME)dlsym(nxLibDynamicHandle, #NAME);                                           \
		_Pragma("GCC diagnostic pop")
#endif
#include "nxLibDynamicMacro.h"
#undef NXLIB_MACRO

	if (result) *result = NxLibOperationSucceeded;
}

#ifdef __cplusplus
void NXLIB_ATTR_UNUSED nxLibLoadDynamic(std::string const& dllPath)
{
	int result;
	nxLibLoadDynamic(&result, dllPath.c_str());
	nxLibCheckReturnCode(result);
}
#endif

void NXLIB_ATTR_UNUSED nxLibFreeDynamic(NXLIBERR* result)
{
#define NXLIB_MACRO(TYPE, NAME, DESC, PARS) _##NAME = 0;
#include "nxLibDynamicMacro.h"
#undef NXLIB_MACRO

#ifdef _WIN32
	if (nxLibDynamicHandle != 0) FreeLibrary(nxLibDynamicHandle);
#else
	if (nxLibDynamicHandle != 0) dlclose(nxLibDynamicHandle);
#endif

	nxLibDynamicHandle = 0;

	if (result) *result = NxLibOperationSucceeded;
}

#if defined(__cplusplus)
void NXLIB_ATTR_UNUSED nxLibFreeDynamic()
{
	int result;
	nxLibFreeDynamic(&result);
	nxLibCheckReturnCode(result);
}
#endif

NXLIBBOOL NXLIB_ATTR_UNUSED nxLibIsRemoteLoaded(void)
{
	return _nxLibConnect != 0 ? NXLIBTRUE : NXLIBFALSE;
}

#undef NXLIB_ATTR_UNUSED

#endif /* __NXLIBDYNAMIC_SRC_H__ */
