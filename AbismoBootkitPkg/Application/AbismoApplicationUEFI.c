/** @file
  UEFI Application
**/



// Includes
#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Functions/FunctionsScreen.h>
#include <Functions/FunctionsBoot.h>



/**
  Entry point for the Application

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table. https://uefi.org/specs/UEFI/2.10/04_EFI_System_Table.html

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.
**/

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  // Set the console output to the highest resolution
  FunctionsScreen_SetConsoleOutputToHighestResolution(SystemTable->ConOut);

  // Set console output color
  SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTBLUE | EFI_BACKGROUND_BLACK); // https://uefi.org/specs/UEFI/2.10/12_Protocols_Console_Support.html#efi-simple-text-output-protocol-setattribute


  // Hello World
  Print(L"AbismoApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - UEFI Specification 2.10 - https://uefi.org/specs/UEFI/2.10/\r\n");
  Print(L"AbismoApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - Hello World\r\n");


  // Variables
  EFI_STATUS Status;
  CHAR16* ABISMO_DXE_RUNTIME_DRIVER_PATH_FILENAME = L"\\EFI\\Microsoft\\Boot\\bootmgfwdd.efi";
  CHAR16* ABISMO_WINDOWS_BOOT_MANAGER_BACKUP_PATH_FILENAME = L"\\EFI\\Microsoft\\Boot\\bootmgfwcp.efi";


  // Load DXE Runtime Driver
  Print(L"AbismoApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - Load DXE Runtime Driver\r\n");
  Status = FunctionsBoot_LoadImage(ABISMO_DXE_RUNTIME_DRIVER_PATH_FILENAME);
  SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTBLUE | EFI_BACKGROUND_BLACK);

  // Failed
  if (EFI_ERROR(Status))
  {
    Print(L"AbismoApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - FunctionsBoot_LoadImage failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }

  // Pause
  gBS->Stall(10000 * 1000);


  // Boot the operating system
  Print(L"AbismoApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - Boot the Operating System\r\n");
  Status = FunctionsBoot_LoadImage(ABISMO_WINDOWS_BOOT_MANAGER_BACKUP_PATH_FILENAME);
  SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_LIGHTBLUE | EFI_BACKGROUND_BLACK);

  // Failed
  if (EFI_ERROR(Status))
  {
    Print(L"AbismoApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - FunctionsBoot_LoadImage failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }


  // Pause
  gBS->Stall(10000 * 1000);

  // Bye
  Print(L"AbismoApplicationUEFI [AbismoApplicationUEFI.c] (UefiMain) - Bye\r\n");
  return EFI_SUCCESS;
}
