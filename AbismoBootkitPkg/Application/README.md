## Abismo - UEFI Application

---



### UEFI Application

An [UEFI Application](https://tianocore-docs.github.io/edk2-ModuleWriteGuide/draft/4_uefi_applications/) is an EFI image of the type EFI_IMAGE_SUBSYSTEM_EFI_APPLICATION. This image is executed and automatically unloaded when the image exits or returns from its entry point. OS loader is a special type of application that normally does not return or exit. Instead, it calls the EFI Boot Service gBS->ExitBootServices() to transfer control of the platform from the firmware to an operating system.

---



### Files

```
├───Application
│   │   AbismoApplicationUEFI.c
│   │   AbismoApplicationUEFI.inf
│   │   README.md
│   │
│   └───Functions
│       │   1FunctionsScreen.c
│       │   1FunctionsScreen.h
│       │   2FunctionsBanner.c
│       │   2FunctionsBanner.h
│       │   3FunctionsBoot.c
│       │   3FunctionsBoot.h
│       │
│       └───Utils
│               1FunctionsUtilsEfiSystemPartition.c
│               1FunctionsUtilsEfiSystemPartition.h
```

---
