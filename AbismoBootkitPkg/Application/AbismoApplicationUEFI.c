/** @file
  UEFI Application
**/



// Libraries
#include <Uefi.h>                                 // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiApplicationEntryPoint.h>    // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiApplicationEntryPoint.h
#include <Library/UefiBootServicesTableLib.h>     // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                      // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



// Local
#include <Functions/1FunctionsScreen.h>
#include <Functions/2FunctionsBanner.h>
#include <Functions/3FunctionsBoot.h>



/**
  Entry point for the Application

  @param[in]  ImageHandle       The firmware allocated handle for the EFI image.  
  @param[in]  SystemTable       A pointer to the EFI System Table.

  @retval     EFI_SUCCESS       The entry point is executed successfully.
  @retval     other             Some error occurs when executing this entry point.
**/

EFI_STATUS
EFIAPI
UefiMain (
  IN    EFI_HANDLE          ImageHandle,
  IN    EFI_SYSTEM_TABLE    *SystemTable // https://uefi.org/specs/UEFI/2.10/04_EFI_System_Table.html
  )
{

  // Set the console output to the highest resolution
  //FunctionsScreen_SetConsoleOutputToHighestResolution();


  // Banner
  //FunctionsBanner_AsciiBanner();  

  // Pause
  //gBS->Stall(10000 * 10); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-stall


  // Wiki
  //FunctionsBanner_Wiki();
  //gBS->Stall(10000 * 10);


  // Set console text output color
  //SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTBLUE | EFI_BACKGROUND_BLACK); // https://uefi.org/specs/UEFI/2.10/12_Protocols_Console_Support.html#efi-simple-text-output-protocol-setattribute


  // Hello World
  //Print(L"ApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - UEFI Specification 2.10 - https://uefi.org/specs/UEFI/2.10/\r\n");
  //Print(L"ApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - Hello World\r\n");
  //gBS->Stall(10000 * 10);


  // Variables
  EFI_STATUS Status;
  CHAR16* ABISMO_DXE_RUNTIME_DRIVER_PATH_FILENAME = L"\\EFI\\Boot\\AbismoDriverDXERuntime.efi"; //L"\\EFI\\Microsoft\\Boot\\bootmgfwdd.ef";
  CHAR16* ABISMO_WINDOWS_BOOT_MANAGER_BACKUP_PATH_FILENAME = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi"; //L"\\EFI\\Microsoft\\Boot\\bootmgfwcp.ef";


  // Load DXE Runtime Driver
  //Print(L"ApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - Load DXE Runtime Driver\r\n");
  Status = FunctionsBoot_LoadImage(ABISMO_DXE_RUNTIME_DRIVER_PATH_FILENAME);
  //SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTBLUE | EFI_BACKGROUND_BLACK);

  // Failed
  if (EFI_ERROR(Status))
  {
    //Print(L"ApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - FunctionsBoot_LoadImage failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }

  // Pause
  //gBS->Stall(10000 * 10);


  // Boot the operating system
  //Print(L"ApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - Boot the Operating System\r\n");
  Status = FunctionsBoot_LoadImage(ABISMO_WINDOWS_BOOT_MANAGER_BACKUP_PATH_FILENAME);
  //SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTBLUE | EFI_BACKGROUND_BLACK);

  // Failed
  if (EFI_ERROR(Status))
  {
    //Print(L"ApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - FunctionsBoot_LoadImage failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }

  // Pause
  //gBS->Stall(10000 * 10);


  // Bye
  //Print(L"ApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - Bye\r\n");
  //gBS->Stall(10000 * 10);
  return EFI_SUCCESS;
}
