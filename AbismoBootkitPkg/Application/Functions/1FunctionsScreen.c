/** @file
    Screen
**/



// Libraries
#include <Uefi.h>                                 // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
#include <Library/UefiBootServicesTableLib.h>     // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
#include <Library/UefiLib.h>                      // https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h



/**
    Set the console output to the highest resolution.

    This function iterates through all available text modes supported by the console output device, and sets the console to the mode with the highest resolution (i.e., the mode with the most rows and columns). It is useful for maximizing the amount of information that can be displayed on the console.

    @retval None
**/

VOID
FunctionsScreen_SetConsoleOutputToHighestResolution(
    )
{
    // Variables
    UINTN MaxMode = gST->ConOut->Mode->MaxMode;
    UINTN MaxRows = 0;
    UINTN MaxColumns = 0;
    UINTN BestMode = 0;


    // Iterate through all available text modes
    for (UINTN Mode = 0; Mode < MaxMode; Mode++) {
        UINTN Columns, Rows;

        // Query the console for the number of rows and columns in the current mode
        EFI_STATUS Status = gST->ConOut->QueryMode(gST->ConOut, Mode, &Columns, &Rows); // https://uefi.org/specs/UEFI/2.10/12_Protocols_Console_Support.html#efi-simple-text-output-protocol-querymode
        if (!EFI_ERROR(Status)) {

            // If the current mode has a greater area (rows * columns) than the previous, update the best mode
            if ((Rows * Columns) > (MaxRows * MaxColumns)) {
                MaxRows = Rows;
                MaxColumns = Columns;
                BestMode = Mode;
            }
        }
    }


    // Set the console to the best mode found
    gST->ConOut->SetMode(gST->ConOut, BestMode); // https://uefi.org/specs/UEFI/2.10/12_Protocols_Console_Support.html#efi-simple-text-output-protocol-setmode
}
