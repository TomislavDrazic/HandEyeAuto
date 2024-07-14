#ifndef __NXLIBDYNAMIC_H__
#define __NXLIBDYNAMIC_H__

#include "nxLibConstants.h"
#if defined(__GNUC__)
#	define NXLIB_ATTR_UNUSED __attribute__((unused))
#else
#	define NXLIB_ATTR_UNUSED
#endif

#ifdef __WINDOWS__
#	define NXLIB_MACRO(TYPE, NAME, DESC, PARS) typedef TYPE(__cdecl* __##NAME) DESC;
#else
#	define NXLIB_MACRO(TYPE, NAME, DESC, PARS) typedef TYPE(*__##NAME) DESC;
#endif

#include "nxLibDynamicMacro.h"
#undef NXLIB_MACRO

#define NXLIB_MACRO(TYPE, NAME, DESC, PARS) extern __##NAME _##NAME;
#include "nxLibDynamicMacro.h"
#undef NXLIB_MACRO

// C under MSVC warns about returning (void)0
#define NXLIB_RETURN0_void
#define NXLIB_RETURN_void
#define NXLIB_RETURN0_NXLIBSTR return (NXLIBSTR)0;
#define NXLIB_RETURN_NXLIBSTR return
#define NXLIB_RETURN0_NXLIBINT return (NXLIBINT)0;
#define NXLIB_RETURN_NXLIBINT return
#define NXLIB_RETURN0_NXLIBDOUBLE return (NXLIBDOUBLE)0;
#define NXLIB_RETURN_NXLIBDOUBLE return
#define NXLIB_RETURN0_NXLIBBOOL return (NXLIBBOOL)0;
#define NXLIB_RETURN_NXLIBBOOL return

#define NXLIB_MACRO(TYPE, NAME, DESC, PARS)                  \
	static TYPE NXLIB_ATTR_UNUSED NAME DESC                  \
	{                                                        \
		if (_##NAME) {                                       \
			NXLIB_RETURN_##TYPE _##NAME PARS;                \
		} else {                                             \
			if (result) *result = NxLibCouldNotLoadFunction; \
			NXLIB_RETURN0_##TYPE                             \
		}                                                    \
	}
#include "nxLibDynamicMacro.h"
#undef NXLIB_MACRO

#ifndef NXLIB_DYNAMIC_LOAD
#	define NXLIB_DYNAMIC_LOAD
#endif

#include "nxLib.h"
#include "nxLibRemote.h"

/**
 * Loads the NxLib located at @a dllPath via the Dynamic Link Loader methods dlopen (under Unix) or LoadLibrary (under
 * Windows) and tries to resolve each @rst{:doc:`NxLib <c:other/nxlib/funcs>`} and
 * @rst{:doc:`NxLibRemote <c:other/nxlibremote/funcs>`} API function handle. Calling an API function loaded by
 * nxLibLoadDynamic may result in an `NxLibCouldNotLoadFunction` @rst{:doc:`error code <api:return-codes>`} if it could
 * not be resolved, otherwise a standard API call will be executed.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. It is set to
 * - `NxLibOperationSucceeded` if NxLib was loaded successfully,
 * - `NxLibLibraryAlreadyLoaded` if nxLibLoadDynamic has been called successfully before, without a subsequent call of
 * nxLibFreeDynamic or
 * - `NxLibCouldNotLoadLibrary` if anything went wrong calling dlopen/LoadLibrary.
 * @param dllPath The path to the NxLib to load.
 */
void NXLIB_ATTR_UNUSED nxLibLoadDynamic(NXLIBERR* result, NXLIBSTR dllPath);

/**
 * Unmap and close the NxLib loaded with nxLibLoadDynamic(). All subsequent API calls set
 * @rst{:doc:`error code <api:return-codes>`} to `NxLibCouldNotLoadFunction` in their result argument until an NxLib is
 * loaded again.
 * @note Make sure you call nxLibFinalize() first.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. Will be set to
 * `NxLibOperationSucceeded`.
 */
void NXLIB_ATTR_UNUSED nxLibFreeDynamic(NXLIBERR* result);

/**
 * Indicates if the loaded NxLib is a remote NxLib and implements the
 * @rst{:doc:`Remote interface. <c:other/nxlibremote/funcs>`}
 * @return NXLIBTRUE if the loaded NxLib is a remote lib, NXLIBFALSE otherwise.
 */
NXLIBBOOL NXLIB_ATTR_UNUSED nxLibIsRemoteLoaded(void);

#if defined(__cplusplus)
/**
 * @cppoverload{nxLibLoadDynamic(NXLIBERR*,NXLIBSTR)}
 **/
void NXLIB_ATTR_UNUSED nxLibLoadDynamic(std::string const& dllPath);

/**
 * @cppoverload{nxLibFreeDynamic(NXLIBERR*)}
 **/
void NXLIB_ATTR_UNUSED nxLibFreeDynamic();
#endif

#if defined(NXLIB_DYNAMIC_IMPLEMENTATION)
#	include "nxLibDynamic.src.h"
#endif

#undef NXLIB_ATTR_UNUSED
#undef NXLIB_DYNAMIC_LOAD

#endif /* __NXLIBDYNAMIC_H__ */
