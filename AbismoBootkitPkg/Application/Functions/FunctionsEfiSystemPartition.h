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
);
