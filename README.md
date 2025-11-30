
# C Position-Independent Code (PIC)

Small cross‑platform demo that compiles a tiny C program into **position‑independent code** and shows how to:

- resolve Windows APIs manually via the PEB + PE exports,
- use raw Linux syscalls,
- and extract a compact, “shellcode‑like” blob from a normal executable.

[Original GitHub Project](https://github.com/mrzaxaryan/c-pic)

> ⚠️ This repository is for **low‑level systems / compiler education and research only**.
> Do **not** use it in any way that violates laws, school rules, workplace policies, or service terms.

---

## Table of Contents

- [Overview](#overview)
- [Quick Start (Step‑by‑step)](#quick-start-step-by-step)
  - [1. Set up the toolchain](#1-set-up-the-toolchain)
  - [2. Build all targets](#2-build-all-targets)
  - [3. Run the Windows samples](#3-run-the-windows-samples)
  - [4. Run the Linux samples](#4-run-the-linux-samples)
- [Features](#features)
- [Supported Targets](#supported-targets)
- [How It Works](#how-it-works)
  - [Windows Path](#windows-path)
  - [Linux Path](#linux-path)
- [Project Layout](#project-layout)
- [Strings & Data in PIC](#strings--data-in-pic)
- [Research & Ethics](#research--ethics)
- [Reference](#reference)

---

## Overview

The sample program itself just prints `"Hello world!\n"` and exits cleanly,
but the techniques are general and can be extended to more complex payloads.

This project demonstrates how to:

- obtain the Process Environment Block (PEB) manually on Windows;
- walk the loader data structures to find the base address of `Kernel32.dll`;
- parse PE export tables to resolve functions such as `WriteConsoleA`;
- perform direct Linux syscalls (`write`, `exit`) without libc;
- cross‑compile the same C source for multiple OS / architecture targets;
- extract a compact, contiguous shellcode‑like blob from the resulting binary.

In C language position‑independent code (PIC) programming has some limitations:
- No use of global/static variables that require relocation.
- No reliance on absolute addresses; everything must be relative.
- No use of string literals or data that the compiler might place in separate sections.
- Stack-based strings must be used or string literals must be marked using attributes to put them in the same section as code.
- Can not use callback functions or function pointers that the compiler might place in separate sections.
- ** Or just use assembler where you have full control over the layout. But that is more complex, time-consuming, and error-prone for larger projects. **

This project fixes that by: 
- providing a custom linker script to keep code and read-only data together.
- ensuring that all data is properly aligned for position-independent access.

However, putting read-only data in the same section will resolve most of the issues related to position-independent code, but in Windows i386 architecture, MSVC, GCC, and Clang compilers still emit absolute addresses for string literals and callback functions.

This project fixes that by having no external library dependencies and no headers included so it could be possible to compile as an ELF Linux i386 architecture where string literals are accessed using relative addressing. 
The core logic is always built Windows and Linux payloads as Linux target triples but compiled with PLATFORM_WINDOWS or PLATFORM_LINUX macros. It becomes possible as project does not rely on any Windows or Linux-specific headers, standard libraries, or compiler routines; only LLVM/Clang needed to produce position-independent code that is written in C, a more high-level language than assembler.
- **primitives.h** defines primitives for Linux and Windows platforms and fixes the issues like **WCHAR is an unsigned short type in Windows but an int type in Linux.**

---

## Quick Start (Step‑by‑step)

### 1. Set up the toolchain

1. Use a **Windows host** machine.
2. Install an **LLVM / Clang** toolchain (e.g. LLVM binaries or via a package manager).
3. Make sure the following tools are available on your `PATH`:
   - `clang` and `lld` (able to target the desired triples),
   - `llvm-objcopy`,
   - `llvm-objdump`,
   - `llvm-strings`.
4. Clone or download this repository, then open a **Developer Command Prompt** or PowerShell in the repo root.

### 2. Build all targets

1. From the repository root, run:

   ```powershell
   .\build.bat
   ```

2. If the build succeeds, you should see a `bin\` directory that contains:

   - One directory per platform, for example:
     - `bin\windows\...`
     - `bin\linux\...`
   - Within each platform, one sub‑directory or file set per architecture:
     - e.g. `windows_amd64`, `windows_i386`, `linux_amd64`, etc.
   - For each target:
     - a standard executable, and
     - extracted “PIC blob” artifacts (raw byte blob plus small helper files:
       section dump, string dump, size summary).

3. Build behavior (triples, flags, filenames) is mostly driven by:
   - `build.bat` (top‑level orchestration),
   - `toolchain.bat` (per‑target pipeline and naming).

### 3. Run the Windows samples

1. On your Windows test VM / machine, copy one of the built binaries from:

   ```text
   bin\windows\...
   ```

2. Open a console window (Command Prompt or PowerShell).
3. Run the chosen executable.
4. You should see:

   ```text
   Hello world!
   ```

   printed to the console with no explicit imports for the usual Windows user‑mode DLLs, beyond what the loader itself needs.

### 4. Run the Linux samples

1. Copy a Linux binary from:

   ```text
   bin\linux\...
   ```

   to a **Linux** test machine that matches the target architecture (e.g. x86‑64, ARM64).

2. Make the binary executable and run it. For example:

   ```bash
   chmod +x ./linux_amd64      # example name
   ./linux_amd64
   ```

3. You should again see:

   ```text
   Hello world!
   ```

   followed by a clean exit via the appropriate syscall.

> 💡 Keep all experiments inside disposable VMs or containers.
> That makes it easier to tear down and rebuild environments and reduces the risk of accidentally violating any policies.

---

## Features

- **No CRT / libc** – only minimal integer / pointer / string primitives.
- **No Windows headers** – required NT / PE structures are re‑declared locally.
- **Manual API resolution** – functions obtained via PEB + PE export parsing.
- **Position‑independent output** – safe to copy the code blob to an arbitrary address.
- **Cross‑OS, cross‑arch** – Windows & Linux; x86, x86‑64, and ARM64.
- **Single `.text` segment** – linker script keeps code & read‑only data together.
- **Shellcode‑oriented artifacts** – build emits a raw byte blob plus basic metadata.

---

## Supported Targets

The build scripts currently target:

- **Windows**
  - 32‑bit x86
  - 64‑bit x86‑64
  - 32‑bit ARMv7
  - 64‑bit ARM64
- **Linux**
  - 32‑bit x86
  - 64‑bit x86‑64
  - 64‑bit ARM64

The exact triples and flags live in `build.bat` and are passed down into `toolchain.bat`.
You can tweak or extend those scripts to add/remove targets as needed.

---

## How It Works

### Windows Path

On Windows builds (`PLATFORM_WINDOWS`):

1. **Locate the PEB.**  
   `GetCurrentPEB` in `peb.c` uses the appropriate segment register
   (e.g. `FS` / `GS` / `x18`, depending on architecture) to obtain a pointer to the current process’s PEB.

2. **Find Kernel32.**  
   `GetModuleHandleFromPEB` walks the linked list of loaded modules in `PEB_LDR_DATA`
   to find `Kernel32.dll` by case‑insensitive name.

3. **Resolve exports.**  
   `GetExportAddress` in `pe.c` parses the PE headers and export directory of `Kernel32.dll`
   to look up function addresses by name (e.g. `WriteConsoleA`).

4. **Call via function pointers.**  
   `console.windows.c` casts the resolved address to the appropriate function pointer type
   and uses it to call `WriteConsoleA`.

5. **Entry / exit glue.**  
   `main.c` prepares the `"Hello world!\n"` string, calls `WriteConsole`,
   then uses the `ExitProcess` macro from `environment.h` to terminate cleanly.

All of this happens **without** including `<windows.h>` or linking against the usual Windows import libraries.

### Linux Path

On Linux builds (`PLATFORM_LINUX`):

- `console.linux.c` implements `WriteConsole` using inline assembly to invoke the `write` syscall (`rax = 1`) on `stdout`.
- `environment.linux.c` implements `TerminateProcess` using the appropriate `exit` syscall for each supported architecture.
- `main.c` stays shared between platforms: it just calls `WriteConsole` and then `ExitProcess`.

Because the Linux path does not rely on external libraries or global data,
the resulting code is also naturally position‑independent.

---

## Project Layout

```text
src/
  main.c               # Shared entrypoint that prints "Hello world!"
  primitives.h         # Minimal type & integer / pointer definitions
  string.{c,h}         # Small helpers: length + case-insensitive comparisons
  environment.h        # Common macros + platform-specific entry / exit glue
  environment.linux.c  # Linux TerminateProcess implementation
  console.h            # Platform-neutral WriteConsole declaration
  console.windows.c    # Windows implementation, uses manual API resolution
  console.linux.c      # Linux implementation, uses raw syscalls
  peb.{c,h}            # PEB structures + module enumeration (Windows only)
  pe.{c,h}             # PE header / export directory parsing helpers
linker.script          # lld linker script: pack .text / .rodata into one segment
build.bat              # Top-level build script for all targets
toolchain.bat          # Per-target build & extraction pipeline
```

---

## Strings & Data in PIC

Position‑independent **code** is usually straightforward; the tricky part is **data**:

- Compilers like to place string literals and constants in `.rdata` / `.rodata`.
- If you later extract only a single section (for shellcode‑like use),
  any pointers into other sections become invalid.
- On x86‑64, RIP‑relative addressing hides much of this:
  as long as code and data live in the same contiguous region, relative references keep working.
- On 32‑bit x86 windows, compilers often emit **absolute addresses**, so the blob must either:
  - use stack‑based strings,
  - use custom loader

This repository uses a pragmatic mix:

- a custom `linker.script` keeps `.text` and `.rodata` together in one loadable segment;
- the C code tries to avoid unexpected global data;
- architecture‑specific notes in the source highlight where extra work would be needed
  for more complex payloads, especially on 32‑bit x86.

If you extend the project with additional global data or many string literals,
keep these constraints in mind so the extracted blob remains truly position‑independent.

---

## Research & Ethics

This repository is intended for:

- learning how binaries, loaders, and calling conventions work;
- experimenting with compiler output and custom link layouts;
- building better intuition for what “position‑independent code” actually means.

It is **not** intended for:

- bypassing security controls,
- running untrusted third‑party payloads,
- or any use that would violate laws, school rules, workplace policies, or service terms.

Always:

- work in isolated test environments (VMs, lab machines, or containers);
- only experiment on systems you own or have explicit permission to use;
- follow local laws and responsible security‑research practices.

---

## Reference

- Original idea and inspiration: [mrzaxaryan / c-pic](https://github.com/mrzaxaryan/c-pic)
