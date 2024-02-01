/** @file
	Pattern
**/



// Includes
#include <Uefi.h>
#include <Library/UefiLib.h>



/**
	Searches for a specified pattern in a memory region.

	This function searches for the specified pattern (byte sequence) in the memory region defined. It compares the memory content against the provided pattern, which can contain wildcard bytes to represent don't care values. If the pattern is found within the specified memory region, the pointer is updated with the address of the first occurrence.

	@param[in]		Pattern						The pattern to search for.
	@param[in]		Wildcard					The wildcard value used in the pattern.
	@param[in]		PatternLength				The length of the pattern to search for.
	@param[in]		Base						The base address of the memory region to search within.
	@param[in]		Size						The size of the memory region to search within.
	@param[out]		Found						On return, points to the address of the first occurrence of the pattern if found.

	@retval			EFI_SUCCESS					The pattern was found, and Found is updated with the address of the first occurrence.
	@retval			EFI_NOT_FOUND				The pattern was not found within the specified memory region.
	@retval			EFI_INVALID_PARAMETER		One or more input parameters are invalid.
**/

EFI_STATUS
EFIAPI
FunctionsUtilsPattern_FindPattern(
	IN		CONST UINT8*	Pattern,
	IN		UINT8			Wildcard,
	IN		UINT32			PatternLength,
	IN		CONST VOID*		Base,
	IN		UINT32			Size,
	OUT		VOID			**Found
	)
{
    // Hello World
    //Print(L"DriverDXERuntime [Functions/Utils/Pattern.c] (FindPattern) - Hello World\r\n");


    // Check if essential parameters are NULL
	//Print(L"DriverDXERuntime [Functions/Utils/Pattern.c] (FindPattern) - Checks\r\n");
	if (Found == NULL || Pattern == NULL || Base == NULL)
	{
		//Print(L"DriverDXERuntime [Functions/Utils/Pattern.c] (FindPattern) - Failed: Essential parameters are NULL\r\n");
		return EFI_INVALID_PARAMETER;
	}


	// Variables
	*Found = NULL;


    // Iterate through memory
	//Print(L"DriverDXERuntime [Functions/Utils/Pattern.c] (FindPattern) - Iterate through memory\r\n");
	for (UINT8 *Address = (UINT8*)Base; Address < (UINT8*)((UINTN)Base + Size - PatternLength); ++Address)
	{
		UINT32 i;
		for (i = 0; i < PatternLength; ++i)
		{
			if (Pattern[i] != Wildcard && (*(Address + i) != Pattern[i]))
				break;
		}

        // Match found
		if (i == PatternLength)
		{
			//Print(L"DriverDXERuntime [Functions/Utils/Pattern.c] (FindPattern) - Match found\r\n");

			// Set found pointer to matching address
			//Print(L"DriverDXERuntime [Functions/Utils/Pattern.c] (FindPattern) - Set found pointer to matching address\r\n");
			*Found = (VOID*)Address;
			return EFI_SUCCESS;
		}
	}


	// Match not found
	//Print(L"DriverDXERuntime [Functions/Utils/Pattern.c] (FindPattern) - Match not found\r\n");


    // Bye
	//Print(L"DriverDXERuntime [Functions/Utils/Pattern.c] (FindPattern) - Bye\r\n");
	return EFI_NOT_FOUND;
}
