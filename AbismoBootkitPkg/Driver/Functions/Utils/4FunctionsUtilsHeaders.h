// Structures
#include <Structures/2pe.h>



/**
  Retrieves the NT headers of a PE (Portable Executable) file.

  This function extracts and returns the NT headers structure from a PE file, which contains essential information about the file's format and characteristics.

  @param[in]      Base                        A pointer to the base address of the PE file.
  @param[in]      Size                        (Optional) The size, in bytes, of the PE file.

  @retval         PEFI_IMAGE_NT_HEADERS       A pointer to the EFI_IMAGE_NT_HEADERS structure if found; otherwise, NULL.
**/

PEFI_IMAGE_NT_HEADERS
EFIAPI
FunctionsUtilsHeaders_GetNTHeadersPEFile(
  IN    CONST VOID*     Base,
  IN    UINTN           Size    OPTIONAL
);
