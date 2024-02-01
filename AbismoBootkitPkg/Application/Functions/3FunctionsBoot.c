/** @file
    Boot
**/



// Libraries
#include <Uefi.h>                                 // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>     // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                      // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



// Local
#include <Functions/Utils/1FunctionsUtilsEfiSystemPartition.h>



/**
    Locates, loads and starts an image from a specified file path.

    This function locates, loads, and starts an image using the provided path. It first locates the file in the EFI System Partition, then loads it into memory, and finally starts the image.

    @param[in]  ImagePath           The path of the image to load, specified as a wide string.

    @retval     EFI_SUCCESS         The image was loaded and started successfully.
    @retval     other               An error occurred during one of the steps (locating, loading, or starting).
**/

EFI_STATUS
FunctionsBoot_LoadImage(
    IN      CHAR16*     ImagePath
    )
{
    // Hello World
    //Print(L"ApplicationUEFI [Functions/Boot.c] (LoadImage) - Hello World\r\n");


    // Variables
    EFI_STATUS Status;
    EFI_DEVICE_PATH *DevicePath = NULL;
    EFI_HANDLE ImageHandle = NULL;


    // Locate file in EFI partition
    //Print(L"ApplicationUEFI [Functions/Boot.c] (LoadImage) - Locate file in EFI partition (%S)\r\n", ImagePath);
    Status = FunctionsUtilsEfiSystemPartition_LocateFileInEfiPartition(ImagePath, &DevicePath);

    // Failed
    if (EFI_ERROR(Status))
    {
        //Print(L"ApplicationUEFI [Functions/Boot.c] (LoadImage) - FunctionsEfiSystemPartition_LocateFileInEfiPartition failed: %llx (%r).\r\n", Status, Status);
        return Status;
    }


    // Load image into memory
    //Print(L"ApplicationUEFI [Functions/Boot.c] (LoadImage) - Load image into memory\r\n");
    Status = gBS->LoadImage(FALSE, gImageHandle, DevicePath, NULL, 0, &ImageHandle); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-loadimage

    // Failed
    if (EFI_ERROR(Status))
    {
        //Print(L"ApplicationUEFI [Functions/Boot.c] (LoadImage) - gBS->LoadImage failed: %llx (%r).\r\n", Status, Status);
        return Status;
    }


    // Start loaded image
    //Print(L"ApplicationUEFI [Functions/Boot.c] (LoadImage) - Start loaded image\r\n");
    Status = gBS->StartImage(ImageHandle, NULL, NULL); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-startimage

    // Failed
    if (EFI_ERROR(Status))
    {
        //Print(L"ApplicationUEFI [Functions/Boot.c] (LoadImage) - gBS->StartImage failed: %llx (%r).\r\n", Status, Status);
        return Status;
    }


    // Bye
    //Print(L"ApplicationUEFI [Functions/Boot.c] (LoadImage) - Bye\r\n");
    return EFI_SUCCESS;
}
