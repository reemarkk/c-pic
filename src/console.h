#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "string.h"

UINT32 PrintFormatedString(const CHAR *format, ...);
// Function to write output to the console
UINT32 WriteConsole(const CHAR *output, USIZE outputLength);

#endif // __CONSOLE_H__