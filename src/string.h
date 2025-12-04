#ifndef __STRING_H__
#define __STRING_H__

#include "primitives.h"

#define towlower(c) (((c) >= L'A' && (c) <= L'Z') ? ((c) + (L'a' - L'A')) : (c))
#define TO_LOWER_CASE(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + ('a' - 'A')) : (c))

// Case-insensitive string comparison
BOOL CompareStringIgnoreCase(const CHAR *s1, const CHAR *s2);

// Case-insensitive wide string comparison
BOOL CompareWideStringIgnoreCase(const WCHAR *s1, const WCHAR *s2);

// Get the length of a null-terminated string
USIZE GetStringLength(const CHAR *str);

// Convert a string to a double-precision floating-point number
DOUBLE StringToDouble(const CHAR *s);

INT32 String_FormatV(PCHAR s, const CHAR *format, VA_LIST args);

#endif // __STRING_H__