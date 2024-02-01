/**
  Patch to Windows Boot Manager (bootmgfw.efi) image.

  This function applies a custom patch to modify the behavior of the Windows Boot Manager.

  @param[in]      ImageBase           A pointer to the base address of the loaded image.
  @param[in]      ImageSize           The size, in bytes, of the loaded image.

  @retval         EFI_SUCCESS         The patch was successfully applied.
  @retval         EFI_NOT_FOUND       The necesarry functions were not found.
  @retval         other               An error occurred during the patching process.
**/

EFI_STATUS
EFIAPI
FunctionsWindowsBootManager_PatchBootmgfwEfi(
  IN      VOID*       ImageBase,
  IN      UINTN       ImageSize
);
