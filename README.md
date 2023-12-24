
<p align="center">
  <img src="images/Abismo_Squid_Construction.png">
</p>


## Overview

Abismo is a comprehensive project thoroughly designed with the explicit goal of establishing a robust foundation for the development of bootkits. By offering a centralized repository of knowledge, Abismo stands as a valuable initiative for anyone looking to contribute to and benefit from the collective understanding of this field. However, it is imperative to underscore that Abismo is not a tool intended for malicious purposes; rather, it is a carefully constructed initiative for educational exploration and practical insights.

Abismo's significance extends beyond its basic functionality within Windows environments; it serves as a gateway for individuals venturing into the intricate and advanced field of bootkit development.


## Note

This project represents a dedicated exploration into the intricate realm of bootkits, with a specific focus on providing a comprehensive resource for both my students and individuals embarking on their journey in this complex field.

The subject of bootkits is multifaceted, and this project serves as a starting point for understanding its nuances. It's important to acknowledge that certain aspects may assume a level of prior knowledge, while others may remain uncharted due to the inherent complexities of the subject matter.

This work is designed to offer valuable insights and resources to support your educational and developmental goals, making it suitable for anyone seeking to delve into bootkit development. If you have specific inquiries, require additional clarification, or wish to engage in collaborative efforts, please do not hesitate to get in [touch](https://www.linkedin.com/in/vazquez-vazquez-alejandro/).


## What is a Bootkit?

"Bootkits are a type of modern malware used by a threat actor to attach malicious software to a computer system. Bootkits can be a critical security threat to your business and often involve rootkit tools for evading detection. These attacks target the Unified Extensible Firmware Interface (UEFI), software that connects a PC’s operating system with its device firmware". ~ [CrowdStrike](https://www.crowdstrike.com/cybersecurity-101/malware/bootkit/)

Essentially, a bootkit is a form of malicious software strategically designed to target a computer's operating system boot process. Put plainly, a bootkit is a rootkit that loads before the operating system.

In the case of Abismo, this manifests as a specialized focus on the complexities inherent in UEFI.



## Windows Boot Process

Before starting to develop a bootkit that targets Windows systems, it's essential to understand the [Windows boot process](https://uefi.org/sites/default/files/resources/UEFI-Plugfest-WindowsBootEnvironment.pdf), a complex sequence of events that starts when a computer is turned on and ends when the operating system is fully loaded:

* ***Power-On Self-Test (POST)***: When you power on your PC, the BIOS/UEFI firmware performs the POST operation to verify the integrity of the hardware components and ensures that no errors exist, allowing the system to boot up correctly.
* ***UEFI Phase***: In modern systems with UEFI firmware, the UEFI phase begins after POST. UEFI initializes the hardware required for booting and then looks for a boot device (like a hard drive, SSD, CD/DVD, USB drive).
* ***Loading bootmgfw.efi***: The file bootmgfw.efi is the UEFI Windows Boot Manager, stored in the EFI System Partition (ESP). The UEFI firmware loads bootmgfw.efi from the ESP. This Boot Manager is responsible for presenting boot options (if multiple operating systems or boot options are available) and managing the initial boot process.
* ***Loading winload.efi***: Once a boot option is selected (or automatically chosen in a single-boot system), bootmgfw.efi loads winload.efi, the Windows OS loader. winload.efi is responsible for loading the essential drivers needed to start Windows and preparing the system for the handoff to the Windows kernel. It's also during this phase that important system files are verified for integrity.
* ***Loading ntoskrnl.exe (NT OS Kernel)***: After the initial setup, winload.efi loads the Windows NT kernel (ntoskrnl.exe) into memory. The kernel takes over and initializes the system's core subsystems (like security, process and memory management, hardware abstraction, etc.).


This critical process serves as the foundation for a computer's startup, and [measures](https://learn.microsoft.com/en-us/windows/security/operating-system-security/system-security/secure-the-windows-10-boot-process) have been included to protect it from threats.

<p align="center">
  <img width="400px" src="images/Boot_Process.png">
</p>



## Unified Extensible Firmware Interface (UEFI)


### Introduction

[UEFI](https://wiki.osdev.org/UEFI) is a [specification](https://uefi.org/specifications) for x86, x86-64, ARM, and Itanium platforms that defines a software interface between the operating system and the platform firmware/BIOS.

The UEFI firmware loads a UEFI application (a relocatable PE executable file of arbitrary size) from a FAT partition on a GPT or MBR partitioned boot device to an address dynamically chosen at run-time. Subsequently, it invokes the main entry point of that application.

When UEFI firmware calls the entry point function of a UEFI application, it provides a "System Table" structure, which contains pointers to all of the system's ACPI (Advanced Configuration and Power Interface) tables, memory map, and other information relevant to an operating system.

The UEFI firmware establishes many callable functions in memory, organized into sets known as "protocols," discoverable through the System Table. An important aspect to note is that each function's behavior, within each protocol, is defined by specification.

Additionally, UEFI applications have the capability to define their own protocols and persist them in memory for other UEFI applications to use. These functions adhere to a standardized and modern calling convention supported by many C compilers.

UEFI applications can be developed in any language that can be compiled and linked into a PE executable file and supports the calling convention used to access functions established in memory by the UEFI firmware. In practice, this typically involves one of three development environments: [POSIX-UEFI](https://gitlab.com/bztsrc/posix-uefi), [GNU-EFI](https://sourceforge.net/projects/gnu-efi/) or [EDK2](https://github.com/tianocore/edk2).


### EDK2

[EDK2](https://wiki.osdev.org/EDK2) is the official development environment for UEFI applications and also has tools for UEFI driver developers. It is developed by the open-source [Tianocore](https://www.tianocore.org/) project, of which Intel, HP, and Microsoft are the primary contributors.


### Boot Stages

UEFI encompasses six primary boot phases, which are all critical in the initialization process of the platform. These collectively form what is known as [Platform Initialization](https://uefi.org/specs/PI/1.8/index.html) (PI):

* ***Security Phase (SEC)***: The [Security Phase](https://uefi.org/specs/PI/1.8/V1_Security_SEC_Phase_Information.html) is the initial stage of the UEFI boot process. It typically contains minimal assembly code specific to the architecture. Its primary function is to ensure the integrity and authenticity of the UEFI firmware before executing it. During this phase, temporary memory is initialized and it serves as the root of trust in the system.
* ***Pre-EFI Initialization (PEI)***: The Pre-EFI Initialization stage follows SEC, leveraging the CPU's existing resources to manage Pre-EFI Initialization Modules (PEIMs). These PEIMs are responsible for handling critical hardware initialization tasks, such as configuring main memory in a process known as CAR (Cache as RAM). Additionally, this phase facilitates the transition of control to the Driver Execution Environment (DXE).
* ***Driver Execution Environment (DXE)***: DXE is the next phase, where the majority of the system initialization occurs. At this point, main memory is accessible, so CPU, chipset, mainboard, and other I/O devices are initialized. During this stage, hardware drivers, runtime services, and any boot services required for the operating system to start, are loaded.
* ***Boot Device Select (BDS)***: BDS represents the phase where boot devices are initialized. During BDS, UEFI drivers or Option ROMs for PCI devices are executed according to the system's configuration. This phase is responsible for processing boot options and selecting the appropriate boot device. The selected boot entry is then loaded and executed in preparation for the Transient System Load (TSL).
* ***Transient System Load (TSL)***: TSL serves as the stage between boot device selection and the hand-off to the operating system.
During this phase, it's possible to launch an UEFI application like a shell or a boot loader that takes the responsibility of ending the UEFI Boot Services by making the ExitBootServices() call.
* ***Runtime (RT)***: The Runtime phase marks the hand-off to the operating system (OS) once the ExitBootServices() function is executed. In this phase, a UEFI-compatible OS takes control and is responsible for exiting boot services. This action triggers the firmware to unload unnecessary code and data, retaining only runtime services and relying on their own programs, such as kernel drivers to manage hardware devices.

<p align="center">
  <img width="500px" src="images/UEFI_Boot_Stages.png">
</p>



## Building an EDK2 UEFI application (Windows)


### Environment

To set up the necessary environment for UEFI application development using EDK2 on [Windows](https://github.com/tianocore/tianocore.github.io/wiki/Windows-systems), the following virtual machines (VMs), tools, and packages are essential:


* #### Machine

    Download a [Windows 10](https://www.microsoft.com/en-us/software-download/windows10) or [Windows 11](https://www.microsoft.com/en-us/software-download/windows11) ISO image and set up a virtual machine using [VirtualBox](https://www.virtualbox.org/) or [VMware Workstation](https://www.vmware.com/products/workstation-pro/workstation-pro-evaluation.html), configuring it with 70 GB of storage and 8 gigabytes of RAM.


* #### Visual Studio 2019 Community

    Download [Visual Studio 2019 Community](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=16) and, before installing, make sure to select the "[Desktop development with C++](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-160)" workload.


* #### Git

    Install [Git](https://git-scm.com/) by downloading the appropriate [binary](https://git-scm.com/download/win).


* #### Python

    Install [Python37](https://www.python.org/downloads/release/python-370/) or a later [version](https://www.python.org/) and set the PYTHON_HOME environment variable (set PYTHON_HOME=C:\Python37).

* #### NASM Open Source Assembly Compiler

    To [setup](https://github.com/tianocore/tianocore.github.io/wiki/Nasm-Setup) NASM, download the binary from the [website](https://www.nasm.us/), install it in the C:\nasm\ directory, and configure the environment variable "NASM_PREFIX" (set NASM_PREFIX=C:\nasm\).


* #### ACPI Source Language (ASL) Compiler

    To [setup](https://github.com/tianocore/tianocore.github.io/wiki/Asl-Setup) ASL, download the [Windows Binary Tools](https://www.intel.com/content/www/us/en/download/774881/acpi-component-architecture-downloads-windows-binary-tools.html) release package and place the unzipped content "iasl.exe" into the "C:\ASL" directory (create it if it doesn't exist).

* #### Workspace

    Execute the commands in a [PowerShell](https://learn.microsoft.com/en-us/powershell/scripting/learn/ps101/01-getting-started?view=powershell-7.4) to complete the setup of the environment:

    ```
    git clone https://github.com/tianocore/edk2.git
    cd edk2
    git submodule update --init
    cd ..
    ```

* #### Compile BaseTools

    Open a CMD prompt and execute the command '*edksetup.bat Rebuild*'. This process may take several minutes.

    This will compile the EDK2 build tools into edk2\BaseTools\Bin\Win32 and copy the default configuration templates from edk2\BaseTools\Conf to edk2\Conf.

* #### Configure target.txt

    Open the file [edk2\Conf\target.txt](https://github.com/tianocore/edk2/blob/master/BaseTools/Conf/target.template) and modify the values of the architecture, compiler, and platform variables:
    * TARGET_ARCH = X64 # 64 bits
    * [TOOL_CHAIN_TAG](https://github.com/tianocore/tianocore.github.io/wiki/Windows-systems-ToolChain-Matrix) = VS2019 # Visual Studio 2019
    * ACTIVE_PLATFORM = MdeModulePkg/MdeModulePkg.dsc # Complete UEFI wrapper library

* #### Script

    To simplify the process of establishing a bootkit development environment on Windows, a corresponding PowerShell script [Setup_Development_Environment.ps1](Setup_Development_Environment.ps1) has been created. This script is designed to walk you through the necessary downloads and installations.

    <p align="center">
        <img width="800px" src="images/Run_Script_Setup_Environment.png">
    </p>


### Create a project

After successfully establishing the foundational structure of EDK2, the process of [bulding UEFI applications](https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-Writing-Simple-Application) becomes remarkably straightforward:

1. Create a directory that contains the application.
    ```
    cd edk2
    mkdir FirstUEFIApp
    ```

2. Create, at least, the files [FirstUEFIAppCode.c](https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-Writing-MyHelloWorld.c) and [FirstUEFIAppCode.inf](https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-Writing-MyHelloWorld.inf).


### Build

In order to obtain the .efi file associated with that code, simply follow these steps in an orderly manner:

1. Update an existing platform .DSC file with the project's .inf file. The application should be added to the "Components" section just before the "BuildOptions" section (e.g., FirstUEFIApp/FirstUEFIAppCode.inf).

2. Open a command-prompt and ran "*edksetup.bat*", then, again from the top-level directory, simply run "*build*".

3. The final output, an .efi file, will be located in the directory edk2/Build/MdeModulePkg/DEBUG_$(TOOL_CHAIN_TAG)/X64.


### Test

At this point, we already have the .efi application (in this case, it was a simple 'Hello, World'), and we just need to test that it works. To do this, we can mount the EFI system partition and copy the application there. Next, we restart the machine, enter the BIOS (F12), and using the UEFI shell, we execute the application.

```
mountvol M: /S
cd M:\EFI\Boot
copy C:\.....\edk2\Build\MdeModulePkg\DEBUG_$(TOOL_CHAIN_TAG)\X64\FirstUEFIAppCode.efi .
```

<p align="center">
    <img width="350px" src="images/EFI_Shell_First.png">
</p>



## Abismo Bootkit V0.1


### Download

```
cd C:\Users\user1\Documents\Bootkits\
git clone https://github.com/TheMalwareGuardian/Abismo
$sourcePath = "C:\Users\user1\Documents\Bootkits\Abismo\AbismoBootkitPkg"
$destinationPath = "C:\Users\user1\Documents\Bootkits\edk2\AbismoBootkitPkg"
Copy-Item -Path $sourcePath -Destination $destinationPath -Recurse
```


### Configuration

```
cd C:\Users\user1\Documents\Bootkits\edk2
$filePath = "$PWD\conf\target.txt"
$content = Get-Content -Path $filePath
$content = $content -replace 'ACTIVE_PLATFORM = MdeModulePkg/MdeModulePkg.dsc', 'ACTIVE_PLATFORM = AbismoBootkitPkg/AbismoBootkitPkg.dsc'
$content | Set-Content -Path $filePath
```


### Build

```
Open Cmd
edksetup.bat
build
```


### Installation

```
cd C:\Users\user1\Documents\Bootkits\edk2\Build\AbismoBootkitPkg\DEBUG_VS2019\X64
Copy "AbismoApplicationUEFI.efi" and "AbismoDriverDXERuntime.efi" 
cd AbismoBootkitPkg\Installer\
python -m venv venv
pip3 install -r requirements.txt
venv\Scripts\activate
python AbismoInstaller.py
```



## Videos

[![](https://github.com/TheMalwareGuardian/Abismo/tree/main/videos/V01.png)]({https://github.com/TheMalwareGuardian/Abismo/tree/main/videos/V01.mp4})



## Resources

* ***[Awesome Bootkits & Rootkits Development](https://github.com/TheMalwareGuardian/Awesome-Bootkits-Rootkits-Development)***: My compilation (+100) of extensive resources dedicated to bootkit and rootkit development.



## License

This project is licensed under the GNU GLPv3 License - see the [LICENSE](LICENSE) file for details.


## 
