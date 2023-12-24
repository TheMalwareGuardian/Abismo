/**
  Set the console output to the highest resolution.

  This function iterates through all available text modes supported by the console output device, and sets the console to the mode with the highest resolution (i.e., the mode with the most rows and columns). It is useful for maximizing the amount of information that can be displayed on the console.

  @param[in] ConOut    A pointer to the EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL instance.
**/

VOID
FunctionsScreen_SetConsoleOutputToHighestResolution(
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut
);
