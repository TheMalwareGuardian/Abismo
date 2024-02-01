/** @file
    Headers
**/



// Includes
#include <Uefi.h>
#include <Library/UefiLib.h>



// Local
#include <Functions/Utils/3FunctionsUtilsMemory.h>



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
    IN      CONST VOID*     Base,
    IN      UINTN           Size    OPTIONAL
    )
{
    // Hello World
    //Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Hello World\r\n");


    // Checks
    CONST BOOLEAN RangeCheck = Size > 0;
	//Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Checks\r\n");


    // Check if size is less than the size of EFI_IMAGE_DOS_HEADER
    if (RangeCheck && Size < sizeof(EFI_IMAGE_DOS_HEADER))
    {
        //Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Failed: Size is less than the size of EFI_IMAGE_DOS_HEADER\r\n");
        return NULL;
    }

    // Check if e_magic field not match expected DOS signature
    if (((PEFI_IMAGE_DOS_HEADER)Base)->e_magic != EFI_IMAGE_DOS_SIGNATURE)
    {
        //Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Failed: e_magic field not match expected DOS signature\r\n");
        return NULL;
    }

    // Extract e_lfanew field from DOS header
    //Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Extract e_lfanew field from DOS header\r\n");
    CONST UINT32 e_lfanew = ((PEFI_IMAGE_DOS_HEADER)Base)->e_lfanew;

    // Check if e_lfanew field is not within valid bounds
    if (RangeCheck && (e_lfanew >= Size || e_lfanew >= (MAX_UINT32 - sizeof(EFI_IMAGE_NT_SIGNATURE) - sizeof(EFI_IMAGE_FILE_HEADER)) || e_lfanew + sizeof(EFI_IMAGE_NT_SIGNATURE) + sizeof(EFI_IMAGE_FILE_HEADER) >= Size))
    {
        //Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Failed: e_lfanew field is not within valid bounds\r\n");
        return NULL;
    }


    // Get NT headers address
    //Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Get NT headers address\r\n");
    CONST PEFI_IMAGE_NT_HEADERS NtHeaders = (PEFI_IMAGE_NT_HEADERS)(((UINT8*)Base) + e_lfanew);

    // Check if NtHeaders address is not canonical
    if (!FunctionsUtilsMemory_IsAddressCanonical((UINTN)NtHeaders))
    {
        //Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Failed: NtHeaders address is not canonical\r\n");
        return NULL;
    }

    // Check if NT headers signature field not match expected NT signature
    if (NtHeaders->Signature != EFI_IMAGE_NT_SIGNATURE)
    {
        //Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Failed: NT headers signature field not match expected NT signature\r\n");
        return NULL;
    }


    // Bye
	//Print(L"DriverDXERuntime [Functions/Utils/Headers.c] (GetNTHeadersPEFile) - Bye\r\n");
    return NtHeaders;
}
