/** @file
	Windows Kernel
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>       // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
#include <Library/BaseMemoryLib.h>                  // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h



// Local
#include <Functions/Utils/7FunctionsUtilsPortableExecutable.h>
#include <Functions/Protections/1FunctionsProtectionsDriverSignatureEnforcement.h>



// Structures
#include <Structures/2pe.h>



/**
	Patch to Windows Kernel (ntoskrnl.exe) image.

  	This function applies a custom patch to modify the behavior of the Windows Kernel.

	@param[in]		ImageBase			A pointer to the base address of the loaded image.
	@param[in]		NtHeaders			A pointer to the NT headers of the loaded image.

	@retval			EFI_SUCCESS			The patch was successfully applied.
	@retval			other				An error occurs during the patching process.
**/

EFI_STATUS
EFIAPI
FunctionsWindowsKernel_PatchNtoskrnlExe(
	IN		VOID*						ImageBase,
	IN		PEFI_IMAGE_NT_HEADERS		NtHeaders
	)
{
	// Set console text output color
    //gST->ConOut->SetAttribute(gST->ConOut, EFI_MAGENTA | EFI_BACKGROUND_BLACK);


	// Variables
	EFI_STATUS Status;
	UINT16 MajorVersion = 0, MinorVersion = 0, BuildNumber = 0, Revision = 0;
	UINT32 FileFlags = 0;


	// Hello World
	//Print(L"DriverDXERuntime [Functions/WindowsKernel.c] (PatchNtoskrnlExe) - Hello World\r\n");


	// Get version info
	//Print(L"DriverDXERuntime [Functions/WindowsKernel.c] (PatchNtoskrnlExe) - Get version info\r\n");
	Status = FunctionsUtilsPortableExecutable_GetVersionPEFile(ImageBase, &MajorVersion, &MinorVersion, &BuildNumber, &Revision, &FileFlags);

	// Failed
	if (EFI_ERROR(Status))
	{
		//Print(L"DriverDXERuntime [Functions/WindowsKernel.c] (PatchNtoskrnlExe) - FunctionsUtilsPortableExecutable_GetVersionPEFile failed: %llx (%r).\r\n", Status, Status);
		return EFI_NOT_FOUND;
	}


	// Print info
	//Print(L"DriverDXERuntime [Functions/WindowsKernel.c] (PatchNtoskrnlExe) - Print info\r\n");
	//Print(L"DriverDXERuntime [Functions/WindowsKernel.c] (PatchNtoskrnlExe) - Ntoskrnl.exe v%u.%u.%u.%u...\r\n", MajorVersion, MinorVersion, BuildNumber, Revision);


	// Find INIT, .text and PAGE sections
	//Print(L"DriverDXERuntime [Functions/WindowsKernel.c] (PatchNtoskrnlExe) - Find INIT, .text and PAGE sections\r\n");
	PEFI_IMAGE_SECTION_HEADER InitSection = NULL, TextSection = NULL, PageSection = NULL;
	PEFI_IMAGE_SECTION_HEADER Section = IMAGE_FIRST_SECTION(NtHeaders);
	for (UINT16 i = 0; i < NtHeaders->FileHeader.NumberOfSections; ++i)
	{
		CHAR8 SectionName[EFI_IMAGE_SIZEOF_SHORT_NAME + 1];
		CopyMem(SectionName, Section->Name, EFI_IMAGE_SIZEOF_SHORT_NAME);
		SectionName[EFI_IMAGE_SIZEOF_SHORT_NAME] = '\0';

		if (AsciiStrCmp(SectionName, "INIT") == 0)
			InitSection = Section;
		else if (AsciiStrCmp(SectionName, ".text") == 0)
			TextSection = Section;
		else if (AsciiStrCmp(SectionName, "PAGE") == 0)
			PageSection = Section;

		Section++;
	}


	// Disable Driver Signature Enforcement
	//Print(L"DriverDXERuntime [Functions/WindowsKernel.c] (PatchNtoskrnlExe) - Disable Driver Signature Enforcement\r\n");
    Status = FunctionsProtections_DisableDriverSignatureEnforcement(ImageBase, NtHeaders, PageSection, BuildNumber);

	// Failed
	if (EFI_ERROR(Status))
	{
		//Print(L"DriverDXERuntime [Functions/WindowsKernel.c] (PatchNtoskrnlExe) - FunctionsProtections_DisableDriverSignatureEnforcement failed: %llx (%r).\r\n", Status, Status);
		return Status;
	}


	return EFI_SUCCESS;
}
