/** @file
	Strings
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



/**
    Performs a case-insensitive comparison of two ASCII strings with a specified length.

    This function compares two ASCII strings, FirstString and SecondString, up to the specified Length characters. The comparison is case-insensitive. If FirstString is equal to SecondString, it returns 0. If FirstString is less than SecondString, it returns a negative value. If FirstString is greater than SecondString, it returns a positive value.

    @param[in]	FirstString		A pointer to the first ASCII string.
    @param[in]	SecondString	A pointer to the second ASCII string.
    @param[in]	Length			The maximum number of characters to compare.

    @retval		0				The two strings are equal up to Length characters.
    @retval		Negative		FirstString is less than SecondString.
    @retval		Positive		FirstString is greater than SecondString.
**/

INTN
EFIAPI
FunctionsUtilsStrings_StrniCmp(
	IN		CHAR16		*FirstString,
	IN		CHAR16		*SecondString,
	IN		UINTN		Length
	)
{
	// Hello World
  	//Print(L"DriverDXERuntime [Functions/Utils/Strings.c] (StrniCmp) - Hello World\r\n");


    // Check if either of the input strings is NULL or if Length is 0
	//Print(L"DriverDXERuntime [Functions/Utils/Strings.c] (StrniCmp) - Checks\r\n");
	if (FirstString == NULL || SecondString == NULL || Length == 0)
	{
		return 0;
	}


    // Convert first characters of both strings to uppercase
	//Print(L"DriverDXERuntime [Functions/Utils/Strings.c] (StrniCmp) - Convert first characters of both strings to uppercase\r\n");
	CHAR16 UpperFirstChar = CharToUpper(*FirstString);
	CHAR16 UpperSecondChar = CharToUpper(*SecondString);


    // Iterate through the strings
	//Print(L"DriverDXERuntime [Functions/Utils/Strings.c] (StrniCmp) - Iterate through the strings\r\n");
	while ((*FirstString != L'\0') && (*SecondString != L'\0') && (UpperFirstChar == UpperSecondChar) && (Length > 1))
	{
		FirstString++;
		SecondString++;
		UpperFirstChar = CharToUpper(*FirstString);
		UpperSecondChar = CharToUpper(*SecondString);
		Length--;
	}


	// Bye
  	//Print(L"DriverDXERuntime [Functions/Utils/Strings.c] (StrniCmp) - Bye\r\n");


	// Return result of case-insensitive comparison
  	//Print(L"DriverDXERuntime [Functions/Utils/Strings.c] (StrniCmp) - Return result of case-insensitive comparison\r\n");
	return UpperFirstChar - UpperSecondChar;
} // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strnicmp-wcsnicmp-mbsnicmp-strnicmp-l-wcsnicmp-l-mbsnicmp-l
