#ifdef EMBEDDED
#else
#include <stdio.h>
#ifdef __BORLANDC__
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef _WIN
void window1_print(char * str);

void win_printf(char *format, ...)
{
   va_list ap;
   int arg;
   char buffer[512];
   FILE *f;

   f=fopen("simple.log","a+");
   va_start(ap, format);
   vsprintf(buffer, format, ap);
//   printf(buffer);
   window1_print(buffer);
   if(f!=NULL){
     fseek(f,0,SEEK_END);
     fprintf(f,buffer);
     fclose(f);
   };
   va_end(ap);
};
#else
void win_printf(char *format, ...)
{
   va_list ap;
   int arg;
   char buffer[512];
   FILE *f;

   f=fopen("simple.log","a+");
   va_start(ap, format);
   vsprintf(buffer, format, ap);
//   uart0Puts(buffer);
   printf(buffer);
   if(f!=NULL){
     fseek(f,0,SEEK_END);
     fprintf(f,buffer);
     fclose(f);
   };
   va_end(ap);
};
#endif
#else
#ifdef __LINUX
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
void win_printf(char *format, ...)
{
   va_list ap;
   int arg;
   char buffer[512];
   FILE *f;

   f=fopen("simple.log","a+");
   va_start(ap, format);
   vsprintf(buffer, format, ap);
   printf(buffer);
   if(f!=NULL){
     fseek(f,0,SEEK_END);
     fprintf(f,buffer);
     fclose(f);
   };
   va_end(ap);
};
#else
#define win_printf printf_n
#endif
#endif
#endif
 
