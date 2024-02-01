/**
	Hooks the ArchStartBootApplication function.

	This function replaces the original ArchStartBootApplication function with a custom hook that allows for additional functionality.

	@param[in]		AppEntry					A pointer to the BL_APPLICATION_ENTRY structure.
	@param[in]		ImageBase					A pointer to the base address of the loaded image.
	@param[in]		ImageSize					The size, in bytes, of the loaded image.
	@param[in]		BootOption					The boot option identifier.
	@param[out]		ReturnArguments				A pointer to the BL_RETURN_ARGUMENTS structure.
	x@param[in]		OriginalFunction			A pointer to the original ArchStartBootApplication function.
	x@param[in]		OriginalFunctionBytes		A pointer to the original bytes of the ArchStartBootApplication function.

	@retval			EFI_SUCCESS					The ArchStartBootApplication function was successfully hooked.
	@retval			other						An error occurred during the hooking process.
**/

EFI_STATUS
EFIAPI
FunctionsHooksBootmgfwEfi_ArchStartBootApplication(
	IN		PBL_APPLICATION_ENTRY		AppEntry,
	IN		VOID*						ImageBase,
	IN		UINT32						ImageSize,
	IN		UINT32						BootOption,
	OUT		PBL_RETURN_ARGUMENTS		ReturnArguments
);
