/** @file
  EFI System Partition Functions
**/



// Libraries
#include <Uefi.h>                                 // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>     // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                      // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
#include <Library/DevicePathLib.h>                // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h
#include <Library/FileHandleLib.h>                // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/FileHandleLib.h



// Protocols
#include <Protocol/LoadedImage.h>                 // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h
#include <Protocol/SimpleFileSystem.h>            // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/SimpleFileSystem.h



/**
  Locates a file within the current EFI System Partition.

  This function searches for a specified file (denoted by ImagePath) in the EFI System Partition where the UEFI application is currently running. If found, the function provides a device path to the located file.

  @param[in]    ImagePath               The path of the file to locate, relative to the root of the EFI System Partition.
  @param[out]   DevicePath              On return, points to the device path of the located file.

  @retval       EFI_SUCCESS             The function successfully located the file and returned its device path.
  @retval       EFI_DEVICE_ERROR        A device related issue occurred.
  @retval       other                   An error occurred while trying to locate the file.
**/

EFI_STATUS
FunctionsUtilsEfiSystemPartition_LocateFileInEfiPartition(
  IN      CHAR16*               ImagePath,
  OUT     EFI_DEVICE_PATH**     DevicePath
  )
{
  // Hello World
  //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - Hello World\r\n");


  // Variables
  EFI_STATUS Status;
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
  EFI_FILE_HANDLE VolumeHandle;
  EFI_FILE_HANDLE FileHandle;


  // Get EFI_LOADED_IMAGE_PROTOCOL
  //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - Get EFI_LOADED_IMAGE_PROTOCOL\r\n");
  Status = gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **) &LoadedImage); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-handleprotocol

  // Failed
  if (EFI_ERROR(Status))
  {
    //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - gBS->HandleProtocol failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }


  // Get EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
  //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - Get EFI_SIMPLE_FILE_SYSTEM_PROTOCOL\r\n");
  Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **) &FileSystem); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-handleprotocol

  // Failed
  if (EFI_ERROR(Status))
  {
    //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - gBS->HandleProtocol failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }


  // Open volume to get root directory
  //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - Open volume to get root directory\r\n");
  Status = FileSystem->OpenVolume(FileSystem, &VolumeHandle); // https://uefi.org/specs/UEFI/2.10/13_Protocols_Media_Access.html#efi-simple-file-system-protocol-openvolume

  // Failed
  if (EFI_ERROR(Status))
  {
    //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - FileSystem->OpenVolume failed: %llx (%r).\r\n", Status, Status);
    return Status;
  }


  // Open file
  //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - Open file\r\n");
  Status = VolumeHandle->Open(VolumeHandle, &FileHandle, ImagePath, EFI_FILE_MODE_READ, 0); // https://uefi.org/specs/UEFI/2.10/13_Protocols_Media_Access.html#efi-file-protocol-open

  // Failed
  if (EFI_ERROR(Status))
  {
    //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - VolumeHandle->Open failed: %llx (%r).\r\n", Status, Status);
    VolumeHandle->Close(VolumeHandle);
    return Status;
  }


  // Create device path to file
  //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - Create device path to file\r\n");
  *DevicePath = FileDevicePath(LoadedImage->DeviceHandle, ImagePath); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/UefiDevicePathLibDevicePathProtocol/UefiDevicePathLib.c

  // Failed
  if (*DevicePath == NULL)
  {
    //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - FileDevicePath failed: %llx (%r).\r\n", Status, Status);
    FileHandle->Close(FileHandle);
    VolumeHandle->Close(VolumeHandle);
    return EFI_DEVICE_ERROR;
  }


  // Close file and volume handles as they are no longer needed
  //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - Close file and volume handles as they are no longer needed\r\n");
  FileHandle->Close(FileHandle);
  VolumeHandle->Close(VolumeHandle); // https://uefi.org/specs/UEFI/2.10/13_Protocols_Media_Access.html#efi-file-protocol-close


  // Bye
  //Print(L"ApplicationUEFI [Functions/Utils/EfiSystemPartition.c] (LocateFileInEfiPartition) - Bye\r\n");
  return EFI_SUCCESS;
}
