#include "environment.h"

DOUBLE MakeDouble(INT32 intPart, INT32 fracPart)
{
    // Step 1: Count digits in fracPart
    INT32 x = fracPart;
    INT32 digits = 0;
    while (x > 0) { x /= 10; digits++; }

    // Step 2: Extract most significant digit
    INT32 pow10 = 1;
    for (INT32 i = 1; i < digits; i++)
        pow10 *= 10;

    INT32 firstDigit = fracPart / pow10;

    // Step 3: Trim first digit if it is '1' and there are more digits
    if (firstDigit == 1 && digits > 1) {
        fracPart %= pow10;   // remove MSB
        digits--;
    }

    // Step 4: Compute denominator correctly
    DOUBLE denom = 1.0;
    for (INT32 i = 0; i < digits; i++)
        denom *= 10.0;

    return (DOUBLE)intPart + (DOUBLE)fracPart / denom;
}


#if defined(PLATFORM_WINDOWS_I386)

PCHAR GetInstructionAddress(VOID)
{
    return __builtin_return_address(0);
}

PCHAR ReversePatternSearch(PCHAR rip, const CHAR *pattern, UINT32 len)
{
    PCHAR p = rip;
    while (1)
    {
        UINT32 i = 0;
        for (; i < len; i++)
        {
            if (p[i] != pattern[i])
                break;
        }
        if (i == len)
            return p; // found match
        p--;          // move backward
    }
}

// Logic:
// We support two execution modes:
//   1) Normal Windows executable:
//        - Image is loaded by the OS loader.
//        - Relocations are applied.
//        - All pointers to our image are based on ENV_BASE.
//   2) PIC blob:
//        - Image is mapped and executed manually.
//        - Relocations are NOT applied.
//        - All compiler-generated addresses still assume IMAGE_LINK_BASE.
//
// Runtime information:
//   - IMAGE_LINK_BASE: original image base used during linking (always 0x401000).
//   - ENV_BASE      : actual base address where the image is loaded in memory at runtime.
//   - pointer       : some absolute address that may be:
//                       * already relocated by the loader (ENV_BASE + offset), or
//                       * still at the original link-time location (IMAGE_LINK_BASE + offset), or
//                       * not part of our image at all.
//

PVOID RebaseLiteral(PVOID p)
{
    // Get the PEB
    PPEB peb = GetCurrentPEB();

    // Go to loader data
    PPEB_LDR_DATA ldr = peb->LoaderData;

    // First module in InMemoryOrderModuleList is our EXE
    PLIST_ENTRY list = &ldr->InMemoryOrderModuleList;
    PLIST_ENTRY flink = list->Flink;

    // Convert LIST_ENTRY to LDR_DATA_TABLE_ENTRY
    PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

    // Get the Entry point of EXE
    USIZE EntryPoint = (USIZE)entry->EntryPoint;

    // If it is _start function then our app is the EXE
    if (EntryPoint == (USIZE)GetEnvironmentBaseAddress())
    {
        // Pointer is already relocated
        return p;
    }
    // Otherwise we are running as a PIC blob
    else
    {
        // and we should do the relocation ourselves
        return (PVOID)((USIZE)p + GetEnvironmentBaseAddress() - IMAGE_LINK_BASE);
    }
}

#endif // PLATFORM_WINDOWS_I386