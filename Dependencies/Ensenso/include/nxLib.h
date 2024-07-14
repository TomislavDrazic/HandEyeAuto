#ifndef __NXLIB_H__
#define __NXLIB_H__

#include "nxLibConstants.h"
#include "nxLibVersion.h"

#if defined(_WIN32) || defined(__WINDOWS__)
#	if defined(NXLIB_EXPORT_SYMBOLS)
#		define NXLIB_EXPORT __declspec(dllexport)
#	else
#		define NXLIB_EXPORT __declspec(dllimport)
#	endif
#else /* defined (_WIN32) */
#	define NXLIB_EXPORT
#endif

#ifndef ATTR_UNUSED
#	if defined(_WIN32) || defined(__WINDOWS__)
#		define ATTR_UNUSED
#	else /* defined (_WIN32) */
#		define ATTR_UNUSED __attribute__((unused))
#	endif
#endif

#include "nxLibFunctions.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifndef NXLIB_DYNAMIC_LOAD
/**
 * Explicitly initializes the library and starts to enumerate the connected cameras. When omitting this function
 * call, the library is initialized on first access to the tree.
 * @note After nxLibFinalize() was called, the library will no longer be initialized automatically when you access the
 * tree. You can reinitialize it by explicitly calling nxLibInitialize.
 * @param result The error code of the operation.
 * @param waitForInitialCameraRefresh Specifies whether to wait for all cameras to be enumerated. If this parameter is
 * disabled, the function will return immediately while the camera list might not be fully populated yet. You can still
 * access the tree normally and the camera list will be completed eventually.
 */
NXLIB_EXPORT void nxLibInitialize(NXLIBERR* result, NXLIBBOOL waitForInitialCameraRefresh);

/**
 * Explicitly closes the library, terminating all internal threads and freeing allocated memory. It is important to
 * explicitly call nxLibFinalize() before unloading the NxLib library when your process is not terminating afterwards,
 * because Windows doesn't allow to cleanly exit threads during DLL unload.
 * @note After closing the library with nxLibFinalize(), it will no longer be automatically initialized when you attempt
 * to access the tree. You can call nxLibInitialize() explicitly to initialize the library again.
 * @param result The error code of the operation.
 */
NXLIB_EXPORT void nxLibFinalize(NXLIBERR* result);

/**
 * Opens a TCP port through which NxLibRemote can connect to the current NxLib instance.
 * @note This function is only available in the NxLib interface. NxLibRemote provides the corresponding functions
 * nxLibConnect() and nxLibDisconnect().
 * @param result The error code of the operation.
 * @param portNumber The port to open. Specify 0 here to automatically select a port in the range 24000 to 25000.
 * @param openedPort A pointer to a variable that gets the number of the opened port.
 */
NXLIB_EXPORT void nxLibOpenTcpPort(NXLIBERR* result, NXLIBINT portNumber, NXLIBINT* openedPort);

/**
 * Disconnects all connected NxLibRemote instances and closes the open TCP port, if any.
 * @note This function is only available in the NxLib interface. NxLibRemote provides the corresponding functions
 * nxLibConnect() and nxLibDisconnect().
 * @param result The error code of the operation.
 */
NXLIB_EXPORT void nxLibCloseTcpPort(NXLIBERR* result);
#endif
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
 * @cppoverload{nxLibInitialize(int32_t*, int32_t)}
 **/
static void ATTR_UNUSED nxLibInitialize(bool waitForInitialCameraRefresh = true)
{
	int result;
	nxLibInitialize(&result, static_cast<NXLIBBOOL>(waitForInitialCameraRefresh));
	nxLibCheckReturnCode(result);
}

/**
 * @cppoverload{nxLibFinalize(int32_t*)}
 **/
static void ATTR_UNUSED nxLibFinalize()
{
	int result;
	nxLibFinalize(&result);
	nxLibCheckReturnCode(result);
}

/**
 * @cppoverload{nxLibOpenTcpPort(int32_t*, int32_t, int32_t*)}
 **/
static void ATTR_UNUSED nxLibOpenTcpPort(int portNumber = 0, int* openedPort = 0)
{
	int result;
	nxLibOpenTcpPort(&result, portNumber, openedPort);
	nxLibCheckReturnCode(result);
}

/**
 * @cppoverload{nxLibCloseTcpPort(int32_t*)}
 **/
static void ATTR_UNUSED nxLibCloseTcpPort()
{
	int result;
	nxLibCloseTcpPort(&result);
	nxLibCheckReturnCode(result);
}
#endif

#endif /*__NXLIB_H_*/
