# Position-Independent String (PIS)

A practical approach for embedding and using strings in Windows shellcode, overcoming the limitations of PE section extraction and position-independent code.

## Table of Contents
- [Problem](#problem)
- [Workarounds](#workarounds)
- [Proposed Solution](#proposed-solution)
- [Technical Details](#technical-details)
- [Pattern Scanning Example](#pattern-scanning-example)
- [Usage](#usage)
- [Detection & Ethics Note](#detection--ethics-note)
- [References](#references)

## Problem
In C shellcode, a common workaround is to use stack strings (e.g., `char s[] = { 'H','e','l','l','o',0 };`) to embed strings directly in the code. For more details on compiling shellcode and using stack strings, see the related project README: [C Shellcode Compilation Guide](../c-shellcode/).
When extracting only the `.text` section from a PE file for shellcode, string literals in `.rdata` or `.data` are lost. This complicates runtime string handling in position-independent code.

## Workarounds

### Stack Strings
Construct strings at runtime:
```c
char helloWorld[] = { 'H','e','l','l','o','W','o','r','l','d',0 };
```
- **Pros:** Obfuscates strings and hinders static analysis.
- **Cons:** Increases code complexity and size.

### Place Strings in `.text`
Force string literals into `.text`:
```c
#pragma code_seg(".text")
__declspec(allocate(".text"))
char helloWorld[] = "HelloWorld";
```

### Custom Loader
Manually map PE/COFF files and perform relocations. This offers control but adds complexity and a detectable fingerprint.

## Proposed Solution
Modify the linker script to merge `.data` and `.rdata` into `.text`:
```
SECTIONS
{
    .text 0x0 : ALIGN(1) SUBALIGN(1)
    {
        *(.text$mainCRTStartup)
        *(.text*)
        *(.data*)
        *(.rdata*)
    }
}
```
This ensures all code and data are present in the shellcode blob.

## Technical Details

### x86_64 (64-bit)
- Uses RIP-relative addressing for data and literals.
- PIC code works seamlessly after extraction.

### x86 (32-bit)
- Data references are absolute; manual relocation is required.
- Shellcode must determine its base address and fix pointers at runtime.

Relocation example:
```c
#define GetBaseAddress() (*(PVOID *)((BYTE *)__readfsdword(0x30) + 0x10))
#define SetBaseAddress(v) (*(PVOID *)((BYTE *)__readfsdword(0x30) + 0x10) = (v))
#define Relocate(s) (GetBaseAddress() + s)

BOOL WINAPI mainCRTStartup() {
    char* rip = (char*)_ReturnAddress();
    uint8_t x86Prologue[] = {0x55, 0x89, 0xE5}; // x86 prologue
    rip = scan_for_pattern((uint8_t*)rip, x86Prologue, sizeof(x86Prologue));
    SetBaseAddress(rip);
    char* invalidString = "HelloWorld";
    char* validString = Relocate(invalidString);
}
```

## Pattern Scanning Example

To locate the x86 function prologue (`push ebp; mov ebp, esp`):
```c
uint8_t x86Prologue[] = {0x55, 0x89, 0xE5};
uint8_t* found = scan_for_pattern(rip, x86Prologue, sizeof(x86Prologue));
```

Implementation:
```c
uint8_t* scan_for_pattern(uint8_t* rip, const uint8_t* pattern, size_t len)
{
    uint8_t* p = rip;
    while (1) {
        size_t i = 0;
        for (; i < len; i++) {
            if (p[i] != pattern[i])
                break;
        }
        if (i == len)
            return p; // found match
        p--; // move backward
    }
}
```
This is useful for runtime code analysis and shellcode relocation.

## Usage
1. Update your linker script as shown above.
2. Use relocation logic for x86 shellcode.
3. Use `scan_for_pattern` to locate function prologues or data patterns.

## Detection & Ethics Note

**Research-only.**
- This technique does not guarantee stealth. Security products may detect hardware breakpoint usage, VEH patterns, or emulator behavior.
- Do not use to evade detection, run untrusted code, or break laws/policies.
- Always test in isolated, offline VMs and follow responsible disclosure and research ethics.

## References
- [Original PIS Project](https://github.com/mrzaxaryan/PIS)

