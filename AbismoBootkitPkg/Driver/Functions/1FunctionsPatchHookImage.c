/** @file
    Image
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>       // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
#include <Library/SynchronizationLib.h>             // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/SynchronizationLib.h
#include <Library/DevicePathLib.h>                  // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h



// Protocols
#include <Protocol/LoadedImage.h>                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h



// Local
#include <Functions/Utils/2FunctionsUtilsRegisters.h>
#include <Functions/Utils/9FunctionsUtilsHooks.h>
#include <Functions/2FunctionsPatchHookWindowsBootManager.h>



/**
    Hooks a service table pointer, replacing its original function with a new one.

    This function modifies an entry in the EFI service table. It replaces the existing function pointed to by ServiceTableFunction with a new function provided by NewFunction. Additionally, it returns the address of the original function to allow for later restoration if needed.

    @param[in]      ServiceTableHeader          A pointer to the EFI service table header. This header is updated as part of the hooking process.
    @param[in]      ServiceTableFunction        A double reference to the EFI service table function. This argument is expected to point to the function to be replaced.
    @param[in]      NewFunction                 A pointer to the new function that will replace the existing function in the EFI service table.

    @retval         VOID*                       Returns a pointer to the original function that was in the service table before the modification.
**/

VOID*
FunctionsImage_HookServiceTablePointer(
	IN      EFI_TABLE_HEADER        *ServiceTableHeader,
	IN      VOID                    **ServiceTableFunction,
	IN      VOID                    *NewFunction
	)
{
    // Set console text output color
    //gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);


	// Hello World
    //Print(L"DriverDXERuntime [Functions/Image.c] (HookServiceTablePointer) - Hello World\r\n");


	// Variables
	BOOLEAN WpEnabled;


    // Raise TPL
	//Print(L"DriverDXERuntime [Functions/Image.c] (HookServiceTablePointer) - Raise TPL\r\n");
    EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-raisetpl


	// Disable write protection
    //Print(L"DriverDXERuntime [Functions/Image.c] (HookServiceTablePointer) - Disable write protection\r\n");
	FunctionsUtilsRegisters_DisableWriteProtection(&WpEnabled);


    // Exchange the service table pointer
	//Print(L"DriverDXERuntime [Functions/Image.c] (HookServiceTablePointer) - Exchange the service table pointer\r\n");
    VOID* OriginalFunction = InterlockedCompareExchangePointer(ServiceTableFunction, *ServiceTableFunction, NewFunction); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseSynchronizationLib/SynchronizationGcc.c


    // Update CRC32 in the service table header
	//Print(L"DriverDXERuntime [Functions/Image.c] (HookServiceTablePointer) - Update CRC32 in the service table header\r\n");
    ServiceTableHeader->CRC32 = 0;
    gBS->CalculateCrc32((UINT8*)ServiceTableHeader, ServiceTableHeader->HeaderSize, &ServiceTableHeader->CRC32); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-calculatecrc32


    // Enable write protection
    //Print(L"DriverDXERuntime [Functions/Image.c] (HookServiceTablePointer) - Enable write protection\r\n");
	FunctionsUtilsRegisters_EnableWriteProtection(WpEnabled);


	// Restore TPL
	//Print(L"DriverDXERuntime [Functions/Image.c] (HookServiceTablePointer) - Restore TPL\r\n");
    gBS->RestoreTPL(Tpl); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-restoretpl


	// Bye
    //Print(L"DriverDXERuntime [Functions/Image.c] (HookServiceTablePointer) - Bye\r\n");
    return OriginalFunction;
}



/**
    Hooks and processes the Boot Service's LoadImage function.

    This function serves as a custom hook for UEFI's LoadImage function. It performs additional processing before calling the original LoadImage function.

    @param[in]      BootPolicy              Indicates the policy for loading the image. If TRUE, the image is loaded as a boot option; otherwise, it is loaded without boot option.
    @param[in]      ParentImageHandle       The handle of the image that is loading this image.
    @param[in]      DevicePath              The pointer to the device path of the image.
    @param[in]      SourceBuffer            If not NULL, a pointer to the memory location containing a copy of the image to be loaded.
    @param[in]      SourceSize              The size in bytes of SourceBuffer.
    @param[out]     ImageHandle             The pointer where the handle of the loaded image will be returned.

    @retval         EFI_SUCCESS             The image is successfully loaded.
    @retval         other                   An error occurred.
**/

EFI_STATUS
EFIAPI
FunctionsImage_HookgBSLoadImage(
	IN      BOOLEAN                         BootPolicy,
	IN      EFI_HANDLE                      ParentImageHandle,
	IN      EFI_DEVICE_PATH_PROTOCOL        *DevicePath,
	IN      VOID                            *SourceBuffer           OPTIONAL,
	IN      UINTN                           SourceSize,
	OUT     EFI_HANDLE                      *ImageHandle
	)
{
    // Set console text output color
    //gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);


	// Hello World
    //Print(L"DriverDXERuntime [Functions/Image.c] (HookgBSLoadImage) - Hello World\r\n");


	// Variables
	EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage = NULL;


    // Call original LoadImage function
	//Print(L"DriverDXERuntime [Functions/Image.c] (HookgBSLoadImage) - Call original LoadImage function\r\n");
    Status = gOriginalgBSLoadImage(BootPolicy, ParentImageHandle, DevicePath, SourceBuffer, SourceSize, ImageHandle);

    // Failed
    if (EFI_ERROR(Status) || *ImageHandle == NULL) {
        //Print(L"DriverDXERuntime [Functions/Image.c] (HookgBSLoadImage) - gOriginalgBSLoadImage failed: %llx (%r).\r\n", Status, Status);
        return Status;
    }


    // Get loaded image info
    //Print(L"DriverDXERuntime [Functions/Image.c] (HookgBSLoadImage) - Get loaded image info\r\n");
    Status = gBS->OpenProtocol(*ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-openprotocol
    LoadedImage->ParentHandle = NULL;

    // Failed
    if (EFI_ERROR(Status))
    {
        //Print(L"DriverDXERuntime [Functions/Image.c] (HookgBSLoadImage) - gBS->OpenProtocol failed: %llx (%r).\r\n", Status, Status);
        return Status;
    }


    // Print image info
    //Print(L"DriverDXERuntime [Functions/Image.c] (HookgBSLoadImage) - Print image info\r\n");
	//Print(L"    -> FilePath = %llx\r\n", LoadedImage->FilePath);
    //Print(L"        -> FilePath = %s\r\n", ConvertDevicePathToText(LoadedImage->FilePath, TRUE, TRUE));
	//Print(L"    -> ImageBase = %llx\r\n", LoadedImage->ImageBase);
	//Print(L"    -> ImageSize = %llx\r\n", LoadedImage->ImageSize);
    //gBS->Stall(10000 * 10);


    // Patch Windows Boot Manager
    //Print(L"DriverDXERuntime [Functions/Image.c] (HookgBSLoadImage) - Patch Windows Boot Manager\r\n");
    Status = FunctionsWindowsBootManager_PatchBootmgfwEfi(LoadedImage->ImageBase, LoadedImage->ImageSize);
    //gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);

    // Failed
    if (EFI_ERROR(Status))
    {
        //Print(L"DriverDXERuntime [Functions/Image.c] (HookgBSLoadImage) - FunctionsWindowsBootManager_PatchBootmgfwEfi failed: %llx (%r).\r\n", Status, Status);
        return Status;
    }


	// Bye
    //Print(L"DriverDXERuntime [Functions/Image.c] (HookgBSLoadImage) - Bye\r\n");
    //gBS->Stall(10000 * 10);
    return EFI_SUCCESS;
}
