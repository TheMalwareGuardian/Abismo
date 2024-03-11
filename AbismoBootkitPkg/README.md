## Abismo UEFI Windows Bootkit

<p align="center">
  <img src="../images/logos/Windows_Bootkit.png">
</p>

The Abismo UEFI Windows Bootkit is a sophisticated piece of malware designed to infiltrate Windows systems at the most fundamental level, by targeting the boot process. Its primary mechanism of action involves patching three critical components of the Windows boot sequence: the Windows Boot Manager (bootmgfw.efi), the Windows OS Loader (winload.efi), and the Windows Kernel itself (ntoskrnl.exe).

The core objective of this bootkit is to disable Driver Signature Enforcement (DSE), a security feature that prevents unsigned drivers from being loaded into the system. By circumventing DSE, the Abismo bootkit gains the capability to install a kernel driver equipped with extensive surveillance functionalities, including a keylogger, and mechanisms to conceal malicious activities. This includes hiding files, processes, connections, and other evidence of its presence, thereby modifying the Windows kernel's code to facilitate undetected operation within the infected system.

Through these modifications, the Abismo bootkit achieves a high level of stealth and persistence, embedding itself deeply within the system's architecture. Its ability to manipulate the boot process and kernel operations allows it to carry out its malicious activities undetected by traditional antivirus and security solutions, representing a significant threat to system integrity and user privacy.

Furthermore, the Abismo UEFI Windows Bootkit incorporates functionalities observed in other notorious bootkits found in the wild, such as [ESPecter](https://www.welivesecurity.com/2021/10/05/uefi-threats-moving-esp-introducing-especter-bootkit/) and [BlackLotus](https://www.welivesecurity.com/2023/03/01/blacklotus-uefi-bootkit-myth-confirmed/), showcasing its comprehensive approach to system compromise.

It is designed to be installed on systems where Secure Boot has been disabled, or alternatively, through physical access to the target computer, a scenario commonly referred to as an "[evil maid](https://encyclopedia.kaspersky.com/glossary/evil-maid/)" attack.
