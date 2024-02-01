/**
	Locates and returns a pointer to a specific directory entry in the data directories of a PE image.

	This function is used to retrieve information from a PE image's data directories by specifying the desired directory entry.

	@param[in]		Base				The base address of the PE image.
	@param[in]		MappedAsImage		Indicates whether the PE file is mapped as an image.
	@param[in]		DirectoryEntry		The index of the directory entry to locate.
	@param[out]		Size				A pointer to store the size of the located directory entry.

	@retval			VOID*				A pointer to the located data entry if found, or NULL if not found or if an error occurs.
**/

VOID*
EFIAPI
FunctionsUtilsPortableExecutable_RtlpImageDirectoryEntryToDataEx(
	IN		CONST VOID*		Base,
	IN		BOOLEAN			MappedAsImage,
	IN		UINT16			DirectoryEntry,
	OUT		UINT32			*Size
);



/**
    Get version information from a Portable Executable (PE) file.

    This function extracts version information from a PE file, such as an EFI application or executable, and returns the major version, minor version, build number, revision, and file flags if available.

    @param[in]		ImageBase			A pointer to the base address of the PE file.
    @param[out]		MajorVersion		A pointer to store the major version (optional).
    @param[out]		MinorVersion		A pointer to store the minor version (optional).
    @param[out]		BuildNumber			A pointer to store the build number (optional).
    @param[out]		Revision			A pointer to store the revision (optional).
    @param[out]		FileFlags			A pointer to store the file flags (optional).

    @retval			EFI_SUCCESS			The version information is successfully retrieved.
    @retval			EFI_NOT_FOUND		The version information is not found in the PE file.
    @retval			other				An error occurred.
**/

EFI_STATUS
EFIAPI
FunctionsUtilsPortableExecutable_GetVersionPEFile(
	IN		CONST VOID*		ImageBase,
	OUT		UINT16*			MajorVersion		OPTIONAL,
	OUT		UINT16*			MinorVersion		OPTIONAL,
	OUT		UINT16*			BuildNumber			OPTIONAL,
	OUT		UINT16*			Revision			OPTIONAL,
	OUT		UINT32*			FileFlags			OPTIONAL
);
