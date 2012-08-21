#include <windows.h>
#include <stdio.h>
#include <stdarg.h>


void panic(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	ExitProcess(0);
}
