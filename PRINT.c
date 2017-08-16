#define ARCHITECT16 1

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "MEMMAN.h"
#include "objects.h"
#include "print.h"
#include "win_printf.h"

// #define DEBUGSTR

s_int sc;

#ifdef __BORLANDC__
#define uart0Puts printf
#endif

int level;

void terpri(void)
{
   win_printf("\r\n");
//   uart0Puts("\r\n");
};

/*
#ifdef EMBEDDED
#else
char * string_printf(char *r, char *format,...)
{
 va_list arglist;
 char *buffer, *ret;

    buffer=(char *)malloc(10000);
    va_start(arglist, format);
    vsprintf(buffer, format, arglist );
    va_end( arglist );
    ret=(char*)(malloc(strlen(r)+strlen(buffer)));
    sprintf(ret,"%s%s",r,buffer);
    free(buffer);
    free(r);
    return(ret);
};
#endif

char * concat_string(char *r, char *f)
{
 char *buffer, *ret;

    ret=(char*)(malloc(strlen(r)+strlen(f)+10));
    sprintf(ret,"%s%s",r,f);
    free(r);
    return(ret);
};
*/

// печать атома
void prin1(PNODE p)
{

//  win_printf("prin1 beg!!!");
    if (is_n_atom(p)) {
        switch (get_n_type(p)) {
          case N_LONG: {
               win_printf("%d",get_n_longvalue(p));
               break;
          }
#if ARCHITECT16
#if USE_FLOAT

          case N_FLOAT: {
               win_printf("%f",get_n_floatvalue(p));
               break;
          }
#endif
#endif

          case N_SYMBOL: {
               win_printf("%s",get_n_symbolvalue(p));
               break;
          }

          case N_STRING: {
               #ifdef DEBUGSTR
                 win_printf("'%s'",get_n_stringvalue(p));
               #else
                 win_printf("%s",get_n_stringvalue(p));
               #endif
               break;
          }
          case N_STACKP: {
               win_printf("Code stack (%d)",get_n_stackpptr(p));
               break;
          }
        };
    } else {
        fatal_error("Invalid type %d.",get_n_type(p));
    };
//  win_printf("prin1 end!!!");
};

void prin0(PNODE p)
{
 PNODE r1,r2;
 s_int tp, len;

//  win_printf("prin0 beg!!!");
    if(level<99){
       level++;
    } else {
       return;
    };
    if(is_n_atom(p)) {
//  win_printf("prin0 1!!!");
        prin1(p);
//  win_printf("prin0 2!!!");
        goto e;
    } else {
//  win_printf("prin0 3!!!");

        if(is_n_list(p)) {
            r1=p;
            win_printf("(");
        a:

            prin0(get_n_listcar(r1));
            r2=r1;
            r1=get_n_listcdr(r1);

            if(is_n_atom(r1)) {
                if (r1!=NIL) {
                        win_printf(" ");
                        win_printf(".");
                        win_printf(" ");
                        prin1(r1);
                } else {
                    //      win_printf(" ");
                    //      prin1(r1);
                };
            } else {
                if(is_n_list(r1)) {
                        win_printf(" ");
                        goto a;
                } else {
                        win_printf(" ");
                        win_printf(".");
                        win_printf(" ");
                        prin0(r1);
                };
            };
            win_printf(")");
        } else {
//        win_printf("prin0 4!!!");

            r1=p;
            tp=0;
// патч!!
            if(is_n_tuple(r1)) {
p:
              win_printf("|");
              len=get_n_tuplelen(r1);
          b:
              if(tp == len) {
                  win_printf("|");
                  goto e;
              } else {

//              win_printf(">>>>> %d ",tp);
                  r2=get_n_tuplevalue(r1,tp);
                  prin0(r2);
//                    win_printf("prin0 401!!!");

              };
//              win_printf("<<<<<");
              tp++;
              if(tp == len) {
                  win_printf("|");
                  goto e;
              } else {
                  win_printf(", ");
              };
              goto b;
            };

            if ((get_n_type(r1)& MASK_TYPE)==6)
            switch(get_n_stype(r1)) {
               case 1:
                 win_printf("Стек");
               break;
               case 2:
                 len=get_n_tuplelen(r1);
                 win_printf("Фрейм %d",len);
                 goto p;
               break;
               case 3:
                 win_printf("Функция");
               break;
               case 4:
                 win_printf("Замыкание");
               break;
            };
       
        };
    };
e:
//   win_printf("prin0 end!!!");

   return;
};

PNODE print(PNODE p)
{
    level=0;
    prin0(p);
    terpri();
    return(p);
};

PNODE princ(PNODE p)
{
    level=0;
    prin0(p);
    return(p);
};

void prin_i0(PNODE p)
{
 PNODE r1,r2;
 s_int tp, len;

//  win_printf("prin0 beg!!!");

	if(is_n_atom(p)) {
        prin1(p);
        goto e;
    } else {
        if(is_n_list(p)) {
            win_printf("(List)\r\n");
        } else {
            win_printf("|Tuple|\r\n");
/*
            if (is_n_tuple(r1))
            switch(get_n_stype(r1)) {
               case 1:
                 win_printf("Стек");
               break;
               case 2:
                 win_printf("Фрейм");
               break;
               case 3:
                 win_printf("Функция");
			   break;
               case 4:
                 win_printf("Замыкание");
               break;
            };
*/
        };
    };
e:
   return;
};

PNODE print_i(PNODE p)
{
	prin_i0(p);
	terpri();
	return(p);
};

char *concat_string(char * str1, char * str2)
{
   char *ret;
   int len;
   
   len=strlen(str1)+strlen(str2);
   ret=malloc(len+1);
   strcpy(ret,str1);
   strcat(ret,str2);
   free(str1);
   return(ret);
};

// печать атома
char * sprin1(PNODE p, char * r)
{
 char * ret,  *ret1;
	if (is_n_atom(p)) {
		switch (get_n_type(p)) {
		  case N_LONG: {
			   ret=malloc(12);
			   sprintf(ret,"%ld",get_n_longvalue(p));
			   break;
		  }

#if ARCHITECT16
#if USE_FLOAT
		  case N_FLOAT: {
			   ret=malloc(25);
			   sprintf(ret,"%f",get_n_floatvalue(p));
			   break;
		  }
#endif
#endif

		  case N_SYMBOL: {
			   ret=malloc(get_n_symbollen(p)+1);
			   sprintf(ret,"%s",get_n_symbolvalue(p));
			   break;
		  }

		  case N_STRING: {
			   ret=malloc(get_n_stringlen(p)+1);
			   sprintf(ret,"\"%s\"",get_n_stringvalue(p));
			   break;
		  }
		  case N_STACKP: {
			   ret=malloc(25);
			   sprintf(ret,"Code stack %ld",get_n_stackpptr(p));
//win_printf("%s", ret);
			   break;
		  }
		};
		ret1=(char*)(malloc(strlen(r)+strlen(ret)+10));

//win_printf("%s", ret);

        sprintf(ret1,"%s%s",r,ret);
//win_printf("%s", ret1);

        free(ret);
        free(r);
    } else {
        fatal_error("Invalid type %d.",get_n_type(p));
    };

    return(ret1);
};

char * sprin0(PNODE p, char *r)
{
 PNODE r1,r2;
 s_int tp;
 char * ret, *ret1;

    ret=r;
    if(is_n_atom(p)) {
        ret1=sprin1(p,ret);
        ret=ret1;
//        win_printf("%s", ret);
    } else {
        if(is_n_list(p)) {
            r1=p;
            ret=concat_string(ret,"(");
        a:
            
            ret1=sprin0(get_n_listcar(r1),ret);
            ret=ret1;
            r2=r1;
            r1=get_n_listcdr(r1);

            if(is_n_atom(r1)) {
                if (r1!=NIL) {
						ret=concat_string(ret," ");
						ret=concat_string(ret,".");
						ret=concat_string(ret," ");
                        ret=sprin1(r1,ret);
                } else {
                    //      win_printf(" ");
                    //      prin1(r1);
				};
            } else {
//                printf("prin0L> %ld, %d %ld\n",(long)p,is_n_atom(p),r2);

                if(is_n_list(r1)) {
                        ret=concat_string(ret," ");
                        goto a;
                } else {
                        ret=concat_string(ret," ");
                        ret=concat_string(ret,".");
                        ret=concat_string(ret," ");
                        ret=sprin0(r1,ret);
                };
            };
            ret=concat_string(ret,")");
        } else {
			r1=p;
            tp=0;
// патч!!
            if(is_n_tuple(r1)==0) {
              ret=concat_string(ret,"|");
          b:
              if(tp == get_n_tuplelen(r1)) {
                  ret=concat_string(ret,"|");
                  return(ret);
              } else {
                  ret1=sprin0(get_n_tuplevalue(r1,tp),ret);
                  ret=ret1;
              };
              tp++;
              if(tp == get_n_tuplelen(r1)) {
                  ret=concat_string(ret,"|");
                  return(ret);
              } else {
                  ret=concat_string(ret,", ");
              };
              goto b;
            };
//
//            switch(get_n_stype(r1)) {
//               case 1:
//                 ret=concat_string(ret,"Стек");
//               break;
//               case 2:
//                 ret=concat_string(ret,"Фрейм");
//               break;
//               case 3:
//                 ret=concat_string(ret,"Функция");
//               break;
//               case 4:
//                 ret=concat_string(ret,"Замыкание");
//               break;
//            };
//
		};
	};
	return(ret);
};

char *sprinc(PNODE p)
{
 char *r;

	r=malloc(2);
	sprintf(r,"");
	return(sprin0(p,r));
};
