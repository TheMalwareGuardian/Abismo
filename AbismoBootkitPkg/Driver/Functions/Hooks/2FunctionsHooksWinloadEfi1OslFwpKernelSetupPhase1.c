/** @file
	winload.efi!OslFwpKernelSetupPhase1
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>       // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



// Local
#include <Functions/Utils/1FunctionsUtilsStrings.h>
#include <Functions/Utils/3FunctionsUtilsMemory.h>
#include <Functions/Utils/4FunctionsUtilsHeaders.h>
#include <Functions/Utils/9FunctionsUtilsHooks.h>
#include <Functions/4FunctionsPatchHookWindowsKernel.h>



// Structures
#include <Structures/2pe.h>



/**
    Get a loaded module entry from the boot loader's LoadOrderList based on its name.

    This function search for a loaded module entry with a specified name, performing a case-insensitive comparison.

    @param[in]		LoadOrderListHead		A pointer to the head of the boot loader's LoadOrderList.
    @param[in]		ModuleName				The name of the module to search for.

	@retval			!NULL					A pointer to the found module's entry.
    @retval			NULL					The module with the specified name was not found.
**/

PKLDR_DATA_TABLE_ENTRY
EFIAPI
FunctionsHooksWinloadEfi_GetBootLoadedModule(
	IN		LIST_ENTRY*		LoadOrderListHead,
	IN		CHAR16*			ModuleName
	)
{
	// Hello World
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (GetBootLoadedModule) - Hello World\r\n");


	// Bad parameters
	if (ModuleName == NULL || LoadOrderListHead == NULL)
	{
		return NULL;
	}


	// Iterate through LoadOrderList to find desired module
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (GetBootLoadedModule) - Iterate through LoadOrderList to find desired module\r\n");
	for (LIST_ENTRY* ListEntry = LoadOrderListHead->ForwardLink; ListEntry != LoadOrderListHead; ListEntry = ListEntry->ForwardLink)
	{
		// Entry
		CONST PBLDR_DATA_TABLE_ENTRY Entry = (PBLDR_DATA_TABLE_ENTRY)BASE_CR(ListEntry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

        // Perform a case-insensitive comparison of module names
		if (Entry != NULL && FunctionsUtilsStrings_StrniCmp(Entry->KldrEntry.BaseDllName.Buffer, ModuleName, (Entry->KldrEntry.BaseDllName.Length / sizeof(CHAR16))) == 0)
		{
			// Match found
			//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (GetBootLoadedModule) - Match found\r\n");
			return &Entry->KldrEntry;
		}
	}


	// Match not found
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (GetBootLoadedModule) - Match not found\r\n");


	// Bye
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (GetBootLoadedModule) - Bye\r\n");
	return NULL;
}



/**
	Hooks the OslFwpKernelSetupPhase1 function.

	This function replaces the original OslFwpKernelSetupPhase1 function with a custom hook that allows for additional functionality.

	@param[in]		LoaderBlock			A pointer to the Loader Parameter Block.

	@retval			EFI_SUCCESS			The OslFwpKernelSetupPhase1 function was successfully hooked.
	@retval			other				An error occurred during the hooking process.
**/

EFI_STATUS
EFIAPI
FunctionsHooksWinloadEfi_OslFwpKernelSetupPhase1(
	IN		PLOADER_PARAMETER_BLOCK		LoaderBlock
	)
{
    // Set console text output color
    //gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);


	// Hello World
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Hello World\r\n");


	// Restore original function bytes that were replaced with hook
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Restore original function bytes that were replaced with hook\r\n");
	FunctionsUtilsMemory_CopyMemory((VOID*)gOriginalWinloadOslFwpKernelSetupPhase1, gBytesWinloadEfiOslFwpKernelSetupPhase1Backup, sizeof(gFauxCallHookTemplate));


	// Get kernel entry from loader block's LoadOrderList
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Get kernel entry from loader block's LoadOrderList\r\n");
	UINT8* LoadOrderListHeadAddress = (UINT8*)&LoaderBlock->LoadOrderListHead;
	CONST PKLDR_DATA_TABLE_ENTRY KernelEntry = FunctionsHooksWinloadEfi_GetBootLoadedModule((LIST_ENTRY*)LoadOrderListHeadAddress, L"ntoskrnl.exe");

	// Failed
	if (KernelEntry == NULL)
	{
		//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Failed: KernelEntry not found\r\n");

		// Call original transfering execution back to winload!OslFwpKernelSetupPhase1
		//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Call original transfering execution back to winload!OslFwpKernelSetupPhase1\r\n");
		return ((t_OslFwpKernelSetupPhase1)gOriginalWinloadOslFwpKernelSetupPhase1)(LoaderBlock);
	}


	// Get kernel base and headers
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Get kernel base and headers\r\n");
	VOID* KernelBase = KernelEntry->DllBase;
	CONST UINT32 KernelSize = KernelEntry->SizeOfImage;
	CONST PEFI_IMAGE_NT_HEADERS NtHeaders = KernelBase != NULL && KernelSize > 0 ? FunctionsUtilsHeaders_GetNTHeadersPEFile(KernelBase, (UINTN)KernelSize) : NULL;

	// Failed
	if (KernelBase == NULL || KernelSize == 0)
	{
		//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Failed: Kernel base not found\r\n");

		// Call original transfering execution back to winload!OslFwpKernelSetupPhase1
		//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Call original transfering execution back to winload!OslFwpKernelSetupPhase1\r\n");
		return ((t_OslFwpKernelSetupPhase1)gOriginalWinloadOslFwpKernelSetupPhase1)(LoaderBlock);
	}

	// Patch Windows Kernel
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Patch Windows Kernel\r\n");
	EFI_STATUS Status = FunctionsWindowsKernel_PatchNtoskrnlExe(KernelBase, NtHeaders);

	// Failed
    if (EFI_ERROR(Status))
    {
		//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - FunctionsWindowsKernel_PatchNtoskrnlExe failed: %llx (%r).\r\n", Status, Status);
        return Status;
    }


	// Bye
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Bye\r\n");


    // Call original transfering execution back to winload!OslFwpKernelSetupPhase1
	//Print(L"DriverDXERuntime [Functions/Hooks/WinloadEfiOslFwpKernelSetupPhase1.c] (Hook) - Call original transfering execution back to winload!OslFwpKernelSetupPhase1\r\n");
	return ((t_OslFwpKernelSetupPhase1)gOriginalWinloadOslFwpKernelSetupPhase1)(LoaderBlock);
}
