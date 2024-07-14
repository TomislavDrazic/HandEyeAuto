#ifndef __NXLIB_FUNCTIONS_H__
#define __NXLIB_FUNCTIONS_H__

#include "nxLibConstants.h"
#ifdef __cplusplus
extern "C" {
#endif
#ifndef NXLIB_DYNAMIC_LOAD
/**
 * Erase an item or subtree
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item or subtree to erase.
 */
NXLIB_EXPORT void nxLibErase(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Sets an item to the value Null.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to set to the value Null. When specifying a subtree,
 * the entire subtree will be replaced by a single Null value.
 */
NXLIB_EXPORT void nxLibSetNull(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Sets an item to the given JSON value. The value might itself be an entire tree structure which will be placed under
 * the specified node.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to set.
 * @param value The JSON string representing the value or subtree to write.
 * @param onlyWriteableNodes Specifies whether the function should try to write each single node into the existing tree
 * structure instead of replacing the entire subtree. When specifying `true` here the function will not complain if
 * certain nodes could not be written due to access restrictions. This can be used to restore the state of the library
 * or the Parameters node of a camera if the tree state has previously been captured via `nxLibGetJson()`. When this
 * parameter is set to `false`, all items of the entire subtree must not be access restricted in order for the function
 * call to succeed! If this parameter is set and the given JSON value does not match the current structure of the node,
 * the function will return the error code `NxLibItemTypeNotCompatible`. This indicates, that there was not a single
 * node with the same structure as in the JSON string and the function could never modify any value. In this case you
 * probably tried to apply the JSON value to the wrong path.
 */
NXLIB_EXPORT void nxLibSetJson(NXLIBERR* result, NXLIBSTR itemPath, NXLIBSTR value, NXLIBBOOL onlyWriteableNodes);

/**
 * Sets an item to the specified integer value. The resulting item will be of type Number.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to set. When specifying a subtree, the entire subtree
 * will be replaced by the specified value.
 * @param value The value to set.
 */
NXLIB_EXPORT void nxLibSetInt(NXLIBERR* result, NXLIBSTR itemPath, NXLIBINT value);

/**
 * Sets an item to the specified floating point value. The resulting item will be of type Number.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to set. When specifying a subtree, the entire subtree
 * will be replaced by the specified value.
 * @param value The value to set.
 */
NXLIB_EXPORT void nxLibSetDouble(NXLIBERR* result, NXLIBSTR itemPath, NXLIBDOUBLE value);

/**
 * Sets an item to the specified boolean value. The resulting item will be of type Boolean.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to set. When specifying a subtree, the entire subtree
 * will be replaced by the specified value.
 * @param value The value to set.
 */
NXLIB_EXPORT void nxLibSetBool(NXLIBERR* result, NXLIBSTR itemPath, NXLIBBOOL value);

/**
 * Sets an item to the specified string. The resulting item will be of type String.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to set. When specifying a subtree, the entire subtree
 * will be replaced by the specified value.
 * @param value The value to set.
 */
NXLIB_EXPORT void nxLibSetString(NXLIBERR* result, NXLIBSTR itemPath, NXLIBSTR value);

/**
 * Sets data of a Binary item.
 * The data format must be identical to the current format of the binary node. The current format can be queried using
 * `nxLibGetBinaryInfo()`. The time stamp of the node will be set to the current UTC time.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to set. When specifying a subtree, the entire subtree
 * will be replaced by the specified value.
 * @param buffer Pointer to the buffer
 * @param bufferSize Actual size of the buffer.
 */
NXLIB_EXPORT void nxLibSetBinary(NXLIBERR* result, NXLIBSTR itemPath, void const* buffer, NXLIBINT bufferSize);
NXLIB_EXPORT void nxLibSetBinaryFormatted(
    NXLIBERR* result,
    NXLIBSTR itemPath,
    void const* buffer,
    NXLIBINT width,
    NXLIBINT height,
    NXLIBINT channelCount,
    NXLIBINT bytesPerElement,
    NXLIBBOOL isFloat);

/**
 * Retrieves an item value or an entire subtree in JSON representation.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access
 * @param prettyPrint Specify `true` here to format the resulting string with line breaks and tabs.
 * @param numberPrecision The floating point precision of the returned numbers.
 * @param allowScientificFormat Specifies whether to allow scientific notation.
 * @return A JSON string representing the item value or subtree. Please refer to the
 * @rst{:doc:`string type description <c:types>`} for information about usage restrictions applying to the returned
 * pointer.
 */
NXLIB_EXPORT NXLIBSTR nxLibGetJson(
    NXLIBINT* result,
    NXLIBSTR itemPath,
    NXLIBBOOL prettyPrint,
    NXLIBINT numberPrecision,
    NXLIBBOOL allowScientificFormat);

/**
 * Retrieves an item value or an entire subtree in JSON representation including item metadata (protection, extended
 * type, internal flags). This function is only intended to be used by NxTreeEdit to visualize the entire tree state.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @param numLevels The depth of the returned subtree. Nodes in lower levels will be omitted.
 * @param prettyPrint Specify `true` here to format the resulting string with line breaks and tabs.
 * @param numberPrecision The floating point precision of the returned numbers.
 * @param allowScientificFormat Specifies whether to allow scientific notation.
 * @return A JSON string representing the item value or subtree including each item's metadata. Please refer to the
 * @rst{:doc:`string type description <c:types>`} for information about usage restrictions applying to the returned
 * pointer.
 */
NXLIB_EXPORT NXLIBSTR nxLibGetJsonMeta(
    NXLIBINT* result,
    NXLIBSTR itemPath,
    NXLIBINT numLevels,
    NXLIBBOOL prettyPrint,
    NXLIBINT numberPrecision,
    NXLIBBOOL allowScientificFormat);

/**
 * Retrieves the integer value of a Number item.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @return The integer value of the item. If the item's number value was a floating point number, the rounded value will
 * be returned.
 */
NXLIB_EXPORT NXLIBINT nxLibGetInt(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Retrieves the floating point value of a Number item.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @return The floating point value of the item.
 */
NXLIB_EXPORT NXLIBDOUBLE nxLibGetDouble(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Retrieves the value of a Boolean item.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @return The boolean value of the item.
 */
NXLIB_EXPORT NXLIBBOOL nxLibGetBool(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Retrieves the value of a String item.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @return The string value of the item. Please refer to the @rst{:doc:`string type description <c:types>`} for
 * information about usage restrictions applying to the returned pointer.
 */
NXLIB_EXPORT NXLIBSTR nxLibGetString(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Retrieves data of a Binary item.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @param destinationBuffer A pointer to the memory, where the item content should be copied.
 * @param bufferSize The size of the destination buffer in bytes.
 * @param bytesCopied A pointer to an integer. The variable will be filled with the actual number of bytes written to
 * the buffer
 * @param timestamp The current @rst{:doc:`timestamp <api:types>`} of the binary blob queried.
 */
NXLIB_EXPORT void nxLibGetBinary(
    NXLIBERR* result,
    NXLIBSTR itemPath,
    void* destinationBuffer,
    NXLIBINT bufferSize,
    NXLIBINT* bytesCopied,
    NXLIBDOUBLE* timestamp);

/**
 * Retrieves the item type of a tree item.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @return The type identifier of the basic type of the item.
 * @retval NxLibItemTypeInvalid An error occurred, refer to `result` for further information.
 * @retval NxLibItemTypeNull The item is of type Null.
 * @retval NxLibItemTypeNumber The item is a Number. This can be an integer or floating point value.
 * @retval NxLibItemTypeString The item is a String.
 * @retval NxLibItemTypeBool The item is a Boolean value.
 * @retval NxLibItemTypeArray The item is an Array. The number of array elements can be queried with `nxLibGetCount()`.
 * The array can be empty.
 * @retval NxLibItemTypeObject The item is an Object. The number of children can be queried with `nxLibGetCount()`. The
 * object can be empty.
 */
NXLIB_EXPORT NXLIBINT nxLibGetType(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Retrieves the number of subitems of an @rst{:doc:`Object <api:types>`} or the number of elements of an
 * @rst{:doc:`Array <api:types>`} item. In case of a @rst{:doc:`Binary <api:types>`} item its data size in bytes is
 * returned.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @return The number of subitems.
 */
NXLIB_EXPORT NXLIBINT nxLibGetCount(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Retrieves string of an item. This function is intended to be used to enumerate subitems of an Object. The subitems
 * can be accessed by the index notation (see here), then the corresponding names can be queried with `nxLibGetName()`.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @return The string name of the item, without its path. Please refer to the
 * @rst{:doc:`string type description <c:types>`} for information about usage restrictions applying to the returned
 * pointer.
 */
NXLIB_EXPORT NXLIBSTR nxLibGetName(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Retrieves meta data of a Binary item.
 * @param result A pointer to a variable for the @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to access.
 * @param width The width of the array (consecutive elements in memory).
 * @param height The height of the array (number of rows of `width` * `channelCount` elements)
 * @param channelCount Number of channels for a single item.
 * @param bytesPerElement Size in bytes of a single channel value of an item.
 * @param isFloat Specifies whether the element data type is a floating point type.
 * @param timestamp The current @rst{:doc:`timestamp <api:types>`} of the binary blob queried.
 * @note The entire size of the binary blob is `width` * `height` * `channelCount` * `bytesPerElement` bytes. For
 * example a 24bit RGB image of VGA resolution would have `width` = `640`, `height` = `480`, `channelCount` = `3`,
 * `bytesPerElement` = `1` and `isFloat` = `NXLIBFALSE`.
 */
NXLIB_EXPORT void nxLibGetBinaryInfo(
    NXLIBERR* result,
    NXLIBSTR itemPath,
    NXLIBINT* width,
    NXLIBINT* height,
    NXLIBINT* channelCount,
    NXLIBINT* bytesPerElement,
    NXLIBBOOL* isFloat,
    NXLIBDOUBLE* timestamp);

/**
 * Wait for any change to the specified item, i.e. a change of value or item type.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. If the item is deleted during the wait
 * operation, the return code will be `NxLibItemInexistent`.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to wait for.
 */
NXLIB_EXPORT void nxLibWaitForChange(NXLIBERR* result, NXLIBSTR itemPath);

/**
 * Wait for a type change to the specified item. When the condition to wait for is already satisfied initially, the
 * function returns immediately.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. If the item is deleted during the wait
 * operation, the return code will be `NxLibItemInexistent`.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to wait for.
 * @param nxLibItemType The constant corresponding to the type to wait for. See also `nxLibGetType()`.
 * @param waitForEqual When `true`, the function waits until the item has the specified type, otherwise it waits until
 * the item has a different type than the one specified.
 */
NXLIB_EXPORT void nxLibWaitForType(NXLIBERR* result, NXLIBSTR itemPath, NXLIBINT nxLibItemType, NXLIBBOOL waitForEqual);

/**
 * Wait for specific string value of an item. When the condition to wait for is already satisfied initially, the
 * function returns immediately.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. If the item is deleted during the wait
 * operation, the return code will be `NxLibItemInexistent`.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to wait for.
 * @param value The value to wait for.
 * @param waitForEqual When `true`, the function waits until the item has the specified type, otherwise it waits until
 * the item has a different type than the one specified.
 */
NXLIB_EXPORT void nxLibWaitForStringValue(NXLIBERR* result, NXLIBSTR itemPath, NXLIBSTR value, NXLIBBOOL waitForEqual);

/**
 * Wait for specific integer value of an item. When the condition to wait for is already satisfied initially, the
 * function returns immediately.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. If the item is deleted during the wait
 * operation, the return code will be `NxLibItemInexistent`.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to wait for.
 * @param value The value to wait for.
 * @param waitForEqual When `true`, the function waits until the item has the specified type, otherwise it waits until
 * the item has a different type than the one specified.
 * @note According to the behavior of `nxLibGetInt()` the comparison for this condition is done with the rounded item
 * value.
 */
NXLIB_EXPORT void nxLibWaitForIntValue(NXLIBERR* result, NXLIBSTR itemPath, NXLIBINT value, NXLIBBOOL waitForEqual);

/**
 * Wait for specific double value of an item. When the condition to wait for is already satisfied initially, the
 * function returns immediately.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. If the item is deleted during the wait
 * operation, the return code will be `NxLibItemInexistent`.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to wait for.
 * @param value The value to wait for.
 * @param waitForEqual When `true`, the function waits until the item has the specified type, otherwise it waits until
 * the item has a different type than the one specified.
 */
NXLIB_EXPORT void
nxLibWaitForDoubleValue(NXLIBERR* result, NXLIBSTR itemPath, NXLIBDOUBLE value, NXLIBBOOL waitForEqual);

/**
 * Wait for specific boolean value of an item. When the condition to wait for is already satisfied initially, the
 * function returns immediately.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. If the item is deleted during the wait
 * operation, the return code will be `NxLibItemInexistent`.
 * @param itemPath The @rst{:doc:`path <c:paths>`} of the item to wait for.
 * @param value The value to wait for.
 * @param waitForEqual When `true`, the function waits until the item has the specified type, otherwise it waits until
 * the item has a different type than the one specified.
 */
NXLIB_EXPORT void nxLibWaitForBoolValue(NXLIBERR* result, NXLIBSTR itemPath, NXLIBBOOL value, NXLIBBOOL waitForEqual);

/**
 * Inserts a user defined debug message into the NxLib debug stream. The message will be output as specified under the
 * @rst{:doc:`tree:debug`} node.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation.
 * @param message The message to be written.
 */
NXLIB_EXPORT void nxLibWriteDebugMessage(NXLIBERR* result, NXLIBSTR message);

/**
 * Retrieves debug messages. This can also be done remotely from NxLibRemote.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. If the message buffer could not hold
 * all streamed messages, the error code `NxLibDebugMessageOverflow` will be returned. In that case, the returned string
 * will contain only the newest debug messages that could fit into the buffer.
 * @return The debug messages from all threads separated by the newline character (\\n). Please refer to the
 * @rst{:doc:`string type description <c:types>`} for information about usage restrictions applying to the returned
 * pointer.
 * @note By calling this function the debug buffer will be fully cleared and the debug information can no longer be
 * retrieved with `nxLibGetDebugBuffer()`.
 * @note Calling this function will temporarily lock all debug ring buffers and thus block all NxLib threads which are
 * trying to push debug information onto their buffer. The user should therefore choose a point in time when NxLib is
 * not performing time critical computations for his application to retrieve debug information.
 */
NXLIB_EXPORT NXLIBSTR nxLibGetDebugMessages(NXLIBERR* result);

/**
 * Creates a new item with a unique name.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of operation.
 * @param itemPath The parent path of the new item to create.
 * @param itemName The base name of the item. If the base name does already exist, the base name will be append with a
 * number to make the item unique again.
 * @return The name of the generated item. Please refer to the @rst{:doc:`string type description <c:types>`} for
 * information about usage restrictions applying to the returned pointer.
 */
NXLIB_EXPORT NXLIBSTR nxLibMakeUniqueItem(NXLIBERR* result, NXLIBSTR itemPath, NXLIBSTR itemName);

/**
 * Sets the debug name of the current thread. This name is used by the NxProfiler for the visualization of the debug
 * information. Multiple threads with the same name and no simultaneously open blocks will be grouped together to a
 * common time line.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of operation
 * @param threadName Debug thread name.
 */
NXLIB_EXPORT void nxLibSetDebugThreadName(NXLIBERR* result, NXLIBSTR threadName);

/**
 * Starts a debug block. This information is used by NxProfiler for its visualization. Open debug blocks can be
 * closed with `nxLibCloseDebugBlock()`.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of operation.
 * @param blockName Debug block name.
 * @param level Debug level of this block.
 *
 * @par Possible Debug Levels
 * @verbatim embed:rst:leading-asterisk
 * :code:`nxdInherit`: The block will inherit the debug level of its parent. When the block does not have a parent, this
 * will correspond to the :doc:`debug level <tree:debug/level>` :code:`"Info"`.
 *
 * :code:`nxdInfo`: Corresponds to the :doc:`debug level <tree:debug/level>` :code:`"Info"`.
 *
 * :code:`nxdDebug`: Corresponds to the debug level :code:`"Debug"`.
 *
 * :code:`nxdTrace`: Corresponds to the debug level :code:`"Trace"`.
 * @endverbatim
 */
NXLIB_EXPORT void nxLibOpenDebugBlock(NXLIBERR* result, NXLIBSTR blockName, NXLIBINT level);

/**
 * Closes the current debug block at the inner most nesting level. This information is used by the NxProfiler for its
 * visualization. Debug blocks can be started with `nxLibOpenDebugBlock()`.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of operation.
 */
NXLIB_EXPORT void nxLibCloseDebugBlock(NXLIBERR* result);

/**
 * Retrieves debug information in a binary format which can be read by NxProfiler. This can also be done remotely from
 * NxLibRemote.
 * @note Calling this function will temporarily lock all debug ring buffers and thus block all NxLib threads which
 * are trying to push debug information onto their buffer. The user should therefore choose a point in time when NxLib
 * is not performing time critical computations for his application to retrieve debug information.
 * @param result The @rst{:doc:`error code <api:return-codes>`} of the operation. If the debug buffer could not hold all
 * streamed debug information, the error code `NxLibDebugMessageOverflow` will be returned.
 * @param destinationBuffer Pointer to an allocated memory block.
 * @param bufferSize Size of the destination buffer.
 * @param bytesRemaining Pointer to an integer. Will be set to the number of remaining bytes in the debug buffer after
 * the read operation.
 * @param clearRead When set to `true` the operation will also remove the retrieved content from the debug buffer.
 */
NXLIB_EXPORT NXLIBINT nxLibGetDebugBuffer(
    NXLIBERR* result,
    void* destinationBuffer,
    NXLIBINT bufferSize,
    NXLIBINT* bytesRemaining,
    NXLIBBOOL clearRead);
#endif
#ifdef __cplusplus
}
#endif

/**
 * Retrieves a string description for an @rst{:doc:`API return code <api:return-codes>`}.
 * @param returnCode The @rst{:doc:`return code <api:return-codes>`} to convert to a text string
 * @return A pointer to a string description of the return code or `nullptr`, if an invalid return code was specified.
 */
static char const* ATTR_UNUSED nxLibTranslateReturnCode(NXLIBINT returnCode)
{
	switch (returnCode) {
#ifdef NXLIB_APIERROR
#	undef NXLIB_APIERROR
#endif
#define NXLIB_APIERROR(NAME, VALUE) \
	case VALUE:                     \
		return #NAME;               \
		break
#include "nxLibApiErrors.h"
	}
	return 0;
}

#ifdef __cplusplus

#	include <string>
#	include <cstring>
#	include <sstream>
#	include <vector>
#	include <assert.h>

#	ifndef NXLIB_CPP_COMPATIBILITY_MODE
#		if !(__cplusplus >= 201103L) && !(_MSC_VER >= 1600)
#			error Your compiler does not support C++11. If you want to use the NxLib without C++11 features, you can define NXLIB_CPP_COMPATIBILITY_MODE before including the NxLib headers. See the manual for more information and problems this might cause.
#		endif
#		include <type_traits>
#		include <memory>
#	endif

#	if defined(__GNUC__) || defined(__clang__)
#		define NXLIB_DEPRECATED __attribute__((deprecated))
#	elif defined(_MSC_VER)
#		define NXLIB_DEPRECATED __declspec(deprecated)
#	else
#		define NXLIB_DEPRECATED
#	endif

class NxLibItemToken;
class NxLibException {
protected:
	std::string itemPath;
	int errorCode;
	std::string errorText;

#	ifndef NXLIB_CPP_COMPATIBILITY_MODE
	std::shared_ptr<NxLibItemToken> token = nullptr;
#	endif

public:
#	ifndef NXLIB_CPP_COMPATIBILITY_MODE
	/**
	 * @param _itemPath The path to the tree node, which caused the exception.
	 * @param _errorCode The @rst{:doc:`error code <api:return-codes>`} that occurred while accessing the node.
	 * @param _token An optional NxLibItemToken to keep the execution slot of an NxLibCommand alive while this
	 * exception instance exists.
	 */
	NxLibException(std::string const& _itemPath, int _errorCode, std::shared_ptr<NxLibItemToken> _token = nullptr)
	: itemPath(_itemPath)
	, errorCode(_errorCode)
	, token(_token)
#	else
	NxLibException(std::string const& _itemPath, int _errorCode)
	: itemPath(_itemPath)
	, errorCode(_errorCode)
#	endif
	{
		char const* text = nxLibTranslateReturnCode(_errorCode);
		this->errorText = text ? text : "";
		std::string logMsg = std::string("NxLibException ") + errorText + " at " + _itemPath;
		nxLibWriteDebugMessage(0, logMsg.c_str());
	};

	/// The NxLib @rst{:doc:`error code <api:return-codes>`} that caused this exception.
	int getErrorCode() const { return errorCode; }
	/// The text description of the failure as returned by `nxLibTranslateReturnCode()`.
	std::string const& getErrorText() const { return errorText; }
	/// The path to the tree node on which the failed operation was attempted.
	std::string const& getItemPath() const { return itemPath; }

	/// Whether this exception results from the failed execution of an NxLibCommand.
	///
	/// Exceptions resulting from failed command execution can have more information than those generated purely from
	/// @rst{:doc:`C-API error codes <api:return-codes>`}: an additional error symbol (@ref getCommandErrorSymbol()) and
	/// error text (@ref getCommandErrorText()) that were returned by the command and describe the error in more detail.
	bool isCommandExecutionFailure() const { return errorCode == NxLibExecutionFailed; }

	/// The @rst{:doc:`error symbol <tree:execute/default/result/errorsymbol>`} returned by the command execution.
	///
	/// Only available if the exception was thrown because execution of an NxLibCommand failed. You can check this using
	/// @ref isCommandExecutionFailure(). If the node containing the error symbol does not exist the method will throw
	/// another NxLibException.
	std::string getCommandErrorSymbol() const;

	/// The @rst{:doc:`error text <tree:execute/default/result/errortext>`} returned by the command execution.
	///
	/// Only available if the exception was thrown because execution of an NxLibCommand failed. You can check this using
	/// @ref isCommandExecutionFailure(). If the node containing the error text does not exist the method will throw
	/// another NxLibException.
	std::string getCommandErrorText() const;

#	ifndef NXLIB_CPP_COMPATIBILITY_MODE
	/// Token that holds an NxLibItem.
	std::shared_ptr<NxLibItemToken> const& getToken() const { return token; }
#	endif
};

/**
 * This class simplifies the concatenation of string and integer constants to an NxLib compatible item
 * path specification via its `operator[]`. Assignment and comparison operators simplify the usage of NxLib tree
 * items, almost as if using local variables.
 */
class NxLibItem {
protected:
	template <typename T>
	void assignIfGiven(T* ptr, T const& value) const
	{
		if (ptr) (*ptr) = value;
	}
	void checkReturnCode(int returnCode) const
	{
		if (returnCode != NxLibOperationSucceeded) {
			throw NxLibException(path, returnCode);
		}
	}

public:
	/**
	 * The path in the NxLib Tree.
	 */
	std::string path;

public:
	/**
	 * @param _path Path to a node in the NxLib tree. By default (with an empty path), the item references the root
	 * node.
	 */
	explicit NxLibItem(std::string const& _path = "") { this->path = _path; }

public:
	/**
	 * Copies the JSON value from another NxLibItem.
	 * @param otherItem The NxLibItem to copy from.
	 */
	void operator<<(NxLibItem const& otherItem) const { setJson(otherItem.asJson(), true); }

	/**
	 * Sets a new value to the item from a JSON formatted `std::string`.
	 * @param jsonString JSON formatted string.
	 */
	void operator<<(std::string const& jsonString) const { setJson(jsonString, true); }

	/**
	 * Sets a new value to the item from a json formatted char array.
	 * @param jsonString Pointer to a JSON formatted string.
	 */
	void operator<<(char const* const jsonString) const { setJson(jsonString, true); }

	/**
	 * Creates an NxLibItem referencing a subnode of the current item.
	 * @param subItemName Access a JSON object's subitem by its name.
	 * @return An NxLibItem referring to the subitem.
	 */
	NxLibItem const operator[](std::string const& subItemName) const
	{
		return NxLibItem(path + NxLibItemSeparator + subItemName);
	}

	/**
	 * Creates an NxLibItem referencing a subnode of the current item.
	 * @param subItemIndex Access an element of a JSON Array or a JSON Object subitem by its index.
	 * @return An NxLibItem referring to the subitem.
	 */
	NxLibItem const operator[](int subItemIndex) const
	{
		std::ostringstream newPath;
		newPath << path << NxLibItemSeparator << NxLibIndexEscapeChar << subItemIndex;
		return NxLibItem(newPath.str());
	}

	/**
	 * Compare the item's value to a `char const*` constant.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param value The value to compare the item's value with.
	 * @return An integer indicating the result of the comparison.
	 * @retval -1 If the item's value is smaller than the constant.
	 * @retval 0 If the values are equal.
	 * @retval 1 If the item's value is larger than the constant.
	 */
	int compare(int* returnCode, char const* const value) const { return compare(returnCode, std::string(value)); }

	// Specialization for integers below (will always compare as double!)
	/**
	 * Compare the item's value to a constant of type `T`.
	 * @tparam T One of `bool`, `int`, `double` or `std::string`.
	 * @param value The value to compare the item's value with.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return An integer indicating the result of the comparison.
	 * @retval -1 If the item's value is smaller than the constant.
	 * @retval 0 If the values are equal.
	 * @retval 1 If the item's value is larger than the constant.
	 */
	template <typename T>
	int compare(int* returnCode, T const& value) const
	{
		int result;
		T itemValue = as<T>(&result);
		assignIfGiven(returnCode, result);
		if ((result == NxLibItemTypeNotCompatible) || (result == NxLibItemInexistent)) return 2;
		return itemValue == value ? 0 : itemValue < value ? -1 : 1;
	}

	/**
	 * Compare the item's value to a constant of type `T`.
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param value The value to compare the item's value with.
	 * @return An integer indicating the result of the comparison.
	 * @retval -1 If the item's value is smaller than the constant.
	 * @retval 0 If the values are equal.
	 * @retval 1 If the item's value is larger than the constant.
	 */
	template <typename T>
	int compare(T const& value) const
	{
		int r;
		int result = compare(&r, value);
		checkReturnCode(r);
		return result;
	}

	/**
	 * Set the item's value to a constant.
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param value The value to set.
	 */
	template <typename T>
	void set(T const& value) const
	{
#	ifndef NXLIB_CPP_COMPATIBILITY_MODE
		static_assert(
		    !std::is_same<T, NxLibItem>::value,
		    "You tried to assign the path of a temporary NxLibItem instance! If you wanted to copy the referenced tree "
		    "items instead you can use the \"<<\" operator (e.g. \"a << b;\") or the setJson and getJson functions "
		    "(e.g. \"a.setJson(b.asJson());)\"");
#	endif
		int result;
		set(&result, value);
		checkReturnCode(result);
	}

	/**
	 * Set the item's value to an int constant.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param value The value to set.
	 */
	void set(int* returnCode, int value) const { nxLibSetInt(returnCode, path.c_str(), value); }

	/**
	 * Set the item's value to a double constant.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param value The value to set.
	 */
	void set(int* returnCode, double value) const { nxLibSetDouble(returnCode, path.c_str(), value); }

	/**
	 * Set the item's value to a bool constant.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param value The value to set.
	 */
	void set(int* returnCode, bool value) const
	{
		nxLibSetBool(returnCode, path.c_str(), static_cast<NXLIBBOOL>(value));
	}

	/**
	 * Set the item's value to a string constant.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param value The value to set.
	 */
	void set(int* returnCode, std::string const& value) const { set(returnCode, value.c_str()); }

	/**
	 * Set the item's value to a string constant.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param value The value to set.
	 */
	void set(int* returnCode, char const* const value) const { nxLibSetString(returnCode, path.c_str(), value); }

	/**
	 * Set the item's value to Null.
	 */
	void setNull() const
	{
		int result;
		setNull(&result);
		checkReturnCode(result);
	}

	/**
	 * Set the item's value to Null.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} when accessing the item.
	 */
	void setNull(int* returnCode) const { nxLibSetNull(returnCode, path.c_str()); }

	/**
	 * Set the item's value to a new value specified as JSON string. This might also be a representation of an entire
	 * subtree.
	 * @param value The value to set.
	 * @param onlyWriteableNodes Specifies whether the function should try to write each single node into the existing
	 * tree structure instead of replacing the entire subtree. When specifying `true` here the function will not
	 * complain if certain nodes could not be written due to access restrictions. This can be used to restore the state
	 * of the library or the Parameters node of a camera if the tree state has previously been captured via
	 * `nxLibGetJson()`. When this parameter is set to `false`, all items of the entire subtree must not be access
	 * restricted in order for the function call to succeed! If this parameter is set and the given JSON value does not
	 * match the current structure of the node, the function will return the error code `NxLibItemTypeNotCompatible`.
	 * This indicates, that there was not a single node with the same structure as in the JSON string and the function
	 * could never modify any value. In this case you probably tried to apply the JSON value to the wrong path.
	 */
	void setJson(std::string const& value, bool onlyWriteableNodes = false) const
	{
		int result;
		setJson(&result, value, onlyWriteableNodes);
		checkReturnCode(result);
	}

	/**
	 * Set the item's value to a new value specified as JSON string. This might also be a representation of an entire
	 * subtree.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param value The value to set.
	 * @param onlyWriteableNodes Specifies whether the function should try to write each single node into the existing
	 * tree structure instead of replacing the entire subtree. When specifying `true` here the function will not
	 * complain if certain nodes could not be written due to access restrictions. This can be used to restore the state
	 * of the library or the Parameters node of a camera if the tree state has previously been captured via
	 * `nxLibGetJson()`. When this parameter is set to `false`, all items of the entire subtree must not be access
	 * restricted in order for the function call to succeed! If this parameter is set and the given JSON value does not
	 * match the current structure of the node, the function will return the error code `NxLibItemTypeNotCompatible`.
	 * This indicates, that there was not a single node with the same structure as in the JSON string and the function
	 * could never modify any value. In this case you probably tried to apply the JSON value to the wrong path.
	 */
	void setJson(int* returnCode, std::string const& value, bool onlyWriteableNodes = false) const
	{
		nxLibSetJson(returnCode, path.c_str(), value.c_str(), static_cast<NXLIBBOOL>(onlyWriteableNodes));
	}

	/**
	 * Sets a new value to the item. This operator is templated and simply maps to the corresponding `set()` function.
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param value T The value to set on the item.
	 * @return The current item.
	 */
	template <typename T>
	T const& operator=(T const& value) const
	{
		set(value);
		return value;
	}

	/**
	 * Compare the item's value to a constant for equality. All comparison operators are templates and map to a
	 * corresponding call to the `compare()` member function.
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param value The value to compare the item's value with.
	 * @return Boolean result of the comparison.
	 */
	template <typename T>
	bool operator==(T const& value) const
	{
		return compare(value) == 0;
	}

	/**
	 * Compare the item's value to a constant for inequality. All comparison operators are templates and map to a
	 * corresponding call to the `compare()` member function.
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param value The value to compare the item's value with.
	 * @return Boolean result of the comparison.
	 */
	template <typename T>
	bool operator!=(T const& value) const
	{
		return compare(value) != 0;
	}

	/**
	 * Compare the item's value to a constant. All comparison operators are templates and map to a
	 * corresponding call to the `compare()` member function.
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param value The value to compare the item's value with.
	 * @return Boolean result of the comparison.
	 */
	template <typename T>
	bool operator>(T const& value) const
	{
		return compare(value) > 0;
	}

	/**
	 * Compare the item's value to a constant. All comparison operators are templates and map to a
	 * corresponding call to the `compare()` member function.
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param value The value to compare the item's value with.
	 * @return Boolean result of the comparison.
	 */
	template <typename T>
	bool operator<(T const& value) const
	{
		return compare(value) < 0;
	}

	/**
	 * Compare the item's value to a constant. All comparison operators are templates and map to a
	 * corresponding call to the `compare()` member function.
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param value The value to compare the item's value with.
	 * @return Boolean result of the comparison.
	 */
	template <typename T>
	bool operator>=(T const& value) const
	{
		return compare(value) >= 0;
	}

	/**
	 * Compare the item's value to a constant. All comparison operators are templates and map to a
	 * corresponding call to the `compare()` member function.
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param value The value to compare the item's value with.
	 * @return Boolean result of the comparison.
	 */
	template <typename T>
	bool operator<=(T const& value) const
	{
		return compare(value) <= 0;
	}

	// Access functions returning API errors with an int*

	/**
	 * Reads the item's value in the specified format. The format is given as type argument to the template. The
	 * template maps the C++ types `bool`, `int`, `double` and `std::string` to the member functions `asBool()`,
	 * `asInt()`, `asDouble()` and `asString()`.
	 *
	 * When using the specialization for `char const*` please refer to the
	 * @rst{:doc:`C-API string type description <c:types>`} for information about usage restrictions applying to the
	 * returned pointer.
	 *
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return The item value in the type specified as template parameter.
	 */
	template <typename T>
	T as(int* returnCode) const;
	// Specializations after class declaration

	/**
	 * Reads the item's value as an `std::string`.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return The item value as an `std::string`.
	 */
	std::string asString(int* returnCode) const
	{
		NXLIBSTR ret = nxLibGetString(returnCode, path.c_str());
		return ret ? std::string((char const*)ret) : "";
	}

	/**
	 * Reads the item's value as integer.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return The item value as int value.
	 */
	int asInt(int* returnCode) const
	{
		NXLIBINT ret = nxLibGetInt(returnCode, path.c_str());
		return ret;
	}

	/**
	 * Reads the item's value as integer.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return The item value as double value.
	 */
	double asDouble(int* returnCode) const
	{
		double ret = nxLibGetDouble(returnCode, path.c_str());
		return ret;
	}

	/**
	 * Reads the item's value as boolean.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return The item value as boolean value.
	 */
	bool asBool(int* returnCode) const
	{
		bool ret = nxLibGetBool(returnCode, path.c_str()) != NXLIBFALSE;
		return ret;
	}

	/**
	 * Try to read this tree item and return its JSON representation.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param prettyPrint When `true` the JSON string will be formatted with line breaks and tabs.
	 * @param numberPrecision Specifies the accuracy of numbers.
	 * @param allowScientificFormat Specifies whether to allow scientific notation.
	 * @return The item value as JSON string if the item exists. This might also be an entire subtree in JSON
	 * representation.
	 */
	std::string
	asJson(int* returnCode, bool prettyPrint = false, int numberPrecision = 18, bool allowScientificFormat = true) const
	{
		NXLIBSTR ret;
		ret = nxLibGetJson(
		    returnCode, path.c_str(), static_cast<NXLIBBOOL>(prettyPrint), numberPrecision,
		    static_cast<NXLIBBOOL>(allowScientificFormat));
		return ret ? std::string((char const*)ret) : "";
	}

	/**
	 * Retrieves an item value or an entire subtree in JSON representation including item metadata (protection, extended
	 * type, internal flags). This function is only intended to be used by NxTreeEdit to visualize the entire tree
	 * state.
	 * @param returnCode
	 * @param numLevels The depth of the returned subtree. Nodes in lower levels will be omitted.
	 * @param prettyPrint Specify `true` here to format the resulting string with line breaks and tabs.
	 * @param numberPrecision The floating point precision of the returned numbers.
	 * @param allowScientificFormat Specifies whether to allow scientific notation.
	 * @return The item value as JSON string if the item exists with meta data.
	 */
	std::string asJsonMeta(
	    int* returnCode,
	    int numLevels = -1,
	    bool prettyPrint = false,
	    int numberPrecision = 18,
	    bool allowScientificFormat = true) const
	{
		NXLIBSTR ret;
		ret = nxLibGetJsonMeta(
		    returnCode, path.c_str(), numLevels, static_cast<NXLIBBOOL>(prettyPrint), numberPrecision,
		    static_cast<NXLIBBOOL>(allowScientificFormat));
		return ret ? std::string((char const*)ret) : "";
	}

	/**
	 * Try to read this tree item and return its JSON representation.
	 * @param prettyPrint Specify `true` here to format the resulting string with line breaks and tabs.
	 * @param numberPrecision The floating point precision of the returned numbers.
	 * @param allowScientificFormat Specifies whether to allow scientific notation.
	 * @return The item value as JSON string if the item exists.
	 */
	std::string asJson(bool prettyPrint = false, int numberPrecision = 18, bool allowScientificFormat = false) const
	{
		int returnCode;
		std::string result = asJson(&returnCode, prettyPrint, numberPrecision, allowScientificFormat);
		checkReturnCode(returnCode);
		return result;
	}

	/**
	 * Retrieves an item value or an entire subtree in JSON representation including item metadata (protection, extended
	 * type, internal flags). This function is only intended to be used by NxTreeEdit to visualize the entire tree
	 * state.
	 * @param numLevels The depth of the returned subtree. Nodes in lower levels will be omitted.
	 * @param prettyPrint Specify `true` here to format the resulting string with line breaks and tabs.
	 * @param numberPrecision The floating point precision of the returned numbers.
	 * @param allowScientificFormat Specifies whether to allow scientific notation.
	 * @return The item value as JSON string with meta data, if the item exists.
	 */
	std::string asJsonMeta(
	    int numLevels = -1,
	    bool prettyPrint = false,
	    int numberPrecision = 18,
	    bool allowScientificFormat = true) const
	{
		int returnCode;
		std::string result = asJsonMeta(&returnCode, numLevels, prettyPrint, numberPrecision, allowScientificFormat);
		checkReturnCode(returnCode);
		return result;
	}

	/**
	 * Reads the item's value in a specified format.
	 *
	 * When using the specialization for `char const*` please refer to the
	 * @rst{:doc:`C-API string type description <c:types>`} for information about usage restrictions applying to the
	 * returned pointer.
	 *
	 * @tparam T One of `bool`, `int`, `double`, `std::string` or `char const*`.
	 * @return The item value in the specified format.
	 */
	template <typename T>
	T as() const
	{
		int returnCode;
		T value = as<T>(&returnCode);
		checkReturnCode(returnCode);
		return value;
	}

	/**
	 * Reads the item's value as an `std::string`.
	 * @return The item value as an `std::string`.
	 */
	std::string asString() const { return as<std::string>(); }

	/**
	 * Reads the item's value in an integer format.
	 * @return The item value as int value.
	 */
	int asInt() const { return as<int>(); }

	/**
	 * Reads the item's value in double format.
	 * @return The item value as double value.
	 */
	double asDouble() const { return as<double>(); }

	/**
	 * Reads the item's value in bool format.
	 * @return The item value as boolean value.
	 */
	bool asBool() const { return as<bool>(); }

	/**
	 * Returns whether the item is of type Null or not.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`
	 */
	bool isNull(int* returnCode) const { return type(returnCode) == NxLibItemTypeNull; }

	/**
	 * Returns whether the item is of type String or not.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`
	 */
	bool isString(int* returnCode) const { return type(returnCode) == NxLibItemTypeString; }

	/**
	 * Returns whether the item is of type Number or not.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`
	 */
	bool isNumber(int* returnCode) const { return type(returnCode) == NxLibItemTypeNumber; }

	/**
	 * Returns whether the item is of type Bool or not.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`
	 */
	bool isBool(int* returnCode) const { return type(returnCode) == NxLibItemTypeBool; }

	/**
	 * Returns whether the item is of type Array or not.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`
	 */
	bool isArray(int* returnCode) const { return type(returnCode) == NxLibItemTypeArray; }

	/**
	 * Returns whether the item is of type Object or not.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`
	 */
	bool isObject(int* returnCode) const { return type(returnCode) == NxLibItemTypeObject; }

	/**
	 * Returns whether the item is of type Null or not.
	 * @return
	 */
	bool isNull() const
	{
		int result;
		bool res = isNull(&result);
		checkReturnCode(result);
		return res;
	}

	/**
	 * Returns whether the item is of type String or not.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`.
	 */
	bool isString() const
	{
		int result;
		bool res = isString(&result);
		checkReturnCode(result);
		return res;
	}

	/**
	 * Returns whether the item is of type Number or not.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`.
	 */
	bool isNumber() const
	{
		int result;
		bool res = isNumber(&result);
		checkReturnCode(result);
		return res;
	}

	/**
	 * Returns whether the item is of type Bool or not.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`.
	 */
	bool isBool() const
	{
		int result;
		bool res = isBool(&result);
		checkReturnCode(result);
		return res;
	}

	/**
	 * Returns whether the item is of type Array or not.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`.
	 */
	bool isArray() const
	{
		int result;
		bool res = isArray(&result);
		checkReturnCode(result);
		return res;
	}

	/**
	 * Returns whether the item is of type Object or not.
	 * @return `true` if the item was of the corresponding JSON type, otherwise `false`.
	 */
	bool isObject() const
	{
		int result;
		bool res = isObject(&result);
		checkReturnCode(result);
		return res;
	}

	/**
	 * Retrieve the JSON type of this item.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return The item type constant. See `nxLibGetType()` for item types.
	 */
	int type(int* returnCode) const
	{
		NXLIBINT ret = nxLibGetType(returnCode, path.c_str());
		return ret;
	}

	/**
	 * Retrieve the JSON type of this item.
	 * @return The item type constant. See `nxLibGetType()` for item types.
	 */
	int type() const
	{
		int result;
		int typeId = type(&result);
		checkReturnCode(result);
		return typeId;
	}

	/**
	 * Check whether an item with the corresponding path exists in the tree.
	 * @return `true`, if the item exists, `false` otherwise.
	 */
	bool exists() const
	{
		int result;
		bool res = exists(&result);
		checkReturnCode(result);
		return res;
	}

	/**
	 * Check whether an item with the corresponding path exists in the tree.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return `true`, if the item exists, `false` otherwise.
	 */
	bool exists(int* returnCode) const
	{
		NXLIBINT result;
		NXLIBINT ret = nxLibGetType(&result, path.c_str());
		if (result == NxLibOperationSucceeded) {
			assignIfGiven(returnCode, result);
			return ret != NxLibItemTypeInvalid;
		} else if (result == NxLibItemInexistent) {
			assignIfGiven(returnCode, NxLibOperationSucceeded);
		} else {
			assignIfGiven(returnCode, result);
		}
		return false;
	}

	/**
	 * Deletes the item or subtree.
	 */
	void erase() const
	{
		int result;
		erase(&result);
		if (result == NxLibItemInexistent) return;
		checkReturnCode(result);
	}

	/**
	 * Deletes the item or subtree.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 */
	void erase(int* returnCode) const { nxLibErase(returnCode, path.c_str()); }

	/**
	 * Retrieves the item's name.
	 * @note Calling this function is only allowed when accessing object members by index, otherwise the function will
	 * fail.
	 * @return The item's name.
	 */
	std::string name() const
	{
		int result;
		std::string res = name(&result);
		checkReturnCode(result);
		return res;
	}

	/**
	 * Retrieves the item's name.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return The item's name.
	 */
	std::string name(int* returnCode) const
	{
		NXLIBSTR ret = nxLibGetName(returnCode, path.c_str());
		return ret ? std::string((char const*)ret) : "";
	}

	/**
	 * Retrieves the number of subitems of an @rst{:doc:`Object <api:types>`} or @rst{:doc:`Array <api:types>`}.
	 * Calling the function on scalar nodes will return an error.
	 * @note Calling this function on a @rst{:doc:`Binary <api:types>`} node will retrieve the binary data size in
	 * bytes, but it is recommended to use `getBinaryDataInfo()` instead to get and verify all format properties of the
	 * binary blob before retrieving it.
	 * @return The number of subitems.
	 */
	int count() const
	{
		int result;
		int res = count(&result);
		checkReturnCode(result);
		return res;
	}

	/**
	 * Retrieves the number of subitems of an @rst{:doc:`Object <api:types>`} or @rst{:doc:`Array <api:types>`}.
	 * Calling the function on scalar nodes will return an error.
	 * @note Calling this function on a @rst{:doc:`Binary <api:types>`} node will retrieve the binary data size in
	 * bytes, but it is recommended to use `getBinaryDataInfo()` instead to get and verify all format properties of the
	 * binary blob before retrieving it.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @return The number of subitems.
	 */
	int count(int* returnCode) const
	{
		NXLIBINT ret = nxLibGetCount(returnCode, path.c_str());
		return ret;
	}

	/**
	 * Reads the properties of a @rst{:doc:`Binary <api:types>`} node. You can omit retrieving any of the properties by
	 * setting the corresponding parameter pointer to `nullptr`.
	 * @param width Width of the image.
	 * @param height Height of the image.
	 * @param channels Number of channels, i.e. elements per pixels.
	 * @param bytesPerElement Size of each element.
	 * @param isFloat Indicates whether elements are an IEEE float type of the corresponding size.
	 * @param timestamp The @rst{:doc:`timestamp <api:types>`} when the binary blob was generated.
	 */
	void getBinaryDataInfo(
	    int* width,
	    int* height,
	    int* channels,
	    int* bytesPerElement,
	    bool* isFloat,
	    double* timestamp) const
	{
		int result;
		getBinaryDataInfo(&result, width, height, channels, bytesPerElement, isFloat, timestamp);
		checkReturnCode(result);
	}

	/**
	 * Reads the properties of a @rst{:doc:`Binary <api:types>`} node. You can omit retrieving any of the properties by
	 * setting the corresponding parameter pointer to `nullptr`.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param width Width of the image.
	 * @param height Height of the image.
	 * @param channels Number of channels, i.e. elements per pixels.
	 * @param bytesPerElement Size of each element.
	 * @param isFloat Indicates whether elements are an IEEE float type of the corresponding size.
	 * @param timestamp The @rst{:doc:`timestamp <api:types>`} when the binary blob was generated.
	 */
	void getBinaryDataInfo(
	    int* returnCode,
	    int* width,
	    int* height,
	    int* channels,
	    int* bytesPerElement,
	    bool* isFloat,
	    double* timestamp) const
	{
		NXLIBBOOL isFloat_ = NXLIBFALSE;
		nxLibGetBinaryInfo(returnCode, path.c_str(), width, height, channels, bytesPerElement, &isFloat_, timestamp);
		assignIfGiven(isFloat, isFloat_ != NXLIBFALSE);
	}

	/**
	 * Retrieves data of a @rst{:doc:`Binary <api:types>`} item.
	 * @param bufferPointer Pointer to a buffer where the binary data will be copied to. The buffer must have been
	 * allocated by the caller.
	 * @param bufferSize Size of the allocated buffer in bytes.
	 * @param numBytesCopied Pointer to an integer, can be `nullptr`. If non-zero the value will be set to the number of
	 * bytes actually copied to the user's buffer.
	 * @param timestamp The @rst{:doc:`timestamp <api:types>`} when the binary blob was generated. This pointer can be
	 * set to `nullptr` when the time stamp should not be retrieved.
	 */
	void getBinaryData(void* bufferPointer, int bufferSize, int* numBytesCopied, double* timestamp) const
	{
		int result;
		getBinaryData(&result, bufferPointer, bufferSize, numBytesCopied, timestamp);
		checkReturnCode(result);
	}

	/**
	 * Retrieves data of a @rst{:doc:`Binary <api:types>`} item.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param bufferPointer Pointer to a buffer where the binary data will be copied to. The buffer must have been
	 * allocated by the caller.
	 * @param bufferSize Size of the allocated buffer in bytes.
	 * @param numBytesCopied Pointer to an integer, can be `nullptr`. If non-zero the value will be set to the number of
	 * bytes actually copied to the user's buffer.
	 * @param timestamp The @rst{:doc:`timestamp <api:types>`} when the binary blob was generated. This pointer can be
	 * set to `nullptr` when the time stamp should not be retrieved.
	 */
	void getBinaryData(int* returnCode, void* bufferPointer, int bufferSize, int* numBytesCopied, double* timestamp)
	    const
	{
		nxLibGetBinary(returnCode, path.c_str(), bufferPointer, bufferSize, numBytesCopied, timestamp);
	}

	/**
	 * Sets data of a @rst{:doc:`Binary <api:types>`} item.
	 * @param bufferPointer Pointer to a buffer where the binary data will be copied from.
	 * @param bufferSize Size of the data to be copied in bytes.
	 */
	void setBinaryData(void const* bufferPointer, int bufferSize) const
	{
		int result;
		setBinaryData(&result, bufferPointer, bufferSize);
		checkReturnCode(result);
	}

	/**
	 * Sets data of a @rst{:doc:`Binary <api:types>`} item.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param bufferPointer Pointer to a buffer where the binary data will be copied from.
	 * @param bufferSize Size of the data to be copied in bytes.
	 */
	void setBinaryData(int* returnCode, void const* bufferPointer, int bufferSize) const
	{
		nxLibSetBinary(returnCode, path.c_str(), bufferPointer, bufferSize);
	}

	/**
	 * Retrieves data of a @rst{:doc:`Binary <api:types>`} item.
	 * @note This function takes an `std::vector<T>` as buffer.
	 * @tparam T The element type of the `std::vector` buffer.
	 * @param bufferVector An `std::vector` that should be filled with the binary data.
	 * @param timestamp The @rst{:doc:`timestamp <api:types>`} when the binary blob was generated. This pointer can be
	 * set to `nullptr` when the time stamp should not be retrieved.
	 */
	template <typename T>
	void getBinaryData(std::vector<T>& bufferVector, double* timestamp) const
	{
		int result;
		getBinaryData(&result, bufferVector, timestamp);
		checkReturnCode(result);
	}

	/**
	 * Sets data of a @rst{:doc:`Binary <api:types>`} item.
	 * @note This function takes a `std::vector<T>` as buffer.
	 * @tparam T The element type of the `std::vector` buffer.
	 * @param bufferVector The `std::vector` to take the binary data from.
	 */
	template <typename T>
	void setBinaryData(std::vector<T> const& bufferVector) const
	{
		int result;
		setBinaryData(&result, bufferVector);
		checkReturnCode(result);
	}

	/**
	 * Sets data of a @rst{:doc:`Binary <api:types>`} item.
	 * @tparam T The element type of the `std::vector`.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param bufferVector An `std::vector` from which the binary data should be copied.
	 */
	template <typename T>
	void setBinaryData(int* returnCode, std::vector<T> const& bufferVector) const
	{
		setBinaryData(returnCode, bufferVector.empty() ? 0 : &bufferVector[0], (int)bufferVector.size() * sizeof(T));
	}

	/**
	 * Sets data of a @rst{:doc:`Binary <api:types>`} item.
	 * @tparam T Element type of the `std::vector`.
	 * @param bufferVector A `std::vector` from which the binary data should be copied.
	 * @param width The width of the array (consecutive elements in memory).
	 * @param height The height of the array (number of rows of `width` * `channelCount` elements)
	 * @param channelCount Number of channels for a single item.
	 * @param isFloat Indicates whether elements are an IEEE float type of the corresponding size.
	 */
	template <typename T>
	void setBinaryData(std::vector<T> const& bufferVector, int width, int height, int channelCount, bool isFloat) const
	{
		int result;
		setBinaryData(&result, bufferVector, width, height, channelCount, isFloat);
		checkReturnCode(result);
	}

	/**
	 * Sets data of a @rst{:doc:`Binary <api:types>`} item.
	 * @tparam T Element type of the `std::vector`.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param bufferVector An `std::vector` from which the binary data should be copied.
	 * @param width The width of the array (consecutive elements in memory).
	 * @param height The height of the array (number of rows of `width` * `channelCount` elements)
	 * @param channelCount Number of channels for a single item.
	 * @param isFloat Indicates whether elements are an IEEE float type of the corresponding size.
	 */
	template <typename T>
	void setBinaryData(
	    int* returnCode,
	    std::vector<T> const& bufferVector,
	    int width,
	    int height,
	    int channelCount,
	    bool isFloat) const
	{
		assert(width * height * channelCount == (int)bufferVector.size());
		nxLibSetBinaryFormatted(
		    returnCode, path.c_str(), bufferVector.empty() ? 0 : &bufferVector[0], width, height, channelCount,
		    sizeof(T), isFloat);
	}

	/**
	 * Sets data of a @rst{:doc:`Binary <api:types>`} item.
	 * @tparam T Element type the buffer array.
	 * @param bufferVector Pointer to a buffer that holds the data to copy from.
	 * @param width The width of the array (consecutive elements in memory).
	 * @param height The height of the array (number of rows of `width` * `channelCount` elements)
	 * @param channelCount Number of channels for a single item.
	 * @param isFloat Indicates whether elements are an IEEE float type of the corresponding size.
	 */
	template <typename T>
	void setBinaryData(T const* const bufferVector, int width, int height, int channelCount, bool isFloat) const
	{
		int result;
		setBinaryData(&result, bufferVector, width, height, channelCount, isFloat);
		checkReturnCode(result);
	}

	/**
	 *
	 * @tparam T Element type the buffer array.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param bufferVector Pointer to a buffer that holds the data to copy from.
	 * @param width The width of the array (consecutive elements in memory).
	 * @param height The height of the array (number of rows of `width` * `channelCount` elements)
	 * @param channelCount Number of channels for a single item.
	 * @param isFloat Indicates whether elements are an IEEE float type of the corresponding size.
	 */
	template <typename T>
	void setBinaryData(
	    int* returnCode,
	    T const* const bufferVector,
	    int width,
	    int height,
	    int channelCount,
	    bool isFloat) const
	{
		nxLibSetBinaryFormatted(
		    returnCode, path.c_str(), bufferVector, width, height, channelCount, sizeof(T), isFloat);
	}
#	if defined(EIGEN_MAJOR_VERSION) && EIGEN_MAJOR_VERSION >= 3

	/**
	 * Set the item's value to a binary from a eigen matrix.
	 * @param matrix The Eigen Matrix to copy from.
	 */
	template <int Rows, int Cols, int Opt, int MaxRows, int MaxCols>
	void set(Eigen::Matrix<double, Rows, Cols, Opt, MaxRows, MaxCols> const& matrix) const
	{
		int result;
		set(&result, matrix);
		checkReturnCode(result);
	}

	/**
	 * Set the item's value to a binary from a eigen matrix.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param matrix The Eigen Matrix to copy from.
	 */
	template <int Rows, int Cols, int Opt, int MaxRows, int MaxCols>
	void set(int* returnCode, Eigen::Matrix<double, Rows, Cols, Opt, MaxRows, MaxCols> const& matrix) const
	{
		int result = NxLibOperationSucceeded;
		set(&result);
		if (result == NxLibOperationSucceeded) {
			int r = (int)matrix.rows();
			int c = (int)matrix.cols();

			for (int x = c - 1; x >= 0; x--) {
				for (int y = r - 1; y >= 0; y--) {
					(*this)[x][y].set(&result, matrix(y, x));
					if (result != NxLibOperationSucceeded) break;
				}
				if (result != NxLibOperationSucceeded) break;
			}
		}
		if (returnCode) *returnCode = result;
	}
#	endif
#	if defined(CV_MAJOR_VERSION) && CV_MAJOR_VERSION > 1

	/**
	 * Retrieves data of a Binary item.
	 * @param cvMat An OpenCV matrix into which the binary node content should be copied. OpenCV interface functions are
	 * only available when CV_MAJOR_VERSION is defined while compiling the nxLibFunctions.h header file.
	 * @param timestamp The @rst{:doc:`timestamp <api:types>`} when the binary blob was generated. This pointer can be
	 * set to `nullptr` when the time stamp should not be retrieved.
	 */
	void getBinaryData(cv::Mat& cvMat, double* timestamp) const
	{
		int result;
		getBinaryData(&result, cvMat, timestamp);
		checkReturnCode(result);
	}

	/**
	 * Retrieves data of a Binary item.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param cvMat An OpenCV matrix into which the binary node content should be copied. OpenCV interface functions are
	 * only available when CV_MAJOR_VERSION is defined while compiling the nxLibFunctions.h header file.
	 * @param timestamp The @rst{:doc:`timestamp <api:types>`} when the binary blob was generated. This pointer can be
	 * set to `nullptr` when the time stamp should not be retrieved.
	 */
	void getBinaryData(int* returnCode, cv::Mat& cvMat, double* timestamp) const
	{
		int width = 0, height = 0, type = CV_8UC1, channels = 1, bpe = 1;
		bool isFlt;
		getBinaryDataInfo(returnCode, &width, &height, &channels, &bpe, &isFlt, 0);
		if (returnCode && (*returnCode != NxLibOperationSucceeded)) return;
		if (isFlt) {
			switch (channels) {
				case 1: type = (bpe == 4) ? CV_32FC1 : CV_64FC1; break;
				case 2: type = (bpe == 4) ? CV_32FC2 : CV_64FC2; break;
				case 3: type = (bpe == 4) ? CV_32FC3 : CV_64FC3; break;
				case 4: type = (bpe == 4) ? CV_32FC4 : CV_64FC4; break;
			}
		} else {
			switch (channels) {
				case 1:
					switch (bpe) {
						case 1: type = CV_8UC1; break;
						case 2: type = CV_16SC1; break;
						case 4: type = CV_32SC1; break;
					}
					break;
				case 2:
					switch (bpe) {
						case 1: type = CV_8UC2; break;
						case 2: type = CV_16SC2; break;
						case 4: type = CV_32SC2; break;
					}
					break;
				case 3:
					switch (bpe) {
						case 1: type = CV_8UC3; break;
						case 2: type = CV_16SC3; break;
						case 4: type = CV_32SC3; break;
					}
					break;
				case 4:
					switch (bpe) {
						case 1: type = CV_8UC4; break;
						case 2: type = CV_16SC4; break;
						case 4: type = CV_32SC4; break;
					}
					break;
			}
		}
		cvMat.create(height, width, type);
		int bytesCopied = 0;
		getBinaryData(returnCode, cvMat.ptr(), height * ((int)cvMat.step), &bytesCopied, timestamp);
		assert(bytesCopied == height * ((int)cvMat.step));
	}

	/**
	 *
	 * @param cvMat
	 */
	void setBinaryData(cv::Mat const& cvMat) const
	{
		int result;
		setBinaryData(&result, cvMat);
		checkReturnCode(result);
	}

	/**
	 * Sets data of a Binary item.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param cvMat An OpenCV matrix that should be copied to the binary node. OpenCV interface functions are only
	 * available when CV_MAJOR_VERSION is defined while compiling the nxLibFunctions.h header file.
	 */
	void setBinaryData(int* returnCode, cv::Mat const& cvMat) const
	{
		int channels = 1, bpe = 1;
		bool isFlt = false;
		int type = cvMat.type();
		switch (type) {
			case CV_8SC1:
			case CV_8UC1:
				channels = 1;
				bpe = 1;
				isFlt = false;
				break;
			case CV_8SC2:
			case CV_8UC2:
				channels = 2;
				bpe = 1;
				isFlt = false;
				break;
			case CV_8SC3:
			case CV_8UC3:
				channels = 3;
				bpe = 1;
				isFlt = false;
				break;
			case CV_8SC4:
			case CV_8UC4:
				channels = 4;
				bpe = 1;
				isFlt = false;
				break;
			case CV_16SC1:
			case CV_16UC1:
				channels = 1;
				bpe = 2;
				isFlt = false;
				break;
			case CV_16SC2:
			case CV_16UC2:
				channels = 2;
				bpe = 2;
				isFlt = false;
				break;
			case CV_16SC3:
			case CV_16UC3:
				channels = 3;
				bpe = 2;
				isFlt = false;
				break;
			case CV_16SC4:
			case CV_16UC4:
				channels = 4;
				bpe = 2;
				isFlt = false;
				break;
			case CV_32SC1:
				channels = 1;
				bpe = 4;
				isFlt = false;
				break;
			case CV_32SC2:
				channels = 2;
				bpe = 4;
				isFlt = false;
				break;
			case CV_32SC3:
				channels = 3;
				bpe = 4;
				isFlt = false;
				break;
			case CV_32SC4:
				channels = 4;
				bpe = 4;
				isFlt = false;
				break;
			case CV_32FC1:
				channels = 1;
				bpe = 4;
				isFlt = true;
				break;
			case CV_32FC2:
				channels = 2;
				bpe = 4;
				isFlt = true;
				break;
			case CV_32FC3:
				channels = 3;
				bpe = 4;
				isFlt = true;
				break;
			case CV_32FC4:
				channels = 4;
				bpe = 4;
				isFlt = true;
				break;
			case CV_64FC1:
				channels = 1;
				bpe = 8;
				isFlt = true;
				break;
			case CV_64FC2:
				channels = 2;
				bpe = 8;
				isFlt = true;
				break;
			case CV_64FC3:
				channels = 3;
				bpe = 8;
				isFlt = true;
				break;
			case CV_64FC4:
				channels = 4;
				bpe = 8;
				isFlt = true;
				break;
		}

		nxLibSetBinaryFormatted(
		    returnCode, path.c_str(), cvMat.ptr(), cvMat.cols, cvMat.rows, channels, bpe,
		    static_cast<NXLIBBOOL>(isFlt));
	}
#	endif

	/**
	 * Retrieves data of a Binary item.
	 * @tparam T Element type of the `std::vector` buffer.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 * @param bufferVector An `std::vector` that should be filled with the binary data.
	 * @param timestamp The @rst{:doc:`timestamp <api:types>`} when the binary blob was generated. This pointer can be
	 * set to `nullptr` when the time stamp should not be retrieved.
	 */
	template <typename T>
	void getBinaryData(int* returnCode, std::vector<T>& bufferVector, double* timestamp) const
	{
		int result;
		int sizeNeeded = count(&result);
		if (result != NxLibOperationSucceeded) {
			assignIfGiven(returnCode, result);
			return;
		}
		NXLIBINT elementsNeeded = sizeNeeded / sizeof(T);
		if (sizeNeeded % sizeof(T) != 0) {
			assignIfGiven(returnCode, NxLibBufferNotDivisibleByElementSize);
			return;
		}
		bufferVector.resize(elementsNeeded);
		NXLIBINT bytesCopied;
		char fakeBuffer;
		getBinaryData(
		    &result, bufferVector.size() > 0 ? (void*)&bufferVector[0] : (void*)&fakeBuffer, elementsNeeded * sizeof(T),
		    &bytesCopied, timestamp);
		if ((result == NxLibOperationSucceeded) && (bytesCopied != sizeNeeded)) result = NxLibInternalError;
		assignIfGiven(returnCode, result);
	}

	/**
	 *
	 */
	void waitForChange() const
	{
		int result;
		waitForChange(&result);
		checkReturnCode(result);
	}

	/**
	 * Waits for any change to the item.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the item.
	 */
	void waitForChange(int* returnCode) const { nxLibWaitForChange(returnCode, path.c_str()); }

	/**
	 * Waits until the item has a specific type.
	 * @param type The type constant to wait for. See `nxLibGetType()` for valid types.
	 * @param waitForEqual Indicates whether to wait until the item has the indicated type, or whether to wait until the
	 * item has a type other than the specified one.
	 */
	void waitForType(int type, bool waitForEqual) const
	{
		int result;
		waitForType(&result, type, waitForEqual);
		checkReturnCode(result);
	}

	/**
	 * Waits until the item has a specific type.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the time.
	 * @param type The type constant to wait for. See `nxLibGetType()` for valid types.
	 * @param waitForEqual Indicates whether to wait until the item has the indicated type, or whether to wait until the
	 * item has a type other than the specified one.
	 */
	void waitForType(int* returnCode, int type, bool waitForEqual) const
	{
		nxLibWaitForType(returnCode, path.c_str(), type, static_cast<NXLIBBOOL>(waitForEqual));
	}

	/**
	 *
	 * @param value
	 * @param waitForEqual
	 */
	void waitForValue(int value, bool waitForEqual) const
	{
		int result;
		waitForValue(&result, value, waitForEqual);
		checkReturnCode(result);
	}

	/**
	 * Waits until the item has a specific value.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} returned when accessing the time.
	 * @param value The value to wait for.
	 * @param waitForEqual Indicates whether to wait until the item has the given value, or whether to wait until the
	 * item has a value different than the specified one.
	 */
	void waitForValue(int* returnCode, int value, bool waitForEqual) const
	{
		nxLibWaitForIntValue(returnCode, path.c_str(), value, static_cast<NXLIBBOOL>(waitForEqual));
	}

	/**
	 * Waits until the item has a specific value.
	 * @param value The value to wait for.
	 * @param waitForEqual Indicates whether to wait until the item has the given value, or whether to wait until the
	 * item has a value different than the specified one.
	 */
	void waitForValue(double value, bool waitForEqual) const
	{
		int result;
		waitForValue(&result, value, waitForEqual);
		checkReturnCode(result);
	}

	/**
	 * Waits until the item has a specific value.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} when accessing the item.
	 * @param value The value to wait for.
	 * @param waitForEqual Indicates whether to wait until the item has the given value, or whether to wait until the
	 * item has a value different than the specified one.
	 */
	void waitForValue(int* returnCode, double value, bool waitForEqual) const
	{
		nxLibWaitForDoubleValue(returnCode, path.c_str(), value, static_cast<NXLIBBOOL>(waitForEqual));
	}

	/**
	 * Waits until the item has a specific value.
	 * @param value The value to wait for.
	 * @param waitForEqual Indicates whether to wait until the item has the given value, or whether to wait until the
	 * item has a value different than the specified one.
	 */
	void waitForValue(bool value, bool waitForEqual) const
	{
		int result;
		waitForValue(&result, value, waitForEqual);
		checkReturnCode(result);
	}

	/**
	 * Waits until the item has a specific value.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} when accessing the item.
	 * @param value The value to wait for.
	 * @param waitForEqual Indicates whether to wait until the item has the given value, or whether to wait until the
	 * item has a value different than the specified one.
	 */
	void waitForValue(int* returnCode, bool value, bool waitForEqual) const
	{
		nxLibWaitForBoolValue(
		    returnCode, path.c_str(), static_cast<NXLIBBOOL>(value), static_cast<NXLIBBOOL>(waitForEqual));
	}

	/**
	 * Waits until the item has a specific value.
	 * @param value The value to wait for.
	 * @param waitForEqual Indicates whether to wait until the item has the given value, or whether to wait until the
	 * item has a value different than the specified one.
	 */
	void waitForValue(char const* const value, bool waitForEqual) const
	{
		int result;
		waitForValue(&result, value, waitForEqual);
		checkReturnCode(result);
	}

	/**
	 * Waits until the item has a specific value.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} when accessing the item.
	 * @param value The value to wait for.
	 * @param waitForEqual Indicates whether to wait until the item has the given value, or whether to wait until the
	 * item has a value different than the specified one.
	 */
	void waitForValue(int* returnCode, char const* const value, bool waitForEqual) const
	{
		nxLibWaitForStringValue(returnCode, path.c_str(), value, static_cast<NXLIBBOOL>(waitForEqual));
	}

	/**
	 * Waits until the item has a specific value.
	 * @param value The value to wait for.
	 * @param waitForEqual Indicates whether to wait until the item has the given value, or whether to wait until the
	 * item has a value different than the specified one.
	 */
	void waitForValue(std::string const& value, bool waitForEqual) const
	{
		int result;
		waitForValue(&result, value, waitForEqual);
		checkReturnCode(result);
	}

	/**
	 * Waits until the item has a specific value.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} when accessing the item.
	 * @param value The value to wait for.
	 * @param waitForEqual Indicates whether to wait until the item has the given value, or whether to wait until the
	 * item has a value different than the specified one.
	 */
	void waitForValue(int* returnCode, std::string const& value, bool waitForEqual) const
	{
		int result;
		waitForValue(&result, value.c_str(), waitForEqual);
		assignIfGiven(returnCode, result);
	}

	/**
	 * Checks whether the item name is taken. If yes, the item gets a new unique name. Usually used to create a
	 * temporary unique NxLibItem as slot for an NxLibCommand.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} of operation.
	 * @param itemName The base name of the item. Defaults to `"Temporary"` if left empty.
	 * @return A unique NxLibItem.
	 */
	NxLibItem makeUniqueItem(int* returnCode, std::string const& itemName = "") const
	{
		NXLIBSTR ret = nxLibMakeUniqueItem(returnCode, path.c_str(), itemName.c_str());
		if (ret) {
			return NxLibItem(ret);
		}
		return NxLibItem();
	}

	/**
	 * Checks whether the item name is taken. If yes, the item gets a new unique name. Usually used to create a
	 * temporary unique NxLibItem as slot for an NxLibCommand.
	 * @param itemName The base name of the item. Defaults to `"Temporary"` if left empty.
	 * @return A unique NxLibItem.
	 */
	NxLibItem makeUniqueItem(std::string const& itemName = "") const
	{
		int result;
		NxLibItem item = makeUniqueItem(&result, itemName.c_str());
		checkReturnCode(result);
		return item;
	}
};

#	ifndef DOXYGEN_IGNORE // Ignore template specializations
template <>
inline int NxLibItem::compare<int>(int* returnCode, int const& value) const
{
	return compare<double>(returnCode, value);
}
template <>
inline std::string NxLibItem::as<std::string>(int* returnCode) const
{
	return asString(returnCode);
}
template <>
inline char const* NxLibItem::as<char const*>(int* returnCode) const
{
	return nxLibGetString(returnCode, path.c_str());
}
template <>
inline int NxLibItem::as<int>(int* returnCode) const
{
	return asInt(returnCode);
}
template <>
inline double NxLibItem::as<double>(int* returnCode) const
{
	return asDouble(returnCode);
}
template <>
inline bool NxLibItem::as<bool>(int* returnCode) const
{
	return asBool(returnCode);
}

inline std::string NxLibException::getCommandErrorSymbol() const
{
	return NxLibItem(itemPath)[itmResult][itmErrorSymbol].asString();
}

inline std::string NxLibException::getCommandErrorText() const
{
	return NxLibItem(itemPath)[itmResult][itmErrorText].asString();
}

#	endif // DOXYGEN_IGNORE

#	ifndef NXLIB_CPP_COMPATIBILITY_MODE
/**
 * Holds an NxLibItem. The only purpose of this class is to hold an NxLibItem, especially the ones of
 * temporary NxLibCommand s. If the temporary command execution fails, usually the NxLibItem slot is also
 * destroyed. But the token, holding the NxLibItem, will propagate the item to an NxLibException, if the command
 * execution is not successful and throws.
 */
class NxLibItemToken {
public:
	/**
	 * The NxLibItem, the token should keep alive, till the token itself is destroyed.
	 */
	NxLibItem item;

public:
	/**
	 * Default Constructor.
	 */
	NxLibItemToken() = default;

	/**
	 * Constructor taking an NxLibItem.
	 * @param _item The item the token should hold, till the token itself is destroyed.
	 */
	explicit NxLibItemToken(NxLibItem const& _item)
	: item(_item){};

	/**
	 * Destructor. Erases the item the token has kept alive.
	 */
	~NxLibItemToken()
	{
		try {
			if (!item.path.empty()) item.erase();
		} catch (...) {
		}
	}
};
#	endif

/**
 * This class simplifies the execution of NxLib commands. The `parameters()` and `result()` functions provide access to
 * the corresponding @rst{:doc:`parameter <tree:execute/default/parameters>`} and
 * @rst{:doc:`result <tree:execute/default/result>`} nodes of the tree. The `execute()` method starts the command by
 * writing the command name to the tree and allows for synchronous and asynchronous command execution. The `wait()`
 * method waits for commands executing asynchronously.
 */
class NxLibCommand {
public:
	/**
	 * Name of the command to execute.
	 */
	std::string commandName;

#	ifdef NXLIB_CPP_COMPATIBILITY_MODE
public:
#	else
private:
#	endif
	/**
	 * The NxLibItem the command is executed on. SubItems take the parameters and will also give back the result of the
	 * command executed.
	 */
	NxLibItem commandItem;

private:
#	ifndef NXLIB_CPP_COMPATIBILITY_MODE
	/**
	 * Is used for temporary slots to keep the commandItem alive, if, for example, the execution fails.
	 */
	std::shared_ptr<NxLibItemToken> token; // For temporary slots.
#	else
	/**
	 * Deprecated. If set to `true`, the commandItem will be destroyed after the NxLibItem goes out of scope.
	 */
	bool removeSlotOnDestruction;
#	endif

public:
	/**
	 * @param _commandName Name of the command to execute. This can be overwritten by the `execute()` methods.
	 * @param nodeName Name of the @rst{:doc:`execution slot <tree:execute>`} the command should run in. By default the
	 * class will create a temporary slot used only by this instance of NxLibCommand, which is automatically deleted in
	 * the destructor.
	 */
	explicit NxLibCommand(std::string const& _commandName, std::string const nodeName = "")
	: commandName(_commandName)
	{
		bool const temporaryItem = nodeName.empty();
#	ifdef NXLIB_CPP_COMPATIBILITY_MODE
		removeSlotOnDestruction = temporaryItem;
#	endif
		NxLibItem const exec = NxLibItem()[itmExecute];
		if (temporaryItem) {
			commandItem = exec.makeUniqueItem();
#	ifndef NXLIB_CPP_COMPATIBILITY_MODE
			token = std::make_shared<NxLibItemToken>(commandItem);
#	endif
		} else {
			commandItem = exec[nodeName];
		}
	}

#	ifdef NXLIB_CPP_COMPATIBILITY_MODE
	/**
	 * Destructor. Erases the commandItem if the removeSlotOnDestruction flag was set to `true`.
	 */
	~NxLibCommand()
	{
		try {
			if (removeSlotOnDestruction) commandItem.erase();
		} catch (...) {
		}
	}
#	endif

	// This is always done in the constructor now. The method stays here for API compatibility.

#	ifndef DOXYGEN_IGNORE
	NXLIB_DEPRECATED void createTemporarySlot(std::string const& = "") {}
#	endif // DOXYGEN_IGNORE

protected:
	template <typename T>
	void assignIfGiven(T* ptr, T const& value) const
	{
		if (ptr) (*ptr) = value;
	}
	void checkReturnCode(int returnCode) const
	{
		if (returnCode != NxLibOperationSucceeded) {
#	ifndef NXLIB_CPP_COMPATIBILITY_MODE
			NxLibException exception(slot().path, returnCode, token);
#	else
			NxLibException exception(slot().path, returnCode);
#	endif
			throw exception;
		}
	}

public:
	/**
	 * The slot returns the NxLibItem the NxLibCommand is running on.
	 * @return NxLibItem the command runs on.
	 */
	NxLibItem const& slot() const { return commandItem; }

	/**
	 * The command's @rst{:doc:`parameters <tree:execute/default/parameters>`}, that will be fetched before the command
	 * executes.
	 * @return NxLibItem, in which the parameters are stored.
	 */
	NxLibItem const parameters() const { return slot()[itmParameters]; }

	/**
	 * Remember @rst{:doc:`parameters <tree:execute/default/parameters>`} across command executions in this slot by
	 * setting the @rst{:doc:`persistent parameters <tree:execute/default/persistentparameters>`} node.
	 *
	 * As documented, command parameters are cleared when the command is finished executing. This is disadventageous in
	 * case a command is executed multiple times with (mostly) identical parameters.
	 * @param remember If `true`, parameters are kept across command execution. If `false`, parameters are reset during
	 * command execution.
	 */
	void rememberParameters(bool remember = true) { slot()[itmPersistentParameters] = remember; }

	/**
	 * The command's @rst{:doc:`result <tree:execute/default/result>`} node after the command finishes.
	 * @return NxLibItem, in which the results are stored.
	 */
	NxLibItem const result() const { return slot()[itmResult]; }

	/**
	 * Cancels the current current command. Only works on commands, that support the
	 * @rst{:doc:`commands:break`} command.
	 * @param _wait If `true`, the function waits until execution of the command is finished. If set to
	 * `true`, the function will also throw an NxLibException if the command finished with an error.
	 */
	void cancel(bool _wait = true) const
	{
		try {
			execute(cmdBreak, _wait);
		} catch (NxLibException& e) {
			// Ignore any error from the command when it was canceled successfully.
			if (!_wait || e.getErrorCode() != NxLibExecutionFailed) throw;
		}
	};

	/**
	 * Cancels the current execution of the command.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} of the cancel operation.
	 * @param _wait If `true`, the function waits until execution of the command is finished. If set to
	 * `true`, the function will also throw an NxLibException if the command finished with an error.
	 */
	void cancel(int* returnCode, bool _wait = true) const { execute(returnCode, cmdBreak, _wait); }

	/**
	 * Executes the current command.
	 * @param _wait If `true`, the function waits until execution of the command is finished. If set to
	 * `true`, the function will also throw an NxLibException if the command finished with an error.
	 */
	void execute(bool _wait = true) const { execute("", _wait); }

	/**
	 * Executes the current command.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} of the execution.
	 * @param _wait If `true`, the function waits until execution of the command is finished. If set to
	 * `true`, the function will also throw an NxLibException if the command finished with an error.
	 */
	void execute(int* returnCode, bool _wait = true) const { execute(returnCode, "", _wait); }

	/**
	 * Executes the command with the given name.
	 * @param _commandName Name of the command to execute. This overwrites the command name from the constructor.
	 * @param _wait If `true`, the function waits until execution of the command is finished. If set to
	 * `true`, the function will also throw an NxLibException if the command finished with an error.
	 */
	void execute(char const* _commandName, bool _wait = true) const { execute(std::string(_commandName), _wait); }

	/**
	 * Executes the command with the given name.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} of the execution.
	 * @param _commandName Name of the command to execute. This overwrites the command name from the constructor.
	 * @param _wait If `true`, the function waits until execution of the command is finished. If set to
	 * `true`, the function will also throw an NxLibException if the command finished with an error.
	 */
	void execute(int* returnCode, char const* _commandName, bool _wait = true) const
	{
		execute(returnCode, std::string(_commandName), _wait);
	}

	/**
	 * Executes the command with the given name.
	 * @param _commandName Name of the command to execute. This overwrites the command name from the constructor.
	 * @param _wait If `true`, the function waits until execution of the command is finished. If set to
	 * `true`, the function will also throw an NxLibException if the command finished with an error.
	 */
	void execute(std::string const& _commandName, bool _wait = true) const
	{
		int result;
		execute(&result, _commandName, _wait);
		checkReturnCode(result);
	}

	/**
	 * Executes the command with the given name.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} of execution.
	 * @param _commandName Name of the command to execute. This overwrites the command name from the constructor.
	 * @param _wait If `true`, the function waits until execution of the command is finished. If set to
	 * `true`, the function will also throw an NxLibException if the command finished with an error.
	 */
	void execute(int* returnCode, std::string const& _commandName, bool _wait = true) const
	{
		int result;
		NxLibItem functionItem = slot()[itmCommand];
		functionItem.set(&result, _commandName.empty() ? this->commandName : _commandName);
		if (result != NxLibOperationSucceeded) {
			assignIfGiven(returnCode, result);
			return;
		}
		if (_wait) {
			wait(&result);
		}
		assignIfGiven(returnCode, result);
	}

	/**
	 * Start asynchronous execution of the current command.
	 *
	 * This function is equivalent to `execute(returnCode, false)`.
	 */
	void executeAsync() const { execute(false); }

	/**
	 * Start asynchronous execution of the current command.
	 *
	 * This function is equivalent to `execute(returnCode, false)`.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} of starting the execution.
	 */
	void executeAsync(int* returnCode) const { execute(returnCode, false); }

	/**
	 * Start asynchronous execution of the command with the given name.
	 *
	 * This function is equivalent to `execute(_commandName, false)`.
	 * @param _commandName Name of the command to execute. This overwrites the command name from the constructor.
	 */
	void executeAsync(std::string const& _commandName) const { execute(_commandName, false); }

	/**
	 * Start asynchronous execution of the command with the given name.
	 *
	 * This function is equivalent to `execute(returnCode, _commandName, false)`.
	 * @param returnCode The @rst{:doc:`error code <api:return-codes>`} of starting the execution.
	 * @param _commandName Name of the command to execute. This overwrites the command name from the constructor.
	 */
	void executeAsync(int* returnCode, std::string const& _commandName) const
	{
		execute(returnCode, _commandName, false);
	}

	/**
	 * Checks whether the command execution has already finished.
	 * @return `true`, when NxLib is not currently executing a command in `slot()`, i.e. the
	 * @rst{:doc:`command node <tree:execute/default/command>`} is Null. Otherwise `false`.
	 */
	bool finished() const
	{
		int result;
		bool fin = finished(&result);
		checkReturnCode(result);
		return fin;
	}

	/**
	 * Checks whether the command execution has already finished.
	 * @param returnCode The @rst{:doc:`API return code <api:return-codes>`}.
	 * @return `true`, when NxLib is not currently executing a command in `slot()`, i.e. the
	 * @rst{:doc:`command node <tree:execute/default/command>`} is Null. Otherwise `false`.
	 */
	bool finished(int* returnCode) const
	{
		return !slot()[itmCommand].exists(returnCode) || (slot()[itmCommand].type(returnCode) == NxLibItemTypeNull);
	}

	/**
	 * Waits for the command execution to finish.
	 *
	 * @code
	 * command.executeAsync(&returnCode);
	 * if (returnCode == NxLibOperationSucceeded) {
	 *   command.wait(&returnCode);
	 * }
	 * @endcode
	 * is equivalent to
	 * @code
	 * command.execute(&returnCode);
	 * @endcode
	 * @param returnCode The @rst{:doc:`API return code <api:return-codes>`}.
	 */
	void wait(int* returnCode) const
	{
		int result;
		NxLibItem functionItem = slot()[itmCommand];
		bool exists = functionItem.exists(&result);
		if (result != NxLibOperationSucceeded) {
			assignIfGiven(returnCode, result);
			return;
		} else if (!exists) {
			assignIfGiven(returnCode, NxLibOperationSucceeded);
			return;
		}
		functionItem.waitForType(&result, NxLibItemTypeNull, true);
		if (result != NxLibOperationSucceeded) {
			assignIfGiven(returnCode, result);
			return;
		}
		result = successful(returnCode) ? NxLibOperationSucceeded : NxLibExecutionFailed;
		assignIfGiven(returnCode, result);
	}

	/**
	 * Waits for the command execution to finish. The function will throw an NxLibException if the command finished
	 * with an error.
	 *
	 * @code
	 * command.executeAsync();
	 * command.wait();
	 * @endcode
	 * is equivalent to
	 * @code
	 * command.execute();
	 * @endcode
	 */
	void wait() const
	{
		int result;
		wait(&result);
		checkReturnCode(result);
	}

	/**
	 * Checks whether the previous command execution is finished and was successful and throws an NxLibException if not.
	 */
	void assertSuccessful() const
	{
		if (!finished() || !successful()) checkReturnCode(NxLibExecutionFailed);
	}

	/**
	 * Checks whether the previous command execution was successful.
	 * @return `true`, when the previous NxLib command execution was successful, i.e. there is no
	 * @rst{:doc:`tree:execute/default/result/errorsymbol`} node under the Result node. Otherwise `false`.
	 */
	bool successful() const
	{
		int result;
		bool ok = successful(&result);
		checkReturnCode(result);
		return ok;
	}

	/**
	 * Checks whether the previous command execution was successful.
	 * @param returnCode The @rst{:doc:`API return code <api:return-codes>`}.
	 * @return `true`, when the previous NxLib command execution was successful, i.e. there is no
	 * @rst{:doc:`tree:execute/default/result/errorsymbol`} node under the Result node. Otherwise `false`.
	 */
	bool successful(int* returnCode) const
	{
		int err;
		bool res = !(result()[itmErrorSymbol].exists(&err));
		assignIfGiven(returnCode, err);
		return res;
	}
};

/**
 * This class simplifies the usage of debug blocks. It will call `nxLibOpenDebugBlock()` during its construction and
 * `nxLibCloseDebugBlock()` during its destruction.
 */
class NxLibDebugBlock {
public:
	/**
	 * Constructor. Calls `nxLibOpenDebugBlock()`.
	 * @param name Debug block name.
	 * @param level Debug level of this block, see `nxLibOpenDebugBlock()` for the possible levels.
	 */
	NxLibDebugBlock(char const* name, NxLibDebugLevel level = nxdInherit)
	{
		int result;
		nxLibOpenDebugBlock(&result, name, level);
		if (result != NxLibOperationSucceeded) {
			throw NxLibException("", result);
		}
	}

	/**
	 * Destructor. Calls `nxLibCloseDebugBlock()`.
	 */
	~NxLibDebugBlock() { nxLibCloseDebugBlock(0); }

private:
	NxLibDebugBlock(NxLibDebugBlock const&) {}
};

/**
 * Checks the API return code given and throws, if the API code is not NxLibOperationSucceeded.
 * @param returnCode API return code to check with.
 */
static void ATTR_UNUSED nxLibCheckReturnCode(int returnCode)
{
	if (returnCode != NxLibOperationSucceeded) throw NxLibException("", returnCode);
}

/**
 * @cppoverload{nxLibWriteDebugMessage(int32_t*, const char*)}
 **/
static void ATTR_UNUSED nxLibWriteDebugMessage(std::string const& message)
{
	int result;
	nxLibWriteDebugMessage(&result, message.c_str());
	nxLibCheckReturnCode(result);
}

/**
 * @cppoverload{nxLibGetDebugMessages(int32_t *)}
 **/
static std::string ATTR_UNUSED nxLibGetDebugMessages()
{
	int result;
	std::string messages;
	char const* str = nxLibGetDebugMessages(&result);
	nxLibCheckReturnCode(result);
	if (str) messages = std::string(str);
	return messages;
}

/**
 * @cppoverload{nxLibSetDebugThreadName(int32_t *, const char*)}
 **/
static void ATTR_UNUSED nxLibSetDebugThreadName(std::string const& threadName)
{
	int result;
	nxLibSetDebugThreadName(&result, threadName.c_str());
	nxLibCheckReturnCode(result);
}

/**
 * @cppoverload{nxLibGetDebugBuffer(int32_t*, void*, int32_t, int32_t*, int32_t)}
 * @tparam T Element type of the `std::vector`.
 * @param data Vector to hold the retrieved debug information. The size of the vector is automatically adjusted to hold
 * all the available debug information.
 * @param clearRead If set to `true`, the operation will also remove the retrieved content from the debug buffer.
 * @param overflow Indicates if the buffer has overflowed since the last read operation and older debug information has
 * been lost. This boolean indicates a result equal to `NxLibDebugMessageOverflow`, in which case no exception
 * is thrown.
 */
template <typename T>
static void ATTR_UNUSED nxLibGetDebugBuffer(std::vector<T>& data, bool clearRead = true, bool* overflow = 0)
{
	if (overflow) *overflow = false;
	int chunkSize = 0;
	for (int i = 0; i < 2; i++) {
		data.resize(chunkSize / sizeof(T));
		int result;
		nxLibGetDebugBuffer(
		    &result, data.size() ? &data[0] : 0, (int)data.size() * sizeof(T), &chunkSize, clearRead && (i % 2));
		if (result == NxLibDebugMessageOverflow) {
			if (overflow) *overflow = true;
		} else {
			nxLibCheckReturnCode(result);
		}
	}
}

#endif /* __cplusplus */

#endif /* __NXLIB_FUNCTIONS_H__ */
