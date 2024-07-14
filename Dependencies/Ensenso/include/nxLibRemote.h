#ifndef __NXLIBREMOTE_H__
#define __NXLIBREMOTE_H__

#include "nxLibConstants.h"
#include "nxLibVersion.h"

#if defined(_WIN32)
#	if defined(NXLIBREMOTE_EXPORT_SYMBOLS)
#		define NXLIB_EXPORT __declspec(dllexport)
#	else
#		define NXLIB_EXPORT __declspec(dllimport)
#	endif
#	define ATTR_UNUSED
#else /* defined (_WIN32) */
#	define NXLIB_EXPORT
#	define ATTR_UNUSED __attribute__((unused))
#endif

#include "nxLibFunctions.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NXLIB_DYNAMIC_LOAD

/**
 * Connects to an NxLib instance via TCP on the specified host and port. As NxLibRemote has no own tree structure, it is
 * necessary to connect to an NxLib instance. All tree operations such as reads and writes will then be carried out on
 * the tree of the remote NxLib instance. The remote instance must be prepared for the connection by opening a TCP port
 * using `nxLibOpenTcpPort()`.
 * @note This function is only available in the NxLibRemote Interface. NxLib provides the corresponding functions
 * `nxLibOpenTcpPort()` and `nxLibCloseTcpPort()`.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param hostname A string specifying a valid DNS host name or IP address.
 * @param portNumber The port number on which the remote NxLib instance listens for connections.
 */
NXLIB_EXPORT void nxLibConnect(NXLIBERR* result, NXLIBSTR hostname, NXLIBINT portNumber);

/**
 * Disconnects from the NxLib instance, if connected.
 * @param result The error code of the operation.
 */
NXLIB_EXPORT void nxLibDisconnect(NXLIBERR* result);

#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
 * @cppoverload{nxLibConnect(NXLIBERR*, NXLIBSTR, NXLIBINT)}
 **/
static void ATTR_UNUSED nxLibConnect(std::string const& hostname, int port)
{
	int result;
	nxLibConnect(&result, hostname.c_str(), port);
	nxLibCheckReturnCode(result);
}

/**
 * @cppoverload{nxLibDisconnect(NXLIBERR*)}
 **/
static void ATTR_UNUSED nxLibDisconnect()
{
	int result;
	nxLibDisconnect(&result);
	nxLibCheckReturnCode(result);
}
#endif /* __cplusplus */

#endif /*__NXLIBREMOTE_H__*/
