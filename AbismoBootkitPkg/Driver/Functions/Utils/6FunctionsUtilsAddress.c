/** @file
	Address
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



// Structures
#include <Structures/2pe.h>



/**
	Searches for the start address of a function.

	This function searches for the start address of a function within a PE image, identified by its ImageBase and NT Headers. It starts the search from the given AddressInFunction, which is typically an address within the function. The function uses information from the image's NT Headers to locate the function's start address and returns it as a pointer.

	@param[in]		ImageBase				The base address of the PE image.
	@param[in]		NtHeaders				A pointer to the NT Headers of the PE image.
	@param[in]		AddressInFunction		An address within the function for which the start address is sought.

	@retval			UINT8*					Returns a pointer to the start address of the function, or NULL if the function start address could not be determined.
**/

UINT8*
EFIAPI
FunctionsUtilsAddress_FindStartAddress(
	IN		CONST UINT8*				ImageBase,
	IN		PEFI_IMAGE_NT_HEADERS		NtHeaders,
	IN		CONST UINT8*				AddressInFunction
	)
{
    // Hello World
    //Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Hello World\r\n");


	// Checks
	//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Checks\r\n");

    // Check if AddressInFunction pointer is null
	if (AddressInFunction == NULL)
	{
		//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Failed: AddressInFunction pointer is null\r\n");
		return NULL;
	}

    // Check if the image has exception data directory
	if (NtHeaders->OptionalHeader.NumberOfRvaAndSizes <= EFI_IMAGE_DIRECTORY_ENTRY_EXCEPTION)
	{
		//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Failed: Image has exception data directory\r\n");
		return NULL;
	}


	// Get function table size
	//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Get function table size\r\n");
	CONST UINT32 FunctionTableSize = NtHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size;

    // Check if function table size is zero
	if (FunctionTableSize == 0)
	{
		//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Failed: Function table size is zero\r\n");
		return NULL;
	}


	// Get function table
	//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Get function table\r\n");
	CONST PIMAGE_RUNTIME_FUNCTION_ENTRY FunctionTable = (PIMAGE_RUNTIME_FUNCTION_ENTRY)(ImageBase + NtHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);


	// Calculate relative address to perform binary search
	//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Calculate relative address to perform binary search\r\n");
	CONST UINT32 RelativeAddress = (UINT32)(AddressInFunction - ImageBase);
	PIMAGE_RUNTIME_FUNCTION_ENTRY FunctionEntry = NULL;
	INT32 Low = 0;
	INT32 High = (INT32)(FunctionTableSize / sizeof(IMAGE_RUNTIME_FUNCTION_ENTRY)) - 1;


	// Find the function that contains the specified address
	//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Find the function that contains the specified address\r\n");
	while (High >= Low)
	{
		CONST INT32 Middle = (Low + High) >> 1;
		FunctionEntry = &FunctionTable[Middle];
        // Check if address is within current function entry's range
		if (RelativeAddress < FunctionEntry->BeginAddress)
		{
			High = Middle - 1;
		}
		else if (RelativeAddress >= FunctionEntry->EndAddress)
		{
			Low = Middle + 1;
		}
		else
		{
			break;
		}
	}


    // Valid function entry found
	if (High >= Low)
	{
		//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Valid function entry found\r\n");

		// Function entry marked as indirect
		if ((FunctionEntry->u.UnwindData & RUNTIME_FUNCTION_INDIRECT) != 0)
		{
			//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Function entry marked as indirect\r\n");
			FunctionEntry = (PIMAGE_RUNTIME_FUNCTION_ENTRY)(FunctionEntry->u.UnwindData + ImageBase - 1);
		}

		// Match found
		//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Match found\r\n");

        // Retrieve the address where the function begins
		//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Retrieve the address where the function begins\r\n");
		return (UINT8*)ImageBase + FunctionEntry->BeginAddress;
	}


    // Match not found
	//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Match not found\r\n");


    // Bye
	//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (FindStartAddress) - Bye\r\n");
	return NULL;
}



/**
	Converts a Relative Virtual Address (RVA) to a file offset based on the provided NT headers.

	This function takes the RVA and maps it to the corresponding file offset within the PE file. It iterates through the section headers to find the section that contains the RVA and calculates the corresponding file offset based on the RVA's location within that section.

	@param[in]		NtHeaders		A pointer to the PE file's NT headers.
	@param[in]		Rva				The Relative Virtual Address (RVA) to convert to a file offset.

	@retval			UINT32			The file offset corresponding to the given RVA.
**/

UINT32
EFIAPI
FunctionsUtilsAddress_RelativeVirtualAddressToOffset(
	IN		PEFI_IMAGE_NT_HEADERS		NtHeaders,
	IN		UINT32						Rva
	)
{
	// Hello World
    //Print(L"DriverDXERuntime [Functions/Utils/Address.c] (RelativeVirtualAddressToOffset) - Hello World\r\n");


	// Variables
	PEFI_IMAGE_SECTION_HEADER SectionHeaders = IMAGE_FIRST_SECTION(NtHeaders);
	CONST UINT16 NumberOfSections = NtHeaders->FileHeader.NumberOfSections;
	UINT32 Result = 0;


	// Iterate through the section headers to find the section containing the RVA
	//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (RelativeVirtualAddressToOffset) - Iterate through section headers to find the section containing the RVA\r\n");
	for (UINT16 i = 0; i < NumberOfSections; ++i)
	{
		if (SectionHeaders->VirtualAddress <= Rva && SectionHeaders->VirtualAddress + SectionHeaders->Misc.VirtualSize > Rva)
		{
			// Calculate file offset based on RVA's location within the section
			//Print(L"DriverDXERuntime [Functions/Utils/Address.c] (RelativeVirtualAddressToOffset) - Calculate file offset based on RVA's location within the section\r\n");
			Result = Rva - SectionHeaders->VirtualAddress + SectionHeaders->PointerToRawData;
			break;
		}
		SectionHeaders++;
	}


	// Bye
    //Print(L"DriverDXERuntime [Functions/Utils/Address.c] (RelativeVirtualAddressToOffset) - Bye\r\n");
	return Result;
}
