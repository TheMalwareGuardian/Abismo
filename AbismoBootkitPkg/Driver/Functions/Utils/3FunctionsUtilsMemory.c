/** @file
	Memory
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
#include <Library/BaseMemoryLib.h>                  // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h



// Local
#include <Functions/Utils/2FunctionsUtilsRegisters.h>



/**
	Copy memory from source to destination.

	This function copies data from the source memory location to the destination memory location, with the specified length.

	@param[out]		Destination		Pointer to the destination memory location.
	@param[in]		Source			Pointer to the source memory location.
	@param[in]		Length			Number of bytes to copy.

	@retval			VOID*			Returns a pointer to the destination memory location after copying.
**/

VOID*
EFIAPI
FunctionsUtilsMemory_CopyMemory(
	OUT		VOID		*Destination,
	IN		CONST		VOID *Source,
	IN		UINTN		Length
	)
{
	// Hello World
  	//Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (CopyMemory) - Hello World\r\n");


	// Variables
	BOOLEAN WpEnabled;


	// Disable write protection
    //Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (CopyMemory) - Disable write protection\r\n");
	FunctionsUtilsRegisters_DisableWriteProtection(&WpEnabled);


	// Copy the content of one buffer to another
    //Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (CopyMemory) - Copy the content of one buffer to another\r\n");
	VOID* Result = CopyMem(Destination, Source, Length); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-copymem
	

	// Enable write protection
    //Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (CopyMemory) - Enable write protection\r\n");
	FunctionsUtilsRegisters_EnableWriteProtection(WpEnabled);


	// Bye
  	//Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (CopyMemory) - Bye\r\n");
	return Result;
}



/**
	Check if the given memory address is canonical.

	This function checks whether the provided memory address is canonical or not. A canonical address is a memory address that complies with the requirements of the x86-64 architecture, considering the current paging mode and certain architectural features.

	@param[in]		Address		Memory address to be checked.

	@retval			TRUE		The address is canonical.
	@retval			FALSE		The address is not canonical.
**/

BOOLEAN
EFIAPI
FunctionsUtilsMemory_IsAddressCanonical(
	IN		UINTN		Address
	)
{
	// Hello World
  	//Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (IsAddressCanonical) - Hello World\r\n");
	

	// Variables
	BOOLEAN isCanonical, isUpperAddressIgnoreEnableEnabled, isTrue;


	// Linear address bits based on paging mode
	//Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (IsAddressCanonical) - Linear address bits based on paging mode\r\n");
	CONST UINTN LinearAddressBits = FunctionsUtilsRegisters_IsFiveLevelPagingEnabled() ? 57 : 48; // https://en.wikipedia.org/wiki/X86-64


	// Calculate masked address
	//Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (IsAddressCanonical) - Calculate masked address\r\n");
	UINTN MaskedAddress = (UINTN)(((INTN)Address >> LinearAddressBits) + 1);


	// Check if canonical
	//Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (IsAddressCanonical) - Check if canonical\r\n");
	isCanonical = (MaskedAddress <= 1);


	// Check EFER UAIE (Upper Address Ignore Enable)
	//Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (IsAddressCanonical) - Check EFER UAIE (Upper Address Ignore Enable)\r\n");
	isUpperAddressIgnoreEnableEnabled = ((AsmReadMsr64(MSR_EFER) & EFER_UAIE) != 0);


	// Is?
    //Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (IsAddressCanonical) - Is?\r\n");
	isTrue = isCanonical || isUpperAddressIgnoreEnableEnabled;


	// Bye
  	//Print(L"DriverDXERuntime [Functions/Utils/Memory.c] (IsAddressCanonical) - Bye\r\n");
	return isTrue;
}
