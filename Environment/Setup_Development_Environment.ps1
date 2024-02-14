
function Show-Menu {
    Clear-Host
    Write-Host "=== Options ==="
    Write-Host "1. Requirements -> Visual Studio 2019 Community + Git + Python + NASM + ASL"
    Write-Host "2. Set Up Environment -> EDK2"
    Write-Host "Q. Exit"
    Write-Host "============"
}



function OptionRequirements {

    Write-Host "You have selected the option 'Requirements -> Visual Studio 2019 Community + Git + Python + NASM + ASL'" -ForegroundColor Green
	$response = Read-Host "Do you want to proceed? (Press 'Y')"
	if ($response -eq "Y") {

		$folderEnvironmentBinaries = "BinariesToInstallEnvironmentBootkits"
		$folderEnvironmentBinariesPath = Join-Path -Path $PWD -ChildPath $folderEnvironmentBinaries
		New-Item -ItemType Directory -Path $folderEnvironmentBinariesPath | Out-Null

		$install = Read-Host "Do you want to install Visual Studio? (Y/N)"
		if ($install -eq "Y") {
			Write-Host "Installing Visual Studio 2019 Community" -ForegroundColor Magenta
			Write-Host "The Visual Studio installer can be found at the following link 'https://learn.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads'" -ForegroundColor Yellow
			Write-Host "1. Under the 'Workloads' section -> 'Desktop & Mobile', select 'Desktop development with C++'" -ForegroundColor Yellow
			Write-Host "2. Look for the 'Individual components' section located as the second option in the top left and select 'MSVC v142 - VS 2019 C++ x64/x86 Spectre-mitigated libs (latest)'" -ForegroundColor Yellow
			Write-Host "3. Install Visual Studio" -ForegroundColor Yellow

			Invoke-WebRequest -Uri "https://download.visualstudio.microsoft.com/download/pr/7c09e2e8-2b3e-4213-93ab-5646874f8a2b/0ac797413a56c6b2772f48a567a32cdddd3b739f5b2af649fcf90be4245762ff/vs_Community.exe" -OutFile $folderEnvironmentBinariesPath\vs_Community.exe
			$process = Start-Process -FilePath $folderEnvironmentBinariesPath\vs_Community.exe -PassThru
			$process.WaitForExit()
			while ($true) {
				$response = Read-Host "Has the Visual Studio installation completed? (Y/N)"
				if ($response -eq "Y") {
					break
				}
			}
		}

		$install = Read-Host "Do you want to install Git? (Y/N)"
		if ($install -eq "Y") {
			Write-Host "Installing Git" -ForegroundColor Magenta

			Invoke-WebRequest -Uri "https://github.com/git-for-windows/git/releases/download/v2.43.0.windows.1/Git-2.43.0-64-bit.exe" -OutFile $folderEnvironmentBinariesPath\git.exe
			$process = Start-Process -FilePath $folderEnvironmentBinariesPath\git.exe -PassThru
			$process.WaitForExit()
			while ($true) {
				$response = Read-Host "Has the Git installation completed? (Y/N)"
				if ($response -eq "Y") {
					break
				}
			}
		}

		$install = Read-Host "Do you want to install Python 3.9? (Y/N)"
		if ($install -eq "Y") {
			Write-Host "Installing Python 3.9" -ForegroundColor Magenta
			Write-Host "Python 3.9 installer can be found at the following link 'https://www.python.org/downloads/release/python-390/'" -ForegroundColor Yellow

			Invoke-WebRequest -Uri "https://www.python.org/ftp/python/3.9.0/python-3.9.0-amd64.exe" -OutFile $folderEnvironmentBinariesPath\python39.exe
			$process = Start-Process -FilePath $folderEnvironmentBinariesPath\python39.exe -PassThru
			$process.WaitForExit()
			while ($true) {
				$response = Read-Host "Has the Python 3.9 installation completed? (Y/N)"
				if ($response -eq "Y") {
					break
				}
			}
		}

		$install = Read-Host "Do you want to install NASM Open Source Assembly Compiler? (Y/N)"
		if ($install -eq "Y") {
			Write-Host "Installing NASM" -ForegroundColor Magenta
			Write-Host "You should install it in the path 'C:\nasm\'" -ForegroundColor Yellow
			Write-Host "NASM installer can be found at the following link 'https://www.nasm.us/'" -ForegroundColor Yellow

			Invoke-WebRequest -Uri "https://www.nasm.us/pub/nasm/releasebuilds/2.16.01/win64/nasm-2.16.01-installer-x64.exe" -OutFile $folderEnvironmentBinariesPath\nasm.exe
			$process = Start-Process -FilePath $folderEnvironmentBinariesPath\nasm.exe -PassThru
			$process.WaitForExit()
			while ($true) {
				$response = Read-Host "Has the NASM installation completed? (Y/N)"
				if ($response -eq "Y") {
					break
				}
			}
			Invoke-Expression -Command "set NASM_PREFIX=C:\nasm\"
			Write-Host "The environment variable NASM_PREFIX has been set (set NASM_PREFIX=C:\nasm\)." -ForegroundColor Yellow
		}

		$install = Read-Host "Do you want to install ACPI Source Language (ASL) Compiler? (Y/N)"
		if ($install -eq "Y") {
			Write-Host "Installing ASL" -ForegroundColor Magenta

			Invoke-WebRequest -Uri "https://downloadmirror.intel.com/783546/iasl-win-20230628.zip" -OutFile $folderEnvironmentBinariesPath\iasl-win.zip
			Expand-Archive -Path $folderEnvironmentBinariesPath\iasl-win.zip -DestinationPath "C:\ASL"
			Start-Sleep -Seconds 1
		}

		Remove-Item $folderEnvironmentBinariesPath -Recurse
	}
	Write-Host "Please restart your computer to proceed with the environment setup :)" -ForegroundColor Green
}



function OptionEnvironmentBootkits {

    Write-Host "You have selected the option 'Set Up Environment -> EDK2'" -ForegroundColor Green
	$response = Read-Host "Do you want to proceed? (Press 'Y')"
	if ($response -eq "Y") {

		$folderBootkits = "Bootkits"
		$folderBootkitsPath = Join-Path -Path $PWD -ChildPath $folderBootkits
		$install = Read-Host "Do you want to download and configure EDK2? (Y/N)"
		if ($install -eq "Y") {
			if (-not (Test-Path -Path $folderBootkitsPath)) {
				New-Item -ItemType Directory -Path $folderBootkitsPath | Out-Null
				Write-Host "Installing EDK2" -ForegroundColor Magenta
				Write-Host "You can follow the steps to install the environment manually, check this link 'https://github.com/tianocore/tianocore.github.io/wiki/Windows-systems'" -ForegroundColor Yellow

				$folderBack = $PWD
				Invoke-Expression -Command "git clone https://github.com/tianocore/edk2 $folderBootkitsPath/edk2"
				Set-Location -Path "$folderBootkitsPath\edk2"
				Invoke-Expression -Command "git submodule update --init"

				Start-Process cmd.exe -ArgumentList "/c edksetup.bat Rebuild" -Wait

				$filePath = "Conf/target.txt"
				$lines = Get-Content $filePath
				$lines = $lines -replace '^TARGET_ARCH .+ = .+', 'TARGET_ARCH = X64'
				$lines = $lines -replace '^TOOL_CHAIN_TAG .+ = .+', 'TOOL_CHAIN_TAG = VS2019'
				$lines = $lines -replace '^ACTIVE_PLATFORM .+ = .+', 'ACTIVE_PLATFORM = MdeModulePkg/MdeModulePkg.dsc'
				$lines | Set-Content $filePath

				Start-Process cmd.exe -ArgumentList "/c edksetup.bat Rebuild && build" -Wait

				Write-Host "A sample build has been executed using 'edksetup.bat Rebuild' and 'build' commands in a cmd window." -ForegroundColor Yellow
				Set-Location -Path "$folderBack"
			} else {
				Write-Host "The folder '$folderBootkitsPath' already exists in this directory. Unable to proceed." -ForegroundColor Red
			}
		}
	}
}



do {
    Show-Menu
    $choice = Read-Host "Choose an option"
    switch ($choice) {
        '1' { OptionRequirements }
        '2' { OptionEnvironmentBootkits }
        'Q' { break }
        default { Write-Host "Invalid option. Please choose again." }
    }
    Write-Host "Press any key to continue..."
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
} while ($choice -ne 'Q')
