# 🖥️ PowerShell Script for Automating UEFI Bootkit Development Environment Setup in Windows

This PowerShell script is meticulously crafted to automate the process of setting up a development environment tailored for [UEFI](https://wiki.osdev.org/UEFI) bootkit creation on Windows. It streamlines the otherwise manual and time-consuming tasks of installing necessary tools and configuring the [TianoCore](https://www.tianocore.org/) [EDK2](https://github.com/tianocore/edk2) (EFI Development Kit) development environment, enabling developers to focus more on bootkits innovation and less on setup 😉.


## Features

The script simplifies two steps:

1. **EDK2 Requirements**: It automates the download and installation of all necessary development tools, including [Visual Studio 2019 Community](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=16), [Git](https://git-scm.com/), [Python](https://www.python.org/), [NASM](https://www.nasm.us/) and [ASL](https://www.intel.com/content/www/us/en/download/774881/acpi-component-architecture-downloads-windows-binary-tools.html).
2. **Set Up EDK2 Environment**: It facilitates the seamless [configuration](https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II) of the EDK2 environment, a cornerstone for crafting UEFI bootkits, ensuring that developers have the right foundation to start their projects.


## How to Execute

To automate your development environment setup, follow these steps:

1. Open PowerShell.
2. Navigate to the folder containing the script.
3. Execute the script by entering: `.\Setup_Development_Environment.ps1`
4. Respond to the on-screen prompts to select your desired action.


## Usage Example

```
=== Options ===
1. Requirements -> Visual Studio 2019 Community + Git + Python + NASM + ASL
2. Set Up Environment -> EDK2
Q. Exit
============
Choose an option:
```
