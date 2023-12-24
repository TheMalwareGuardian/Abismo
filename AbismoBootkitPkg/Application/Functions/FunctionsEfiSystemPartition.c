/** @file
  EFI System Partition Functions
**/



// Includes
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/FileHandleLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>



/**
  Locates a file within the current EFI System Partition.

  This function searches for a specified file (denoted by ImagePath) in the EFI System Partition where the UEFI application is currently running. If found, the function provides a device path to the located file.

  @param[in] ImagePath      The path of the file to locate, relative to the root of the EFI System Partition.
  @param[out] DevicePath    On return, points to the device path of the located file.

  @retval EFI_SUCCESS       The function successfully located the file and returned its device path.
  @retval EFI_NOT_FOUND     The specified file was not found in the EFI System Partition.
  @retval other             An error occurred while trying to locate the file.
**/

EFI_STATUS
FunctionsEfiSystemPartition_LocateFileInEfiPartition(
  IN CHAR16* ImagePath,
  OUT EFI_DEVICE_PATH** DevicePath
  )
{
  // Hello World
  Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - Hello World\r\n");


  // Variables
  EFI_STATUS Status;
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
  EFI_FILE_HANDLE VolumeHandle;
  EFI_FILE_HANDLE FileHandle;


  // Get EFI_LOADED_IMAGE_PROTOCOL
  Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - Get EFI_LOADED_IMAGE_PROTOCOL\r\n");
  Status = gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **) &LoadedImage); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-handleprotocol

  // Failed
  if (EFI_ERROR(Status))
  {
    Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - gBS->HandleProtocol failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }


  // Get EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
  Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - Get EFI_SIMPLE_FILE_SYSTEM_PROTOCOL\r\n");
  Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **) &FileSystem); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-handleprotocol

  // Failed
  if (EFI_ERROR(Status))
  {
    Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - gBS->HandleProtocol failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }


  // Open the volume to get the root directory
  Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - Open the volume to get the root directory\r\n");
  Status = FileSystem->OpenVolume(FileSystem, &VolumeHandle); // https://uefi.org/specs/UEFI/2.10/13_Protocols_Media_Access.html#efi-simple-file-system-protocol-openvolume

  // Failed
  if (EFI_ERROR(Status))
  {
    Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - FileSystem->OpenVolume failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }


  // Open the file
  Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - Open the file\r\n");
  Status = VolumeHandle->Open(VolumeHandle, &FileHandle, ImagePath, EFI_FILE_MODE_READ, 0); // https://uefi.org/specs/UEFI/2.10/13_Protocols_Media_Access.html#efi-file-protocol-open

  // Failed
  if (EFI_ERROR(Status))
  {
    Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - VolumeHandle->Open failed: %llx (%r).\r\n", Status, Status);
    VolumeHandle->Close(VolumeHandle);
    return Status;
  }


  // Create a device path to the file
  Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - Create a device path to the file\r\n");
  *DevicePath = FileDevicePath(LoadedImage->DeviceHandle, ImagePath); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiDevicePathLibDevicePathProtocol/UefiDevicePathLib.c

  // Failed
  if (*DevicePath == NULL)
  {
    Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - FileDevicePath failed: %llx (%r).\r\n", Status, Status);
    FileHandle->Close(FileHandle);
    VolumeHandle->Close(VolumeHandle);
    return EFI_OUT_OF_RESOURCES;
  }


  // Close the file and volume handles as they are no longer needed
  FileHandle->Close(FileHandle);
  VolumeHandle->Close(VolumeHandle);


  // Bye
  Print(L"AbismoApplicationUEFI [FunctionsEfiSystemPartition.c] (LocateFileInEfiPartition) - Bye\r\n");
  return EFI_SUCCESS;
}
