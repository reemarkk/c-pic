#include "string.h"

// Custom case-insensitive string comparison
BOOL CompareStringIgnoreCase(const PCHAR s1, const PCHAR s2) {
	PCHAR str1 = s1;
	PCHAR str2 = s2;
	// Loop through each character in both strings
	while (*str1 && *str2) {
		// Convert to lowercase for case-insensitive comparison
		CHAR c1 = tolower(*str1);
		CHAR c2 = tolower(*str2);
		// Compare the characters
		if (c1 != c2) {
			return FALSE; // They differ in case-folded form
		}
		// Move to the next characters
		str1++;
		str2++;
	}
	return (*str1 == *str2); // Both must land on the null terminator together
}

// Custom case-insensitive wide string comparison
BOOL CompareWideStringIgnoreCase(const PWCHAR s1, const PWCHAR s2) {
	PWCHAR str1 = s1;
	PWCHAR str2 = s2;
	// Loop through each character in both strings
	while (*str1 && *str2) {
		// Convert to lowercase cause-insensitive comparison
		WCHAR c1 = towlower(*str1);
		WCHAR c2 = towlower(*str2);
		// Compare the characters
		if (c1 != c2) {
			return FALSE; // They differ in case-folded form
		}
		// Move to the next characters
		str1++;
		str2++;
	}
	return (*str1 == *str2); // Both must land on the null terminator together
}

USIZE GetStringLength(const PCHAR str) {
    const CHAR* s = str;
    while (*s) {
        s++;
    }
    return s - str;
}