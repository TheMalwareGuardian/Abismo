/** @file
	Driver Signature Enforcement
**/



// Libraries
#include <Uefi.h>                                   // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>       // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiRuntimeServicesTableLib.h>	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/SynchronizationLib.h>             // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/SynchronizationLib.h
#include <Library/UefiLib.h>                        // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
#include <Library/BaseMemoryLib.h>                  // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
#include <Library/MemoryAllocationLib.h>            // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/MemoryAllocationLib.h
#include <Library/DevicePathLib.h>                  // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h
#include <Library/PrintLib.h>                       // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/PrintLib.h



// Local
#include <Functions/Utils/3FunctionsUtilsMemory.h>
#include <Functions/Utils/5FunctionsUtilsPattern.h>
#include <Functions/Utils/8FunctionsUtilsTables.h>
#include <Functions/Utils/10FunctionsUtilsSignatures.h>



// Structures
#include <Structures/2pe.h>



// Zydis
#include <Zydis/Zydis.h>



/**
	Disables the Driver Signature Enforcement protection for the duration of the boot by preventing it from initializing.

	This function takes a PE image represented by its ImageBase, NtHeaders, and PageSection, and disables the Driver Signature Enforcement protection. The Driver Signature Enforcement is a security feature that restricts the loading of unsigned or improperly signed drivers in Windows.

	The actual heart of Code Integrity lies inside a single executable image, called CI.dll (you can find it inside your \Windows\system32 directory). If we take a look at the list of imported symbols, we will most likely see the following names: CiCheckSignedFile, CiFindPageHashesInCatalog, CiFindPageHashesInSignedFile, CiFreePolicyInfo, CiGetPEInformation, CiInitialize, CiVerifyHashInCatalog. The first function within our interest is the initialization routine, CI!CiInitialize. This routine is imported by the NT core (ntoskrnl.exe), and called during system initialization

	@param[in]		ImageBase					The base address of the PE image.
	@param[in]		NtHeaders					A pointer to the PE image's NT headers.
	@param[in]		PageSection					A pointer to the section header of the PE image.
	@param[in]		BuildNumber					The build number of the operating system.

	@retval			EFI_SUCCESS					The Driver Signature Enforcement protection was successfully disabled.
	@retval			EFI_UNSUPPORTED				The function is not supported on the provided PE image.
	@retval			EFI_INVALID_PARAMETER		One or more of the input parameters are invalid.
	@retval			EFI_NOT_FOUND				The required data or structures were not found in the PE image.
**/

EFI_STATUS
EFIAPI
FunctionsProtections_DisableDriverSignatureEnforcement(
	IN		UINT8*							ImageBase,
	IN		PEFI_IMAGE_NT_HEADERS			NtHeaders,
	IN		PEFI_IMAGE_SECTION_HEADER		PageSection,
	IN		UINT16							BuildNumber
	)
{
	// Hello World
	//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - Hello World\r\n");


	// Variables
	CONST UINT32 PageSizeOfRawData = PageSection->SizeOfRawData;
	CONST UINT8* PageStartVa = ImageBase + PageSection->VirtualAddress;


	// Find Import Address Table
	//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - Find Import Address Table\r\n");
	VOID* CiInitialize;
	EFI_STATUS IatStatus = FunctionsUtilsTables_FindImportAddressTable(ImageBase, NtHeaders, "CI.dll", "CiInitialize", &CiInitialize);

	// Failed
	if (EFI_ERROR(IatStatus))
	{
		//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - FunctionsUtilsTables_FindImportAddressTable failed: %llx (%r).\r\n", Status, Status);
		return IatStatus;
	}


	// Zydis variables
	ZyanStatus Status;
	ZydisDecoder Decoder;
	ZydisDecodedInstruction Instruction;
	ZydisDecodedOperand Operands[ZYDIS_MAX_OPERAND_COUNT];
	ZyanU64 InstructionAddress;
	UINTN Length = PageSizeOfRawData;
	UINTN Offset = 0;


	// Zydis Decoder Initialization
	//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - Zydis Decoder Initialization\r\n");
	Status = ZydisDecoderInit(&Decoder, IMAGE64(NtHeaders) ? ZYDIS_MACHINE_MODE_LONG_64 : ZYDIS_MACHINE_MODE_LONG_COMPAT_32, IMAGE64(NtHeaders) ? ZYDIS_STACK_WIDTH_64 : ZYDIS_STACK_WIDTH_32); // https://github.com/zyantific/zydis/blob/15e38ac36fa4a84dde0fc6ae973d33c22e9521e9/src/Decoder.c#L4902

	// Failed
	if (!ZYAN_SUCCESS(Status))
	{
		//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - ZydisDecoderInit failed\r\n");
		return EFI_LOAD_ERROR;
	}


	// Address of SepInitializeCodeIntegrityMovEcx and last MOV instruction into ECX
	UINT8* SepInitializeCodeIntegrityMovEcxAddress = NULL;
	UINT8* LastMovIntoEcx = NULL;


	// Loop to disassemble and process instructions
	//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - Loop to disassemble and process instructions\r\n");
	while ((InstructionAddress = (ZyanU64)(PageStartVa + Offset), Status = ZydisDecoderDecodeFull(&Decoder, (VOID*)InstructionAddress, Length - Offset, &Instruction, Operands)) != ZYDIS_STATUS_NO_MORE_DATA)
	{

		// ZydisDecoderDecodeFull failed
		if (!ZYAN_SUCCESS(Status))
		{
			// Increment Offset and continue to next instruction
			Offset++;
			continue;
		}

		// mov ecx, <something>
		if (Instruction.operand_count == 2 && Instruction.length == 2 && Instruction.mnemonic == ZYDIS_MNEMONIC_MOV && Operands[0].type == ZYDIS_OPERAND_TYPE_REGISTER && Operands[0].reg.value == ZYDIS_REGISTER_ECX)
		{
			// Capturing the address of the instruction in memory.
			// 2-byte
			// 'mov ecx, <data or value being moved into the ECX register by the mov instruction>'
			LastMovIntoEcx = (UINT8*)InstructionAddress;
		}

		// JMP or CALL
		else if (((Instruction.operand_count == 2 || Instruction.operand_count == 4) && (Operands[0].type == ZYDIS_OPERAND_TYPE_MEMORY && Operands[0].mem.base == ZYDIS_REGISTER_RIP) && ((Instruction.mnemonic == ZYDIS_MNEMONIC_JMP && Instruction.operand_count == 2) || (Instruction.mnemonic == ZYDIS_MNEMONIC_CALL && Instruction.operand_count == 4))))
		{
			// Calculate absolute address of target operand and check if it matches the address of CiInitialize
			ZyanU64 OperandAddress = 0;
			if (ZYAN_SUCCESS(ZydisCalcAbsoluteAddress(&Instruction, &Operands[0], InstructionAddress, &OperandAddress)) && OperandAddress == (UINTN)CiInitialize) // https://github.com/zyantific/zydis/blob/15e38ac36fa4a84dde0fc6ae973d33c22e9521e9/src/Utils.c#L41
			{
				// Store the address of the 'mov ecx, <data ...>' instruction related to CiInitialize
				SepInitializeCodeIntegrityMovEcxAddress = LastMovIntoEcx;
				break;
			}
		}

		Offset += Instruction.length;
	}

	// Failed
	if (SepInitializeCodeIntegrityMovEcxAddress == NULL)
	{
		//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - Failed: SepInitializeCodeIntegrityMovEcxAddress null\r\n");
		return EFI_NOT_FOUND;
	}


	// Set ECX to Zero
	//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - Set ECX to Zero\r\n");
    CONST UINT16 ZeroEcx = 0xC931; // xor ecx, ecx (https://defuse.ca/online-x86-assembler.htm)
    FunctionsUtilsMemory_CopyMemory(SepInitializeCodeIntegrityMovEcxAddress, &ZeroEcx, sizeof(ZeroEcx)); // The use of xor ecx, ecx is a common assembly instruction used to set the ECX register to zero. It is used as part of the patching process to achieve the desired functionality.


	// Find SeCodeIntegrityQueryInformation signature
	//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - Find SeCodeIntegrityQueryInformation signature\r\n");
	UINT8* Found = NULL;
	EFI_STATUS CiStatus = FunctionsUtilsPattern_FindPattern(SigSeCodeIntegrityQueryInformation, 0xCC, sizeof(SigSeCodeIntegrityQueryInformation), (VOID*)PageStartVa, PageSizeOfRawData, (VOID**)&Found);

	// Failed
	if (EFI_ERROR(CiStatus))
	{
		//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - FunctionsUtilsPattern_FindPattern failed: Failed to find nt!SeCodeIntegrityQueryInformation signature, %llx (%r).\r\n", Status, Status);
		return CiStatus;
	}


	// Inject patch into located code pattern
	//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - Inject patch into located code pattern\r\n");
	CopyMem(Found, SeCodeIntegrityQueryInformationPatch, sizeof(SeCodeIntegrityQueryInformationPatch));


	// Bye
	//Print(L"DriverDXERuntime [Functions/Protections/DisableDriverSignatureEnforcement.c] (Disable) - Bye\r\n");
	return EFI_SUCCESS;
}
