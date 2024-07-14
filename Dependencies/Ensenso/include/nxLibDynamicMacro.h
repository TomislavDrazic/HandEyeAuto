NXLIB_MACRO(
    void,
    nxLibInitialize,
    (NXLIBERR * result, NXLIBBOOL waitForInitialCameraRefresh),
    (result, waitForInitialCameraRefresh))
NXLIB_MACRO(void, nxLibFinalize, (NXLIBERR * result), (result))
NXLIB_MACRO(
    void,
    nxLibOpenTcpPort,
    (NXLIBERR * result, NXLIBINT portNumber, NXLIBINT* openedPort),
    (result, portNumber, openedPort))
NXLIB_MACRO(void, nxLibCloseTcpPort, (NXLIBERR * result), (result))

NXLIB_MACRO(
    void,
    nxLibConnect,
    (NXLIBERR * result, NXLIBSTR hostname, NXLIBINT portNumber),
    (result, hostname, portNumber))
NXLIB_MACRO(void, nxLibDisconnect, (NXLIBERR * result), (result))

NXLIB_MACRO(void, nxLibErase, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))

NXLIB_MACRO(void, nxLibSetNull, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))
NXLIB_MACRO(
    void,
    nxLibSetJson,
    (NXLIBERR * result, NXLIBSTR itemPath, NXLIBSTR value, NXLIBBOOL onlyWriteableNodes),
    (result, itemPath, value, onlyWriteableNodes))
NXLIB_MACRO(void, nxLibSetInt, (NXLIBERR * result, NXLIBSTR itemPath, NXLIBINT value), (result, itemPath, value))
NXLIB_MACRO(void, nxLibSetDouble, (NXLIBERR * result, NXLIBSTR itemPath, NXLIBDOUBLE value), (result, itemPath, value))

NXLIB_MACRO(void, nxLibSetBool, (NXLIBERR * result, NXLIBSTR itemPath, NXLIBBOOL value), (result, itemPath, value))
NXLIB_MACRO(void, nxLibSetString, (NXLIBERR * result, NXLIBSTR itemPath, NXLIBSTR value), (result, itemPath, value))
NXLIB_MACRO(
    void,
    nxLibSetBinary,
    (NXLIBERR * result, NXLIBSTR itemPath, void const* buffer, NXLIBINT bufferSize),
    (result, itemPath, buffer, bufferSize))
NXLIB_MACRO(
    void,
    nxLibSetBinaryFormatted,
    (NXLIBERR * result,
     NXLIBSTR itemPath,
     void const* buffer,
     NXLIBINT width,
     NXLIBINT height,
     NXLIBINT channelCount,
     NXLIBINT bytesPerElement,
     NXLIBBOOL isFloat),
    (result, itemPath, buffer, width, height, channelCount, bytesPerElement, isFloat))

NXLIB_MACRO(
    NXLIBSTR,
    nxLibGetJson,
    (NXLIBINT * result,
     NXLIBSTR itemPath,
     NXLIBBOOL prettyPrint,
     NXLIBINT numberPrecision,
     NXLIBBOOL allowScientificFormat),
    (result, itemPath, prettyPrint, numberPrecision, allowScientificFormat))
NXLIB_MACRO(
    NXLIBSTR,
    nxLibGetJsonMeta,
    (NXLIBINT * result,
     NXLIBSTR itemPath,
     NXLIBINT numLevels,
     NXLIBBOOL prettyPrint,
     NXLIBINT numberPrecision,
     NXLIBBOOL allowScientificFormat),
    (result, itemPath, numLevels, prettyPrint, numberPrecision, allowScientificFormat))
NXLIB_MACRO(NXLIBINT, nxLibGetInt, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))
NXLIB_MACRO(NXLIBDOUBLE, nxLibGetDouble, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))
NXLIB_MACRO(NXLIBBOOL, nxLibGetBool, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))
NXLIB_MACRO(NXLIBSTR, nxLibGetString, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))
NXLIB_MACRO(
    void,
    nxLibGetBinary,
    (NXLIBERR * result,
     NXLIBSTR itemPath,
     void* destinationBuffer,
     NXLIBINT bufferSize,
     NXLIBINT* bytesCopied,
     NXLIBDOUBLE* timestamp),
    (result, itemPath, destinationBuffer, bufferSize, bytesCopied, timestamp))

NXLIB_MACRO(NXLIBINT, nxLibGetType, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))
NXLIB_MACRO(NXLIBINT, nxLibGetCount, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))
NXLIB_MACRO(NXLIBSTR, nxLibGetName, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))
NXLIB_MACRO(
    void,
    nxLibGetBinaryInfo,
    (NXLIBERR * result,
     NXLIBSTR itemPath,
     NXLIBERR* width,
     NXLIBINT* height,
     NXLIBINT* channelCount,
     NXLIBINT* bytesPerElement,
     NXLIBBOOL* isFloat,
     NXLIBDOUBLE* timestamp),
    (result, itemPath, width, height, channelCount, bytesPerElement, isFloat, timestamp))

NXLIB_MACRO(void, nxLibWaitForChange, (NXLIBERR * result, NXLIBSTR itemPath), (result, itemPath))
NXLIB_MACRO(
    void,
    nxLibWaitForType,
    (NXLIBERR * result, NXLIBSTR itemPath, NXLIBINT nxLibItemType, NXLIBBOOL waitForEqual),
    (result, itemPath, nxLibItemType, waitForEqual))
NXLIB_MACRO(
    void,
    nxLibWaitForStringValue,
    (NXLIBERR * result, NXLIBSTR itemPath, NXLIBSTR value, NXLIBBOOL waitForEqual),
    (result, itemPath, value, waitForEqual))
NXLIB_MACRO(
    void,
    nxLibWaitForIntValue,
    (NXLIBERR * result, NXLIBSTR itemPath, NXLIBINT value, NXLIBBOOL waitForEqual),
    (result, itemPath, value, waitForEqual))
NXLIB_MACRO(
    void,
    nxLibWaitForDoubleValue,
    (NXLIBERR * result, NXLIBSTR itemPath, NXLIBDOUBLE value, NXLIBBOOL waitForEqual),
    (result, itemPath, value, waitForEqual))
NXLIB_MACRO(
    void,
    nxLibWaitForBoolValue,
    (NXLIBERR * result, NXLIBSTR itemPath, NXLIBBOOL value, NXLIBBOOL waitForEqual),
    (result, itemPath, value, waitForEqual))

NXLIB_MACRO(void, nxLibWriteDebugMessage, (NXLIBERR * result, NXLIBSTR message), (result, message))
NXLIB_MACRO(NXLIBSTR, nxLibGetDebugMessages, (NXLIBERR * result), (result))
NXLIB_MACRO(
    NXLIBSTR,
    nxLibMakeUniqueItem,
    (NXLIBERR * result, NXLIBSTR itemPath, NXLIBSTR itemName),
    (result, itemPath, itemName))

NXLIB_MACRO(void, nxLibSetDebugThreadName, (NXLIBERR * result, NXLIBSTR threadName), (result, threadName))
NXLIB_MACRO(
    void,
    nxLibOpenDebugBlock,
    (NXLIBERR * result, NXLIBSTR blockName, NXLIBINT level),
    (result, blockName, level))
NXLIB_MACRO(void, nxLibCloseDebugBlock, (NXLIBERR * result), (result))
NXLIB_MACRO(
    NXLIBINT,
    nxLibGetDebugBuffer,
    (NXLIBERR * result, void* destinationBuffer, NXLIBINT bufferSize, NXLIBINT* bytesRemaining, NXLIBBOOL clearRead),
    (result, destinationBuffer, bufferSize, bytesRemaining, clearRead))
