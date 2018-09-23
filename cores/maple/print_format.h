#ifndef print_format_H_
#define print_format_H_

#include <stdarg.h>

typedef void (*pfn_outputchar)(char c, void* p);

//extern "C"{
	int _print_format (pfn_outputchar pfn, void* pvoid, const char *format, va_list ap);
//}

#endif