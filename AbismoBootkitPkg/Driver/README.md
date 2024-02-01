## Abismo - DXE Runtime Driver

---



### DXE Runtime Driver

A DXE driver refers to a driver compliant with the [PI Specification](https://uefi.org/specs/PI/1.8/index.html), which classifies DXE drivers into two classes: UEFI driver model drivers, and non-UEFI driver model drivers.

Non-UEFI Driver Model drivers are executed early in the DXE phase. These drivers are the prerequisites for the [DXE Foundation](https://uefi.org/specs/PI/1.8/V2_DXE_Foundation.html#dxe-foundation) to produce all required services. DXE drivers must be designed so that unavailable services are not required. Given this restriction, all possible work should be deferred to the UEFI drivers.

A [DXE runtime driver](https://tianocore-docs.github.io/edk2-ModuleWriteGuide/draft/8_dxe_drivers_non-uefi_drivers/88_dxe_runtime_driver.html) executes in both boot services and runtime services environments. This means the services that these modules produce are available before and after ExitBootServices() is called, including the time that an operating system is running. If SetVirtualAddressMap() is called, then modules of this type are relocated according to virtual address map provided by the operating system.

The DXE Foundation is considered a boot service component, so the DXE Foundation is also released when ExitBootServices() is called. As a result, runtime drivers may not use any of the UEFI Boot Services, DXE Services, or services produced by boot service drivers after ExitBootServices() is called.

A DXE runtime driver defines MODULE_TYPE as DXE_RUNTIME_DRIVER in the INF file. In addition, because the DXE runtime driver encounters SetVirtualAddressMap() during its life cycle, it may need to register an event handler for the event EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE.

---



### Files

```
├───Driver
│   │   AbismoDriverDXERuntime.c
│   │   AbismoDriverDXERuntime.inf
│   │   README.md
│   │
│   ├───Functions
│   │   │   1FunctionsPatchHookImage.c
│   │   │   1FunctionsPatchHookImage.h
│   │   │   2FunctionsPatchHookWindowsBootManager.c
│   │   │   2FunctionsPatchHookWindowsBootManager.h
│   │   │   3FunctionsPatchHookWindowsOSLoader.c
│   │   │   3FunctionsPatchHookWindowsOSLoader.h
│   │   │   4FunctionsPatchHookWindowsKernel.c
│   │   │   4FunctionsPatchHookWindowsKernel.h
│   │   │
│   │   ├───Hooks
│   │   │       1FunctionsHooksBootmgfwEfi1ArchStartBootApplication.c
│   │   │       1FunctionsHooksBootmgfwEfi1ArchStartBootApplication.h
│   │   │       2FunctionsHooksWinloadEfi1OslFwpKernelSetupPhase1.c
│   │   │       2FunctionsHooksWinloadEfi1OslFwpKernelSetupPhase1.h
│   │   │
│   │   ├───Protections
│   │   │
│   │   └───Utils
│   │           1FunctionsUtilsStrings.c
│   │           1FunctionsUtilsStrings.h
│   │           2FunctionsUtilsRegisters.c
│   │           2FunctionsUtilsRegisters.h
│   │           3FunctionsUtilsMemory.c
│   │           3FunctionsUtilsMemory.h
│   │           4FunctionsUtilsHeaders.c
│   │           4FunctionsUtilsHeaders.h
│   │           5FunctionsUtilsPattern.c
│   │           5FunctionsUtilsPattern.h
│   │           6FunctionsUtilsAddress.c
│   │           6FunctionsUtilsAddress.h
│   │           7FunctionsUtilsPortableExecutable.c
│   │           7FunctionsUtilsPortableExecutable.h
│   │           8FunctionsUtilsTables.c
│   │           8FunctionsUtilsTables.h
│   │           9FunctionsUtilsHooks.h
│   │           10FunctionsUtilsSignatures.h
│   │
│   └───Structures
│           1arc.h
│           2pe.h
```

---
