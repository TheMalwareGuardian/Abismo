/**
	Hooks the OslFwpKernelSetupPhase1 function.

	This function replaces the original OslFwpKernelSetupPhase1 function with a custom hook that allows for additional functionality.

	@param[in]		LoaderBlock			A pointer to the Loader Parameter Block.

	@retval			EFI_SUCCESS			The OslFwpKernelSetupPhase1 function was successfully hooked.
	@retval			other				An error occurred during the hooking process.
**/

EFI_STATUS
EFIAPI
FunctionsHooksWinloadEfi_OslFwpKernelSetupPhase1(
	IN		PLOADER_PARAMETER_BLOCK		LoaderBlock
);
