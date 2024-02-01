/** @file
	Tables
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



// Local
#include <Functions/Utils/7FunctionsUtilsPortableExecutable.h>



// Structures
#include <Structures/2pe.h>



/**
	Searches for the Import Address Table (IAT) address of a specified function in a PE image.

	This function analyze a PE image to find and return the address of the Import Address Table (IAT) entry for a specified function. The function starts by searching the Import Descriptor Table of the PE image for the specified import DLL name. If the DLL is found, it then iterates through the function imports to locate the target function by name.

	@param[in]		ImageBase				The base address of the PE image.
	@param[in]		NtHeaders				A pointer to the PE image's NT headers.
	@param[in]		ImportDllName			The name of the import DLL to search for.
	@param[in]		FunctionName			The name of the function to find in the import DLL.
	@param[out]		FunctionIATAddress		A pointer to the address of the function's IAT entry if found.

	@retval			EFI_SUCCESS				The function's IAT address was successfully found.
	@retval			EFI_NOT_FOUND			The specified import DLL or function was not found in the PE image.
**/

EFI_STATUS
EFIAPI
FunctionsUtilsTables_FindImportAddressTable(
	IN		CONST VOID*					ImageBase,
	IN		PEFI_IMAGE_NT_HEADERS		NtHeaders,
	IN		CONST CHAR8*				ImportDllName,
	IN		CONST CHAR8*				FunctionName,
	OUT		VOID						**FunctionIATAddress
	)
{
	// Hello World
    //Print(L"DriverDXERuntime [Functions/Utils/Tables.c] (FindImportAddressTable) - Hello World\r\n");


	// Variables
	*FunctionIATAddress = NULL;
	UINT32 ImportDirSize;


	// Get Import Descriptor Table
	//Print(L"DriverDXERuntime [Functions/Utils/Tables.c] (FindImportAddressTable) - Get Import Descriptor Table\r\n");
	CONST PIMAGE_IMPORT_DESCRIPTOR DescriptorTable = FunctionsUtilsPortableExecutable_RtlpImageDirectoryEntryToDataEx(ImageBase, TRUE, EFI_IMAGE_DIRECTORY_ENTRY_IMPORT, &ImportDirSize);

	// Failed
	if (ImportDirSize == 0 || DescriptorTable == NULL)
	{
		//Print(L"DriverDXERuntime [Functions/Utils/Tables.c] (FindImportAddressTable) - FunctionsUtilsPortableExecutable_RtlpImageDirectoryEntryToDataEx failed\r\n");
		return EFI_NOT_FOUND;
	}


	// Count number of DLLs
	//Print(L"DriverDXERuntime [Functions/Utils/Tables.c] (FindImportAddressTable) - Count number of DLLs\r\n");
	PIMAGE_IMPORT_DESCRIPTOR Entry = DescriptorTable;
	UINT32 DllCount;
	for (DllCount = 0; Entry->u.OriginalFirstThunk != 0; ++DllCount)
	{
		Entry = (PIMAGE_IMPORT_DESCRIPTOR)((UINTN)(Entry) + sizeof(IMAGE_IMPORT_DESCRIPTOR));
	}


	// Iterate over import descriptors
	//Print(L"DriverDXERuntime [Functions/Utils/Tables.c] (FindImportAddressTable) - Iterate over import descriptors\r\n");
	for (UINT32 i = 0; i < DllCount; ++i)
	{
		// Is this the import descriptor for our DLL?
		CONST PIMAGE_IMPORT_DESCRIPTOR Descriptor = &DescriptorTable[i];
		CONST CHAR8* DllName = (CHAR8*)((UINTN)ImageBase + Descriptor->Name);
	
		// If DLL name is NULL or not match the specified ImportDllName, continue searching
		if (DllName == NULL || AsciiStriCmp(DllName, ImportDllName) != 0)
		{
			continue;
		}

		// Get thunk data
		CONST VOID* ThunkData = (VOID*)((UINTN)ImageBase + (Descriptor->u.OriginalFirstThunk != 0 ? Descriptor->u.OriginalFirstThunk : Descriptor->FirstThunk));

		// Headers
		if (IMAGE64(NtHeaders))
		{
			PIMAGE_THUNK_DATA64 ThunkEntry = (PIMAGE_THUNK_DATA64)ThunkData;
			for (UINT32 j = 0; ThunkEntry->u1.AddressOfData > 0; ++j)
			{
				CONST PIMAGE_IMPORT_BY_NAME ImportByName = (PIMAGE_IMPORT_BY_NAME)((UINTN)ImageBase + ThunkEntry->u1.AddressOfData);

				// Import found
				if ((ThunkEntry->u1.Ordinal & IMAGE_ORDINAL_FLAG64) == 0 && ImportByName->Name[0] != '\0' && AsciiStriCmp(ImportByName->Name, FunctionName) == 0)
				{
					//Print(L"DriverDXERuntime [Functions/Utils/Tables.c] (FindImportAddressTable) - Import found\r\n");
					CONST UINT32 Rva = Descriptor->FirstThunk + j * sizeof(UINTN);
					VOID* Va = (VOID*)((UINTN)(ImageBase) + Rva);
					*FunctionIATAddress = Va;
					return EFI_SUCCESS;
				}

				// Update
				ThunkEntry = (PIMAGE_THUNK_DATA64)((UINTN)ThunkEntry + sizeof(IMAGE_THUNK_DATA64));
			}
		}
		else
		{
			PIMAGE_THUNK_DATA32 ThunkEntry = (PIMAGE_THUNK_DATA32)ThunkData;
			for (UINT32 j = 0; ThunkEntry->u1.AddressOfData > 0; ++j)
			{
				CONST PIMAGE_IMPORT_BY_NAME ImportByName = (PIMAGE_IMPORT_BY_NAME)((UINTN)ImageBase + ThunkEntry->u1.AddressOfData);

				// Import found
				if ((ThunkEntry->u1.Ordinal & IMAGE_ORDINAL_FLAG32) == 0 && ImportByName->Name[0] != '\0' && AsciiStriCmp(ImportByName->Name, FunctionName) == 0)
				{
					//Print(L"DriverDXERuntime [Functions/Utils/Tables.c] (FindImportAddressTable) - Import found\r\n");
					CONST UINT32 Rva = Descriptor->FirstThunk + j * sizeof(UINTN);
					VOID* Va = (VOID*)((UINTN)ImageBase + Rva);
					*FunctionIATAddress = Va;
					return EFI_SUCCESS;
				}

				// Update
				ThunkEntry = (PIMAGE_THUNK_DATA32)((UINTN)ThunkEntry + sizeof(IMAGE_THUNK_DATA32));
			}
		}
	}


	// Bye
    //Print(L"DriverDXERuntime [Functions/Utils/Tables.c] (FindImportAddressTable) - Bye\r\n");
	return EFI_NOT_FOUND;
}
