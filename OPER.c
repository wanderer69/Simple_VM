#define ARCHITECT16 1

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "MEMMAN.h"
#include "vm.h"
#include "objects.h"
#include "print.h"
#include "win_printf.h"

#define ERROR_INVALID_TYPE 1
#define ERROR_DIVIDE_BY_ZERO 2

// глобальная константа ошибки (для вызова прерывания)
extern s_int global_reg_error;

#if USE_FLOAT
// арифметика
PNODE n_add(PNODE p1, PNODE p2)
{
 long l1,l2;
 double f1,f2;
 PNODE ret;
    global_reg_error=0;
    if((is_n_numeric(p1)) && (is_n_numeric(p2))) {
        if((is_n_float(p1)) || (is_n_float(p2))) {
            if(is_n_float(p1)) {
                f1=get_n_floatvalue(p1);
            } else {
                f1=(double)(get_n_longvalue(p1));
            };
            if(is_n_float(p2)) {
                f2=get_n_floatvalue(p2);
            } else {
                f2=(double)(get_n_longvalue(p2));
            };
            ret=make_n_float(f1+f2);
        } else {
            l1=get_n_longvalue(p1);
            l2=get_n_longvalue(p2);
            ret=make_n_long(l1+l2);
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_sub(PNODE p1, PNODE p2)
{
 long l1,l2;
 double f1,f2;
 PNODE ret;
    global_reg_error=0;
    if((is_n_numeric(p1)) && (is_n_numeric(p2))) {
        if((is_n_float(p1)) || (is_n_float(p2))) {
            if(is_n_float(p1)) {
                f1=get_n_floatvalue(p1);
            } else {
                f1=(double)(get_n_longvalue(p1));
            };
            if(is_n_float(p2)) {
                f2=get_n_floatvalue(p2);
            } else {
                f2=(double)(get_n_longvalue(p2));
            };
            ret=make_n_float(f1-f2);
        } else {
            l1=get_n_longvalue(p1);
            l2=get_n_longvalue(p2);
            ret=make_n_long(l1-l2);
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_mul(PNODE p1, PNODE p2)
{
 long l1,l2;
 double f1,f2;
 PNODE ret;
    global_reg_error=0;
    if((is_n_numeric(p1)) && (is_n_numeric(p2))) {
        if((is_n_float(p1)) || (is_n_float(p2))) {
            if(is_n_float(p1)) {
                f1=get_n_floatvalue(p1);
            } else {
                f1=(double)(get_n_longvalue(p1));
            };
            if(is_n_float(p2)) {
                f2=get_n_floatvalue(p2);
            } else {
                f2=(double)(get_n_longvalue(p2));
            };
            ret=make_n_float(f1*f2);
        } else {
            l1=get_n_longvalue(p1);
            l2=get_n_longvalue(p2);
            ret=make_n_long(l1*l2);
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_div(PNODE p1, PNODE p2)
{
 long l1,l2;
 double f1,f2;
 PNODE ret;
    global_reg_error=0;
    if((is_n_numeric(p1)) && (is_n_numeric(p2))) {
        if((is_n_float(p1)) || (is_n_float(p2))) {
            if(is_n_float(p1)) {
                f1=get_n_floatvalue(p1);
            } else {
                f1=(double)(get_n_longvalue(p1));
            };
            if(is_n_float(p2)) {
                f2=get_n_floatvalue(p2);
            } else {
                f2=(double)(get_n_longvalue(p2));
            };
            if(f2==0) {
                global_reg_error=ERROR_DIVIDE_BY_ZERO;
                ret=NIL;
            } else {
                ret=make_n_float(f1/f2);
            };
        } else {
            l1=get_n_longvalue(p1);
            l2=get_n_longvalue(p2);
            if(l2==0) {
                global_reg_error=ERROR_DIVIDE_BY_ZERO;
                ret=NIL;
            } else {
                ret=make_n_long(l1/l2);
            };
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_neg(PNODE p1)
{
 long l1;
 double f1;
 PNODE ret;
    global_reg_error=0;
    if (is_n_numeric(p1)) {
        if (is_n_float(p1)) {
            f1=get_n_floatvalue(p1);
            ret=make_n_float(-f1);
        } else {
            l1=get_n_longvalue(p1);
            ret=make_n_long(-l1);
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

// сравнение
PNODE n_numeq(PNODE p1, PNODE p2)
{
 long l1,l2;
 double f1,f2;
 PNODE ret;
    global_reg_error=0;
    if((is_n_numeric(p1)) && (is_n_numeric(p2))) {
        if((is_n_float(p1)) || (is_n_float(p2))) {
            if(is_n_float(p1)) {
                f1=get_n_floatvalue(p1);
            } else {
                f1=(double)(get_n_longvalue(p1));
            };
            if(is_n_float(p2)) {
                f2=get_n_floatvalue(p2);
            } else {
                f2=(double)(get_n_longvalue(p2));
            };
            if(f1==f2) {
                ret=p1;
            } else {
                ret=FALSE;
            };
        } else {
            l1=get_n_longvalue(p1);
            l2=get_n_longvalue(p2);
            if(l1==l2) {
                ret=p1;
            } else {
                ret=FALSE;
            };
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_numgt(PNODE p1, PNODE p2)
{
 long l1,l2;
 double f1,f2;
 PNODE ret;
    global_reg_error=0;
    if((is_n_numeric(p1)) && (is_n_numeric(p2))) {
        if((is_n_float(p1)) || (is_n_float(p2))) {
            if(is_n_float(p1)) {
                f1=get_n_floatvalue(p1);
            } else {
                f1=(double)(get_n_longvalue(p1));
            };
            if(is_n_float(p2)) {
                f2=get_n_floatvalue(p2);
            } else {
                f2=(double)(get_n_longvalue(p2));
            };
            if(f1>f2) {
                ret=p1;
            } else {
                ret=FALSE;
            };
        } else {
            l1=get_n_longvalue(p1);
            l2=get_n_longvalue(p2);
            if(l1>l2) {
                ret=p1;
            } else {
                ret=FALSE;
            };
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_numlt(PNODE p1, PNODE p2)
{
 long l1,l2;
 double f1,f2;
 PNODE ret;
    global_reg_error=0;
    if((is_n_numeric(p1)) && (is_n_numeric(p2))) {
        if((is_n_float(p1)) || (is_n_float(p2))) {
            if(is_n_float(p1)) {
                f1=get_n_floatvalue(p1);
            } else {
                f1=(double)(get_n_longvalue(p1));
            };
            if(is_n_float(p2)) {
                f2=get_n_floatvalue(p2);
            } else {
                f2=(double)(get_n_longvalue(p2));
            };
            if(f1<f2) {
                ret=p1;
            } else {
                ret=FALSE;
            };
        } else {
            l1=get_n_longvalue(p1);
            l2=get_n_longvalue(p2);
            if(l1<l2) {
                ret=p1;
            } else {
                ret=FALSE;
            };
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};
#else
PNODE n_arith(PNODE p1, PNODE p2, char d)
{
 long l1,l2,l3;
// char * s1, * s2;
 PNODE ret;
    global_reg_error=0;
//    print(p1);
//    print(p2);
    if((is_n_numeric(p1)) && (is_n_numeric(p2))) {
       l1=get_n_longvalue(p2);
       l2=get_n_longvalue(p1);
//       win_printf("> l1=%x l2=%x\r\n",l1,l2);

       switch(d){
          case '+':
            ret=make_n_long(l1+l2);
          break;
          case '-':
            ret=make_n_long(l1-l2);
          break;
          case '/':
            if(l2==0) {
                global_reg_error=ERROR_DIVIDE_BY_ZERO;
                ret=NIL;
            } else {
                l3=l1/l2;
//                win_printf(">>> %x\r\n",l3);
                ret=make_n_long(l3);
//                print(ret);
            };
          break;
          case '*':
            ret=make_n_long(l1*l2);
          break;
          case '<':
            if(l1<l2) {
                ret=p1;
            } else {
                ret=FALSE;
            };
          break;
          case '>':
            if(l1>l2) {
                ret=p1;
            } else {
                ret=FALSE;
            };
          break;
          case '=':
            if(l1==l2) {
                ret=p1;
            } else {
                ret=FALSE;
            };
          break;
       };
    } else
    if((is_n_string(p1)) && (is_n_string(p2))) {
       ret=cont_n_string(p2,p1);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_neg(PNODE p1)
{
 long l1;
 PNODE ret;
    global_reg_error=0;
    if (is_n_numeric(p1)) {
         l1=get_n_longvalue(p1);
         ret=make_n_long(-l1);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

#endif

PNODE n_stringeq(PNODE p1, PNODE p2)
{
 PNODE ret;
    if((is_n_string(p1)) && (is_n_string(p2))) {
        if(strcmp(get_n_stringvalue(p1),get_n_stringvalue(p2))==0) {
            ret=p1;
        } else {
            ret=FALSE;
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_symboleq(PNODE p1, PNODE p2)
{
 PNODE ret;
    if((is_n_symbol(p1)) && (is_n_symbol(p2))) {
//    win_printf("'%s'-'%s'\n",get_n_symbolvalue(p1),get_n_symbolvalue(p2));
        if(strcmp(get_n_symbolvalue(p1),get_n_symbolvalue(p2))==0) {
            ret=p1;
        } else {
            ret=FALSE;
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_listeq(PNODE p1, PNODE p2)
{
 PNODE ret;
    if((is_n_list(p1)) && (is_n_list(p2))) {
        if((get_n_listcar(p1)==get_n_listcar(p2)) && (get_n_listcdr(p1)==get_n_listcdr(p2))) {
            ret=p1;
        } else {
            ret=FALSE;
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_tupleeq(PNODE p1, PNODE p2)
{
 PNODE ret;
    if((is_n_tuple(p1)) && (is_n_tuple(p2))) {
        if(p1==p2) {     // ?????
            ret=p1;
        } else {
            ret=FALSE;
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

// общая команда сравнения
PNODE n_equal(PNODE p1, PNODE p2)
{
 PNODE ret;

    global_reg_error=0;
//    win_printf("p1="); princ(p1); win_printf(" p2="); print(p2);
    if ((is_n_numeric(p1)) && (is_n_numeric(p2))) {
       // сравним числа
#if USE_FLOAT
       ret=n_numeq(p1,p2);
#else
       ret=n_arith(p1,p2,'=');
#endif
    } else {
        if ((is_n_string(p1)) && (is_n_string(p2))) {
           // сравним строки
           ret=n_stringeq(p1,p2);
        } else {
            if ((is_n_symbol(p1)) && (is_n_symbol(p2))) {
              // сравним символы
              ret=n_symboleq(p1,p2);
            } else {
                if ((is_n_list(p1)) && (is_n_list(p2))) {
                   // сравним символы
                   ret=n_listeq(p1,p2);
                } else {
                    if ((is_n_tuple(p1)) && (is_n_tuple(p2))) {
                       // сравним символы
                       ret=n_tupleeq(p1,p2);
                    } else {
      //                 global_reg_error=ERROR_INVALID_TYPE;
                       ret=FALSE;
                    };
                };
            };
        };
    };

    return(ret);
};

// логические
PNODE n_logand(PNODE p1, PNODE p2)
{
 PNODE ret;
    if((p1!=FALSE) && (p1!=NIL)) {
        if((p2!=FALSE) && (p2!=NIL)) {
            ret=TRUE;
        } else {
            ret=FALSE;
        };
    } else {
        ret=FALSE;
    };
    return(ret);
};

PNODE n_logor(PNODE p1, PNODE p2)
{
 PNODE ret;
    if((p1!=FALSE) && (p1!=NIL)) {
        if((p2!=FALSE) && (p2!=NIL)) {
            ret=TRUE;
        } else {
            ret=TRUE;
        };
    } else {
        if((p2!=FALSE) && (p2!=NIL)) {
            ret=TRUE;
        } else {
            ret=FALSE;
        };
    };
    return(ret);
};

PNODE n_lognot(PNODE p1)
{
 PNODE ret;
    if((p1!=FALSE) && (p1!=NIL)) {
        ret=FALSE;
    } else {
        ret=TRUE;
    };
    return(ret);
};

// строковые
PNODE n_addstring(PNODE p1, PNODE p2)
{
 PNODE ret;
    if((is_n_string(p1)) && (is_n_string(p2))) {
        ret=(PNODE)cont_n_string(p1,p2);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_findstring(PNODE p1, PNODE p2)
{
 PNODE ret;
    if((is_n_string(p1)) && (is_n_string(p2))) {
        if(get_n_stringlen(p1)<get_n_stringlen(p2)) {
            ret=NIL;
        } else {
            ret=make_n_long(find_n_string(p1,p2));
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_midstring(PNODE p1, PNODE p2, PNODE p3)
{
 PNODE ret;
 long l1, l2;
 
//    print(p1);
//    print(p2);
//    print(p3);

    if((is_n_string(p1)) && (is_n_long(p2)) && (is_n_long(p3))) {
        l1=get_n_longvalue(p2);
        l2=get_n_longvalue(p3);
        if((get_n_stringlen(p1)<l1) || (get_n_stringlen(p1)<(l1+l2))) {
            ret=NIL;
        } else {
            ret=(PNODE)mid_n_string(p1,l1,l2);
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_lenstring(PNODE p1)
{
 PNODE ret;
 long l1;
    if(is_n_string(p1)){
        l1=get_n_stringlen(p1);
        ret=make_n_long(l1);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

#if USE_FLOAT
// преобразования типов
PNODE n_long_to_float(PNODE p)
{
 PNODE ret;
    ret=make_n_float((double)(get_n_longvalue(p)));
    return(ret);
};

PNODE n_float_to_long(PNODE p)
{
 PNODE ret;

    ret=make_n_long((long)(get_n_floatvalue(p)));
    return(ret);
};
#endif

PNODE n_long_to_string(PNODE p)
{
 PNODE ret;
 char s[45];
// print(p);
    sprintf_n(s,"%ld",(long)(get_n_longvalue(p)));
// win_printf("->> %s\r\n",s);
    ret=make_n_string(s);
// print(ret);
    return(ret);
};

#if USE_FLOAT
PNODE n_float_to_string(PNODE p)
{
 PNODE ret;
 char s[45];
    sprintf_n(s,"%f",(double)(get_n_floatvalue(p)));
    ret=make_n_string(s);
    return(ret);
};
#endif

PNODE n_string_to_long(PNODE p)
{
 PNODE ret;
 int l;
#ifdef __LINUX
    l=strtol(get_n_stringvalue(p),NULL,0);
    ret=make_n_long(l);
#else
    ret=make_n_long(atoi_s(get_n_stringvalue(p)));
#endif
    return(ret);
};

#if USE_FLOAT
PNODE n_string_to_float(PNODE p)
{
 PNODE ret;
    ret=make_n_float((double)atof(get_n_stringvalue(p)));
    return(ret);
};
#endif

PNODE n_symbol_to_string(PNODE p)
{
 PNODE ret;
 char * s;
// print(p);
    s=get_n_symbolvalue(p);
// win_printf("->> %s\r\n",s);
    ret=make_n_string(s);
// print(ret);
    return(ret);
};

PNODE n_string_to_symbol(PNODE p)
{
 PNODE ret;

    ret=make_global_symbol(get_n_stringvalue(p));
    return(ret);
};

// кортеж
PNODE n_tuple(PNODE p)
{
 PNODE ret;
//    print(p);
    if(is_n_long(p)) {
          ret=make_n_tuple(get_n_longvalue(p));
          fill_n_tuple(ret,NIL);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

void n_settuplevalue(PNODE p1,PNODE p2,PNODE p3)
{
//    print(p1);print(p2);
    if((is_n_tuple(p1)) && (is_n_long(p2))) {
        set_n_tuplevalue(p1,get_n_longvalue(p2),p3);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
    };
};

PNODE n_gettuplevalue(PNODE p1,PNODE p2)
{
 PNODE ret;
 s_int l;

//    print(p1);print(p2);
    if((is_n_tuple(p1)) && (is_n_long(p2))) {
        l=get_n_longvalue(p2);
        if(l>=get_n_tuplelen(p1)){
//          print(p2);
//          print(get_n_tuplevalue(p1,l));
          ret=NIL;
        } else {
          ret=get_n_tuplevalue(p1,l);
        };
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_resizetuple(PNODE p1,PNODE p2)
{
 PNODE ret;
    if((is_n_tuple(p1)) && (is_n_long(p2))) {
        ret=(PNODE)resize_n_tuple(p1,get_n_longvalue(p2));
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_lentuple(PNODE p1)
{
 PNODE ret;
 long l;
    if(is_n_tuple(p1)) {
        l=get_n_tuplelen(p1);
        ret=make_n_long(l);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_addtuple(PNODE p1, PNODE p2)
{
 PNODE ret;
    if((is_n_tuple(p1)) && (is_n_tuple(p2))) {
//        ret=get_n_tuple(p1,get_n_longvalue(p2));
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

// фрейм
PNODE n_frame(PNODE p)
{
 PNODE ret;
    if(is_n_symbol(p)) {
          ret=make_n_frame(p);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

PNODE n_frame_from(PNODE p, PNODE k)
{
 PNODE ret;
    if((is_n_symbol(p)) && (is_n_frame(k))) {
          ret=copy_n_frame(p,k);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

void n_add_frame_slot(PNODE p1,PNODE p2)
{
//    win_printf("%d %d\r\n",get_n_type(p1),get_n_type(p2));
    if((is_n_frame(p1)) && (is_n_symbol(p2))) {
        add_n_frame_slot(p1,p2);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
    };
};

void n_del_frame_slot(PNODE p1,PNODE p2)
{
    if((is_n_frame(p1)) && (is_n_symbol(p2))) {
        del_n_frame_slot(p1,p2);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
    };
};

void n_set_framevalue(PNODE p1,PNODE p2,PNODE p3)
{
//    print(p1);print(p2);print(p3);
    if((is_n_frame(p1)) && (is_n_symbol(p2))) {
        set_n_frame_slot_value(p1,p2,p3);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
    };

};

PNODE n_get_framevalue(PNODE p1,PNODE p2)
{
 PNODE ret;
    if((is_n_frame(p1)) && (is_n_symbol(p2))) {
        ret=get_n_frame_slot_value(p1,p2);
    } else {
        global_reg_error=ERROR_INVALID_TYPE;
        ret=NIL;
    };
    return(ret);
};

// вызов функции который я вынес
void n_call_func(PNODE f, s_int key)
{
    s_int *n, l;
    PNODE p, c, ret=NIL;
    s_int (*fcommand)();
    long k;
    s_int m;

    // сохраним текущее замыкание в стеке

//    push_n_stack(get_current_stackd, global_frame.closure);

    if (key==0) {
        p=get_n_tuplevalue(f,1);
//        print(f);
        if(global_type_vm_func==0){
           set_current_closurecounter((s_int)current_pc+5);
        } else if(global_type_vm_func==1){
           global_pc_counter=global_pc_counter+5;
        };
    } else {
        p=f;
        if(global_type_vm_func==0){
           set_current_closurecounter((s_int)current_pc+1);
        } else if(global_type_vm_func==1){
           global_pc_counter=global_pc_counter+1;
        };
    };
    // сохраним в стеке точку возврата
//    push_n_stackp(get_current_stackc, (s_int)current_pc);

    if(is_n_func(p)) {
        if(global_type_vm_func==0){
          // вызов демона а потом вызов того чего надо
          // создадим замыкание
          c=make_n_closurefunc(p,get_n_tuplevalue(get_global_streamclosure,global_stream_closure));
          set_n_closureprev(c,get_n_tuplevalue(get_global_streamclosure,global_stream_closure));
          // делаем текущей
          set_n_tuplevalue(get_global_streamclosure,global_stream_closure,c);
  //        unpack_closure(c);

          // загрузка аргументов из стека
          load_funcarg();

          // устанавливаем счетчик команд на 0
          set_current_closurecounter(0);
        } else if(global_type_vm_func==1){
          // надо скопировать все указатели в стек возвратов
          push_to_bstack(global_arg_point);
          push_to_bstack(global_prev_point);
          push_to_bstack(global_var_point);
          push_to_bstack(global_const_point);
          push_to_bstack(global_func_point);
          push_to_bstack(global_pc_counter);
          global_prev_func=global_current_func;
          copy_func_to_stack(p);
          global_pc_counter=0;
//          print_n_stackvalue(get_current_stackd);
        };
    } else {
        if(is_n_long(p)) {
             // загрузка встроенной функции или из библиотеки
             fcommand=(s_int (*)())get_n_longvalue(p);
             m=(*fcommand)();
             // очистим стек до уровня метки и считаем сохраненую функцию
             l=0;
             while((p=get_n_stacksvalue(get_current_stackd,l))!=SLABEL) {
                  if(l==0) ret=p;
                  l++;
             };
             pool_count_up(ret);

//    info_block(4498);
             // l==0 -> приводит к ошибке
             if(l<1) ret=NIL;
             l++;
             if(l!=0) {
//                  p=get_n_stackvalue(get_current_stackd,l-1);
                  // очистим стек
                  while(l!=0) {
                        pop_n_stack(get_current_stackd);
                        l--;
                  };
//    info_block(4498);
             } else {
                  fatal_error(" Нарушена синхронность стека данных!\n");
                  global_end_exec=3;
             };
             // вернем значение !!!
             if(m!=0) push_n_stack(get_current_stackd,ret);
             pool_count_down(ret);
//    info_block(4498);

//             k=pop_n_stackp(get_current_stackc);

        } else {
                 fatal_error(" функция %s неопределена!\n",get_n_symbolvalue(get_n_tuplevalue(f,0)));
                 global_end_exec=4;
        };
    };
};

// отладочная команда

void n_break(long n) {
//    break_command(n);
};
