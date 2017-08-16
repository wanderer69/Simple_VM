#ifdef	__cplusplus
extern "C" {
#endif
#if EMBEDDED
__dprintf (char *fmt, ...);
#define win_printf __dprintf
#else
#ifdef __BORLANDC__
#include <stdio.h>
void win_printf(char *format, ...);
//#define win_printf printf
#else
#ifdef __LINUX
#include <stdio.h>
void win_printf(char *format, ...);
#define sprintf_n sprintf
//#define win_printf printf
#else
#define win_printf printf_n
#endif
#endif
#endif
#define fatal_error win_printf
#ifdef	__cplusplus
}
#endif

