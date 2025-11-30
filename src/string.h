#ifndef __STRING_H__
#define __STRING_H__

#include "primitives.h"


#define towlower(c) (((c) >= L'A' && (c) <= L'Z') ? ((c) + (L'a' - L'A')) : (c))
#define tolower(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + ('a' - 'A')) : (c))

// Custom case-insensitive string comparison
BOOL CompareStringIgnoreCase(const PCHAR s1, const PCHAR s2);

// Custom case-insensitive wide string comparison
BOOL CompareWideStringIgnoreCase(const PWCHAR s1, const PWCHAR s2);

// Get the length of a null-terminated string
USIZE GetStringLength(const PCHAR str);

#endif // __STRING_H__