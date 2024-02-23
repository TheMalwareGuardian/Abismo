# Bootkits

<p align="center">
  <img src="../images/logos/Octopus_Flowers.png">
</p>

---

This folder contains a variety of UEFI Applications developed to demonstrate bootkit basic functionalities:

* _**MalwareUnderRadarPkg**_ is an UEFI Application that serves as a demonstration of a basic bootkit functionality. This application is responsible for downloading files during the boot process. By operating at the UEFI (Unified Extensible Firmware Interface) level, MalwareUnderRadarPkg gains execution control before the operating system's security features, such as antivirus or EDRs, have a chance to start. This early execution allows it to perform malicious activities, such as downloading additional malware components, modifying system files, or establishing persistent access, with minimal detection risk.
