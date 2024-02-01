// Structures
#include <Structures/1arc.h>



// ----------------------------------------------------------------------------------------------------------------------------------------------------
// HOOKS
// ----------------------------------------------------------------------------------------------------------------------------------------------------


/**
	Hook Template

	A "faux call" is a technique used in low-level programming and reverse engineering to divert or redirect a program's execution to a custom function or code snippet.
	
	The technique involves inserting machine instructions into a program's execution flow to force it to execute specific code at a particular point, rather than continuing with its normal flow. This is achieved by manipulating existing machine instructions or inserting new instructions that perform custom tasks.
**/

CONST UINT8 gFauxCallHookTemplate[] = {
	0x48, 0xB8,												// mov rax,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			// <addr>
	0x50,													// push [e|r]ax
	0xC3													// ret
}; // https://github.com/Mattiwatti/EfiGuard/blob/master/EfiGuardDxe/PatchBootmgr.c

// Offset
CONST UINTN gFauxCallHookTemplateAddressOffset = 2;



// ----------------------------------------------------------------------------------------------------------------------------------------------------
// ORIGINAL FUNCTIONS
// ----------------------------------------------------------------------------------------------------------------------------------------------------


// gBS->LoadImage
EFI_IMAGE_LOAD gOriginalgBSLoadImage;


// ArchStartBootApplication
typedef
EFI_STATUS
(EFIAPI*
t_ImgArchStartBootApplication)(
	IN PBL_APPLICATION_ENTRY AppEntry,
	IN VOID* ImageBase,
	IN UINT32 ImageSize,
	IN UINT32 BootOption,
	OUT PBL_RETURN_ARGUMENTS ReturnArguments
);																									// Original function
VOID* gOriginalBootmgfwImgArchStartBootApplication = NULL;											// Pointer
UINT8 gBytesBootmgfwEfiImgArchStartBootApplication[sizeof(gFauxCallHookTemplate)] = { 0 };			// Bytes


// OslFwpKernelSetupPhase1
typedef
EFI_STATUS
(EFIAPI*
t_OslFwpKernelSetupPhase1)(
	IN PLOADER_PARAMETER_BLOCK LoaderBlock
);																									// Original function
VOID* gOriginalWinloadOslFwpKernelSetupPhase1 = NULL;												// Pointer
UINT8 gBytesWinloadEfiOslFwpKernelSetupPhase1Backup[sizeof(gFauxCallHookTemplate)] = { 0 };			// Bytes
