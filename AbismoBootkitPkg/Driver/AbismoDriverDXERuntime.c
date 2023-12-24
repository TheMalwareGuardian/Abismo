/** @file
  DXE Runtime Driver
**/



// Includes
#include <Uefi.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Functions/FunctionsHook.h>



/**
    Entry point for the Driver

    @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
    @param[in] SystemTable    A pointer to the EFI System Table.

    @retval EFI_SUCCESS       The entry point is executed successfully.
    @retval other             Some error occurs when executing this entry point.
**/

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    )
{
    // Set the console text output color
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK); // https://uefi.org/specs/UEFI/2.10/12_Protocols_Console_Support.html#efi-simple-text-output-protocol-setattribute


    // Hello World
    Print(L"AbismoDriverDXERuntime [AbismoDriverDXERuntime.c] (UefiMain) - Hello World\r\n");


    // Hook gBS->LoadImage
    Print(L"AbismoDriverDXERuntime [AbismoDriverDXERuntime.c] (UefiMain) - Hook gBS->LoadImage\r\n");
    OriginalgBSLoadImage = (EFI_IMAGE_LOAD)FunctionsHook_HookServiceTablePointer(&gBS->Hdr, (VOID**)&gBS->LoadImage, (VOID*)&FunctionsHook_HookedgBSLoadImage);
    Print(L"AbismoDriverDXERuntime [AbismoDriverDXERuntime.c] (UefiMain) - Hooked gBS->LoadImage: 0x%p -> 0x%p\r\n", (VOID*)OriginalgBSLoadImage, (VOID*)&FunctionsHook_HookedgBSLoadImage);
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);


    // Bye
    Print(L"AbismoDriverDXERuntime [AbismoDriverDXERuntime.c] (UefiMain) - Bye\r\n");
    return EFI_SUCCESS;
}
