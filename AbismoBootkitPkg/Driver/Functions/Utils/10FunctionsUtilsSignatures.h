/**
	Signatures
**/



// ----------------------------------------------------------------------------------------------------------------------------------------------------
// BOOTMGFW.EFI
// ----------------------------------------------------------------------------------------------------------------------------------------------------


/**
	Signature for [bootmgfw]ImgArchStartBootApplication

	ImgArchStartBootApplication between windows versions 2004-1709 is invoked to start winload.efi. We can use ImgArchStartBootApplication to detect the point at which the Windows OS loader (winload.efi) is loaded into memory but has not yet been executed.
**/

STATIC CONST UINT8 SigImgArchStartBootApplication[] = {
	0x41, 0xB8, 0x09, 0x00, 0x00, 0xD0							// mov r8d, 0D0000009h
}; // https://guidedhacking.com/threads/hyper-v-hacking-framework-works-on-every-version-of-windows-10-2004-1511-amd-intel.16251/



// ----------------------------------------------------------------------------------------------------------------------------------------------------
// WINLOAD.EFI
// ----------------------------------------------------------------------------------------------------------------------------------------------------


/**
	Signature for [winload]OslFwpKernelSetupPhase1

	The Windows OS Loader has two primary responsibilities. First, it loads the operating system into memory, and second, it manages the transition from the Bootloader to the operating system. The second task is handled by a function called OslExecuteTransition, particularly within the sub-function known as OslArchTransferToKernel.

	If the OS loading process is successful, the UEFI Loader gains the ability to invoke the ExitBootService function. This function releases all EFI Drivers categorized as EFI_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER. If the result of this call is EFI_SUCCESS, it means that the UEFI Loader has taken control of all available system memory and is now responsible for the ongoing system execution.

	EFI Drivers classified as EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER remain in memory and can be used effectively with paging and virtual addresses, as long as the service has accurately defined the virtual memory space it intends to utilize by invoking the SetVirtualAddressMap function.

	Winload.efi is as a UEFI Loader, so logically it has to take charge of everything. Within the OslExecuteTransition function, the first called sub-function is OslFwpKernelSetupPhase1, which is responsible for carrying out these tasks and also handles the mapping of physical to virtual addresses by invoking the SetVirtualAddressMap function.
**/

STATIC CONST UINT8 SigOslFwpKernelSetupPhase1[] = {
	0x89, 0xCC, 0x24, 0x01, 0x00, 0x00,							// mov [REG+124h], r32
	0xE8, 0xCC, 0xCC, 0xCC, 0xCC,								// call BlBdStop
	0xCC, 0x8B, 0xCC											// mov r32, r/m32
}; // https://n4r1b.com/posts/2019/09/transition-from-an-efi-bootloader-to-the-kernel-in-windows-x64/



// ----------------------------------------------------------------------------------------------------------------------------------------------------
// NTOSKRNL.EXE
// ----------------------------------------------------------------------------------------------------------------------------------------------------


/**
	Signature for [nt]SeCodeIntegrityQueryInformation

	SeCodeIntegrityQueryInformation function, called through NtQuerySystemInformation -> SystemCodeIntegrityInformation (https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntquerysysteminformation) is a crucial part of the Code Integrity feature in the Windows operating system.
	
	Code Integrity enhances system security by verifying the integrity of drivers and system files whenever they are loaded into memory. On x64-based versions of the operating system, kernel-mode drivers must be digitally signed.
**/

STATIC CONST UINT8 SigSeCodeIntegrityQueryInformation[] = {
	0x48, 0x83, 0xEC,											// sub rsp, XX
	0xCC, 0x48, 0x83, 0x3D, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,		// cmp ds:qword_xxxx, 0
	0x4D, 0x8B, 0xC8,											// mov r9, r8
	0x4C, 0x8B, 0xD1,											// mov r10, rcx
	0x74, 0xCC													// jz XX
}; // https://j00ru.vexillium.org/2010/06/insight-into-the-driver-signature-enforcement/


/**
	Patch SeCodeIntegrityQueryInformation to report Disable Signature Enforcement (DSE) is disabled
**/

STATIC CONST UINT8 SeCodeIntegrityQueryInformationPatch[] = {
	0x41, 0xC7, 0x00, 0x08, 0x00, 0x00, 0x00,					// mov dword ptr [r8], 8
	0x33, 0xC0,													// xor eax, eax
	0xC7, 0x41, 0x04, 0x01, 0x00, 0x00, 0x00,					// mov dword ptr [rcx+4], 1
	0xC3														// ret
}; // https://github.com/Mattiwatti/EfiGuard/blob/master/EfiGuardDxe/PatchNtoskrnl.c
