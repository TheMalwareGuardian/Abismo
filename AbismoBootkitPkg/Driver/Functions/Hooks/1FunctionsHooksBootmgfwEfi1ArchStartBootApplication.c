/** @file
	bootmgfw.efi!ImgArchStartBootApplication
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>       // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



// Local
#include <Functions/Utils/3FunctionsUtilsMemory.h>
#include <Functions/Utils/4FunctionsUtilsHeaders.h>
#include <Functions/Utils/9FunctionsUtilsHooks.h>
#include <Functions/3FunctionsPatchHookWindowsOSLoader.h>



// Structures
#include <Structures/2pe.h>
#include <Structures/1arc.h>



/**
	Hooks the ArchStartBootApplication function.

	This function replaces the original ArchStartBootApplication function with a custom hook that allows for additional functionality.

	@param[in]		AppEntry					A pointer to the BL_APPLICATION_ENTRY structure.
	@param[in]		ImageBase					A pointer to the base address of the loaded image.
	@param[in]		ImageSize					The size, in bytes, of the loaded image.
	@param[in]		BootOption					The boot option identifier.
	@param[out]		ReturnArguments				A pointer to the BL_RETURN_ARGUMENTS structure.
	x@param[in]		OriginalFunction			A pointer to the original ArchStartBootApplication function.
	x@param[in]		OriginalFunctionBytes		A pointer to the original bytes of the ArchStartBootApplication function.

	@retval			EFI_SUCCESS					The ArchStartBootApplication function was successfully hooked.
	@retval			other						An error occurred during the hooking process.
**/

EFI_STATUS
EFIAPI
FunctionsHooksBootmgfwEfi_ArchStartBootApplication(
	IN		PBL_APPLICATION_ENTRY		AppEntry,
	IN		VOID*						ImageBase,
	IN		UINT32						ImageSize,
	IN		UINT32						BootOption,
	OUT		PBL_RETURN_ARGUMENTS		ReturnArguments
	)
{
	// Set console text output color
    //gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);


	// Hello World
	//Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - Hello World\r\n");


	// Restore original function bytes that were replaced with hook
	//Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - Restore original function bytes that were replaced with hook\r\n");
	FunctionsUtilsMemory_CopyMemory(gOriginalBootmgfwImgArchStartBootApplication, gBytesBootmgfwEfiImgArchStartBootApplication, sizeof(gFauxCallHookTemplate));


	// Get NT Headers
	//Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - Get NT Headers\r\n");
	CONST PEFI_IMAGE_NT_HEADERS NtHeaders = FunctionsUtilsHeaders_GetNTHeadersPEFile(ImageBase, ImageSize);

	// Failed		
	if (NtHeaders == NULL)
	{
		//Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - FunctionsUtilsHeaders_GetNTHeadersPEFile failed: PE image at 0x%p with size 0x%lx is invalid\r\n", ImageBase, ImageSize);

		// Call original
		//Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - Call original\r\n");
		return ((t_ImgArchStartBootApplication)gOriginalBootmgfwImgArchStartBootApplication)(AppEntry, ImageBase, ImageSize, BootOption, ReturnArguments);
	}


	// Print info
	//Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - Print info\r\n");
	//Print(L"bootmgfw!ImgArchStartBootApplication\r\n");
	//Print(L"    -> ImageBase: 0x%p\r\n", ImageBase);
	//Print(L"    -> ImageSize: %lx\r\n", ImageSize);
	//Print(L"	-> EntryPoint: 0x%p\r\n", ((UINT8*)ImageBase + HEADER_FIELD(NtHeaders, AddressOfEntryPoint)));
	//Print(L"    	-> AppEntry:\r\n");
	//Print(L"    	-> Signature: %a\r\n", AppEntry->Signature);
	//Print(L"    	-> Flags: %lx\r\n", AppEntry->Flags);
	//Print(L"    	-> GUID: %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\r\n", AppEntry->Guid.Data1, AppEntry->Guid.Data2, AppEntry->Guid.Data3, AppEntry->Guid.Data4[0], AppEntry->Guid.Data4[1], AppEntry->Guid.Data4[2], AppEntry->Guid.Data4[3], AppEntry->Guid.Data4[4], AppEntry->Guid.Data4[5], AppEntry->Guid.Data4[6], AppEntry->Guid.Data4[7]);


	// Patch Windows OS Loader
	//Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - Patch Windows OS Loader\r\n");
	EFI_STATUS Status = FunctionsWindowsOSLoader_PatchWinloadEfi(ImageBase, NtHeaders);
	//gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);

	// Failed
    if (EFI_ERROR(Status))
    {
        //Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - FunctionsWindowsOSLoader_PatchWinloadEfi failed: %llx (%r).\r\n", Status, Status);
        return Status;
    }


	// Bye
	//Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - Bye\r\n");


	// Call original
	//Print(L"DriverDXERuntime [Functions/Hooks/BootmgfwEfiArchStartBootApplication.c] (Hook) - Call original\r\n");
    return ((t_ImgArchStartBootApplication)gOriginalBootmgfwImgArchStartBootApplication)(AppEntry, ImageBase, ImageSize, BootOption, ReturnArguments);
}
