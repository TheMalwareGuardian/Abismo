/** @file
    Windows Boot Manager
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>       // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
#include <Library/BaseMemoryLib.h>                  // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h



// Local
#include <Functions/Utils/3FunctionsUtilsMemory.h>
#include <Functions/Utils/4FunctionsUtilsHeaders.h>
#include <Functions/Utils/5FunctionsUtilsPattern.h>
#include <Functions/Utils/6FunctionsUtilsAddress.h>
#include <Functions/Utils/9FunctionsUtilsHooks.h>
#include <Functions/Utils/10FunctionsUtilsSignatures.h>
#include <Functions/Hooks/1FunctionsHooksBootmgfwEfi1ArchStartBootApplication.h>



// Structures
#include <Structures/2pe.h>



/**
    Patch to Windows Boot Manager (bootmgfw.efi) image.

    This function applies a custom patch to modify the behavior of the Windows Boot Manager.

    @param[in]      ImageBase           A pointer to the base address of the loaded image.
    @param[in]      ImageSize           The size, in bytes, of the loaded image.

    @retval         EFI_SUCCESS         The patch was successfully applied.
    @retval         EFI_NOT_FOUND       The necesarry functions were not found.
    @retval         other               An error occurred during the patching process.
**/

EFI_STATUS
EFIAPI
FunctionsWindowsBootManager_PatchBootmgfwEfi(
    IN      VOID*       ImageBase,
    IN      UINTN       ImageSize
    )
{
    // Set console text output color
    //gST->ConOut->SetAttribute(gST->ConOut, EFI_YELLOW | EFI_BACKGROUND_BLACK);


	// Hello World
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Hello World\r\n");


    // Get NT Headers
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Get NT Headers\r\n");
    CONST PEFI_IMAGE_NT_HEADERS NtHeaders = FunctionsUtilsHeaders_GetNTHeadersPEFile(ImageBase, ImageSize);

    // Failed
    if (NtHeaders == NULL)
    {
		//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - FunctionsUtilsHeaders_GetNTHeadersPEFile failed: PE image at 0x%p with size 0x%llx is invalid\r\n", ImageBase, ImageSize);
        return EFI_LOAD_ERROR;
    }


    // Find code section
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Find code section\r\n");
    CONST PEFI_IMAGE_SECTION_HEADER CodeSection = IMAGE_FIRST_SECTION(NtHeaders);


	// Find ArchStartBootApplication signature
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Find ArchStartBootApplication signature\r\n");
    UINT8* Found = NULL;
    EFI_STATUS Status = FunctionsUtilsPattern_FindPattern(SigImgArchStartBootApplication, 0xCC, sizeof(SigImgArchStartBootApplication), (UINT8*)ImageBase + CodeSection->VirtualAddress, CodeSection->SizeOfRawData, (VOID**)&Found);

    // Failed
    if (EFI_ERROR(Status))
    {
		//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - FunctionsUtilsPattern_FindPattern failed: Failed to find bootmgfw!ImgArchStartBootApplication signature, %llx (%r).\r\n", Status, Status);
        return Status;
    }


    // Find ArchStartBootApplication start
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Find ArchStartBootApplication start\r\n");
    gOriginalBootmgfwImgArchStartBootApplication = (VOID*)FunctionsUtilsAddress_FindStartAddress(ImageBase, NtHeaders, Found);

    // Failed
    if (gOriginalBootmgfwImgArchStartBootApplication == NULL)
    {
		//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - FunctionsUtilsAddress_FindStartAddress failed: Failed to find bootmgfw!ImgArchStartBootApplication start\r\n");
        return EFI_NOT_FOUND;
    }


    // Hook bootmgfw.efi!ArchStartBootApplication
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Hook bootmgfw.efi!ArchStartBootApplication\r\n");
    VOID* HookAddress = (VOID*)&FunctionsHooksBootmgfwEfi_ArchStartBootApplication;
    //gST->ConOut->SetAttribute(gST->ConOut, EFI_YELLOW | EFI_BACKGROUND_BLACK);


	// Raise TPL
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Raise TPL\r\n");
    CONST EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-raisetpl


    // Copy ArchStartBootApplication bytes
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Copy ArchStartBootApplication bytes\r\n");
    CopyMem(gBytesBootmgfwEfiImgArchStartBootApplication, gOriginalBootmgfwImgArchStartBootApplication, sizeof(gFauxCallHookTemplate)); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-copymem


    // Place faux call (push addr, ret) at the start of the original function to transfer the execution to the hooked function
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Place faux call at the start of the original function\r\n");

	// Place hook template in original address
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Place hook template in original address\r\n");
    FunctionsUtilsMemory_CopyMemory(gOriginalBootmgfwImgArchStartBootApplication, gFauxCallHookTemplate, sizeof(gFauxCallHookTemplate));

	// Place HookAddress in template
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Place HookAddress in template\r\n");
    FunctionsUtilsMemory_CopyMemory((UINT8*)gOriginalBootmgfwImgArchStartBootApplication + gFauxCallHookTemplateAddressOffset, (UINTN*)&HookAddress, sizeof(UINTN));


	// Restore TPL
	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Restore TPL\r\n");
    gBS->RestoreTPL(Tpl); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-restoretpl


	// Bye
  	//Print(L"DriverDXERuntime [Functions/WindowsBootManager.c] (PatchBootmgfwEfi) - Bye\r\n");
    return EFI_SUCCESS;
}
