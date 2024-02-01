/** @file
    DXE Runtime Driver
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiDriverEntryPoint.h>           // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiDriverEntryPoint.h
#include <Library/UefiBootServicesTableLib.h>       // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



// Local
#include <Functions/1FunctionsPatchHookImage.h>
#include <Functions/Utils/9FunctionsUtilsHooks.h>



/**
    Entry point for the Driver.

    @param[in]  ImageHandle         The firmware allocated handle for the EFI image.  
    @param[in]  SystemTable         A pointer to the EFI System Table.

    @retval     EFI_SUCCESS         The entry point is executed successfully.
    @retval     other               Some error occurs when executing this entry point.
**/

EFI_STATUS
EFIAPI
UefiMain(
    IN      EFI_HANDLE          ImageHandle,
    IN      EFI_SYSTEM_TABLE    *SystemTable // https://uefi.org/specs/UEFI/2.10/04_EFI_System_Table.html
    )
{
    // Set console text output color
    //SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK); // https://uefi.org/specs/UEFI/2.10/12_Protocols_Console_Support.html#efi-simple-text-output-protocol-setattribute


    // Hello World
    //Print(L"DriverDXERuntime [AbismoDriverDXERuntime.c] (UefiMain) - Hello World\r\n");


    // Hook gBS->LoadImage
    //Print(L"DriverDXERuntime [AbismoDriverDXERuntime.c] (UefiMain) - Hook gBS->LoadImage\r\n");
    gOriginalgBSLoadImage = (EFI_IMAGE_LOAD)FunctionsImage_HookServiceTablePointer(&gBS->Hdr, (VOID**)&gBS->LoadImage, (VOID*)&FunctionsImage_HookgBSLoadImage);
    //SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);


    // Hooked gBS->LoadImage
    //Print(L"DriverDXERuntime [AbismoDriverDXERuntime.c] (UefiMain) - Hooked gBS->LoadImage: 0x%p -> 0x%p\r\n", (VOID*)gOriginalgBSLoadImage, (VOID*)&FunctionsImage_HookgBSLoadImage);
    //gBS->Stall(10000 * 10);


    // Bye
    //Print(L"DriverDXERuntime [AbismoDriverDXERuntime.c] (UefiMain) - Bye\r\n");
    //gBS->Stall(10000 * 10);
    return EFI_SUCCESS;
}
