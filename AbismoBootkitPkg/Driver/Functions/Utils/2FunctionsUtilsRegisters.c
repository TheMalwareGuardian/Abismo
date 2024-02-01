/** @file
	Registers
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



// Local
#include <Functions/Utils/2FunctionsUtilsRegisters.h>



/**
    Disable WP if supported and retrieve their current state.

    This function disables WP (Write Protection) if it is supported by the system's hardware. It also retrieves the current state of this feature and stores it in the output parameter.

    @param[out]		WpEnabled		A pointer to a boolean variable to store the current write protection state. True if write protection is currently enabled, False otherwise.

    @retval			None
**/

VOID
EFIAPI
FunctionsUtilsRegisters_DisableWriteProtection(
	OUT		BOOLEAN		*WpEnabled
	)
{
	// Hello World
  	//Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (DisableWriteProtection) - Hello World\r\n");


	// Variables
	CONST UINTN Cr0 = AsmReadCr0(); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr0.c
	*WpEnabled = (Cr0 & CR0_WP) != 0;


	// Disable Write Protection
	if (*WpEnabled)
	{
		//Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (DisableWriteProtection) - Disable write protection\r\n");
		AsmWriteCr0(Cr0 & ~CR0_WP); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/WriteCr0.c
	}


	// Bye
    //Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (DisableWriteProtection) - Bye\r\n");
}



/**
    Enable WP if supported.

    This function enables WP (Write Protection) if it is supported by the system's hardware. It takes one boolean parameter to control the enabling/disabling of this feature.

    @param[in]		WpEnabled		A boolean indicating whether write protection should be enabled.

    @retval			None
**/

VOID
EFIAPI
FunctionsUtilsRegisters_EnableWriteProtection(
	IN		BOOLEAN		WpEnabled
	)
{
	// Hello World
  	//Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (EnableWriteProtection) - Hello World\r\n");


	// Enable write protection
	if (WpEnabled)
	{
		//Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (EnableWriteProtection) - Enable write protection\r\n");
		AsmWriteCr0(AsmReadCr0() | CR0_WP); // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/WriteCr0.c
	}


	// Bye
    //Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (EnableWriteProtection) - Bye\r\n");
}



/**
    Check if the system is configured for five-level paging.

    This function checks if the x86-64 system is configured for five-level paging, which is a specific paging mode used in modern x86-64 systems. It determines this by examining certain control registers and processor features.

    @retval		TRUE		Five-level paging is enabled.
    @retval		FALSE		Five-level paging is not enabled.
**/

BOOLEAN
EFIAPI
FunctionsUtilsRegisters_IsFiveLevelPagingEnabled()
{
    // Hello World
    //Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (IsFiveLevelPagingEnabled) - Hello World\r\n");


	// Variables
	BOOLEAN isPagingEnabled, isLongModeActiveEnabled, is57BitLinearAddressessEnabled, isTrue;


    // Check CR0 PG (Paging)
	//Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (IsFiveLevelPagingEnabled) - Check CR0 PG (Paging)\r\n");
    isPagingEnabled = (AsmReadCr0() & CR0_PG) != 0; // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr0.c


    // Check EFER LMA (Long Mode Active)
	//Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (IsFiveLevelPagingEnabled) - Check EFER LMA (Long Mode Active)\r\n");
    isLongModeActiveEnabled = (AsmReadMsr64(MSR_EFER) & EFER_LMA) != 0; // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadMsr64.c


    // Check CR4 LA57 (57-Bit Linear Addresses)
	//Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (IsFiveLevelPagingEnabled) - Check CR4 LA57 (57-Bit Linear Addresses)\r\n");
    is57BitLinearAddressessEnabled = (AsmReadCr4() & CR4_LA57) != 0; // https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr4.c


	// Is?
    //Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (IsFiveLevelPagingEnabled) - Is?\r\n");
	isTrue = isPagingEnabled && isLongModeActiveEnabled && is57BitLinearAddressessEnabled;

    // Bye
    //Print(L"DriverDXERuntime [Functions/Utils/Registers.c] (IsFiveLevelPagingEnabled) - Bye\r\n");
    return isTrue;
}
