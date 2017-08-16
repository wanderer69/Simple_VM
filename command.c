#define ARCHITECT16 1

//#include <conio.h>
//#include <stdio.h>
#include <stdlib.h>

#include "MEMMAN.h"
#include "vm.h"
#include "objects.h"
#include "oper.h"
#include "print.h"
#include "win_printf.h"

// КОМАНДЫ
extern s_int global_debug_code;

extern int global_arg_point; // точка от которой отсчитываются аргументы
extern int global_var_point; // точка от которой отсчитываются переменные

// начнем с простых
void cmd_nop()
{
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// обмен данными
void cmd_pushc()
{
    s_int n;
    // возьмем второй аргумент
    if(global_type_vm_func==0){
      n=get_long_value((char *)((s_int)current_code+current_pc+1));
      // найдем значение константы и запихаем в стек данных
      push_n_stack(get_current_stackd,get_n_tuplevalue(get_current_const, n));
    } else if(global_type_vm_func==1){
      n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
      // найдем значение константы и запихаем в стек данных
//      win_printf("Const ptr %d\r\n",global_const_point);
//      win_printf("pusch\r\n");
//      print(get_n_stackvalue(get_current_stackd,global_const_point-n));
//      print(get_n_stackvalue(get_current_stackd,global_const_point-1));
//      print(get_n_stackvalue(get_current_stackd,global_const_point));
//      win_printf("end\r\n");
      push_n_stack(get_current_stackd,get_n_stackvalue(get_current_stackd,global_const_point-n));
    };
//    print(get_n_tuplevalue(get_current_const, n));
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

void cmd_drop()
{
    pop_n_stack(get_current_stackd);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

int global_type_vm_func; // тип выполняемой фенкции

void cmd_savev()
{
    PNODE v, n, p;
    // возьмем значение переменной из стека данных и сохраним в переменной
    // исправлено!!!
    v=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(v);
    n=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(n);
    if(global_type_vm_func==0){
       p=gettuple_n_envext(get_current_env, n);
    } else if(global_type_vm_func==1){
       p=gettuple_n_envext(get_global_symbol, n);
    };
    set_n_tuplevalue(p,1,v);
    v=pop_n_stack(get_current_stackd);
    n=pop_n_stack(get_current_stackd);
    pool_count_down(v);
    pool_count_down(n);

    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
    // вызовем демона
    // call p
};

void cmd_loadv()
{
    PNODE n, p;
    // возьмем переменную из стека

    n=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(n);
    if(global_type_vm_func==0){
       p=gettuple_n_envext(get_current_env, n);
    } else if(global_type_vm_func==1){
       p=gettuple_n_envext(get_global_symbol, n);
    };
//    print(get_n_tuplevalue(p,1));
    n=pop_n_stack(get_current_stackd);
    push_n_stack(get_current_stackd,get_n_tuplevalue(p,1));
    pool_count_down(n);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
    // вызовем демона
    // call p
};

void cmd_savevl()
{
    PNODE q, p, r;
    s_int n;
/*
    if(global_debug_code==1) {
      stat_info_new();     // выводим статистикику.
    };
*/
    if(global_type_vm_func==0){
       // возьмем второй аргумент
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
       // правильная версия без обнуления счетчика ссылок
       p=get_n_tuplevalue(get_current_locs, n);
       q=get_n_stacksvalue(get_current_stackd,0);
       pool_count_up(q);
       set_n_tuplevalue(p,1,q);
       pop_n_stack(get_current_stackd);
       pool_count_down(q);
    } else if (global_type_vm_func==1) {
       // возьмем второй аргумент
       q=get_n_stacksvalue(get_current_stackd,0);
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
//      win_printf(">>\r\n");
//       print(q);
//      win_printf("end\r\n");
       set_n_stackvalue(get_current_stackd,global_var_point-n,q);
       pop_n_stack(get_current_stackd);
    };
//    pool_count_down(p);

    // возьмем значение переменной из стека данных и сохраним в переменной
//    set_n_tuplevalue(get_n_tuplevalue(get_current_locs, n),1,pop_n_stack(get_current_stackd));
/*
    if(global_debug_code==1) {
      stat_info_new();     // выводим статистикику.
    };
*/
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

void cmd_loadvl()
{
    PNODE q, p, r;
    s_int n;
    if(global_type_vm_func==0){
       // возьмем второй аргумент
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
       // найдем значение переменной и запихаем в стек данных
       push_n_stack(get_current_stackd, get_n_tuplevalue(get_n_tuplevalue(get_current_locs, n),1));
    } else if (global_type_vm_func==1) {
       // возьмем второй аргумент
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
       p=get_n_stackvalue(get_current_stackd,global_var_point-n);
//      win_printf(">>\r\n");
//       print(p);
//      win_printf("end\r\n");
       push_n_stack(get_current_stackd, p);
    };
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

void cmd_saveal()
{
    PNODE p;
    s_int n;
    if(global_type_vm_func==0){
       // возьмем второй аргумент
//       n=(s_int *)((s_int)current_code+current_pc+1);
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
// Исправлено!
       p=get_n_stacksvalue(get_current_stackd,0);
       pool_count_up(p);
       // возьмем значение переменной из стека данных и сохраним в переменной
       set_n_tuplevalue(get_n_tuplevalue(get_current_args, n),1,p);
       pool_count_down(p);
       p=pop_n_stack(get_current_stackd);
    } else if (global_type_vm_func==1) {
       // возьмем второй аргумент
       p=get_n_stacksvalue(get_current_stackd,0);
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
       set_n_stackvalue(get_current_stackd,global_arg_point+n,p);
       pop_n_stack(get_current_stackd);
    };
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

void cmd_loadal()
{
    PNODE p;
    s_int n;
    if(global_type_vm_func==0){
       // возьмем второй аргумент
//       n=(s_int *)((s_int)current_code+current_pc+1);
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
       // найдем значение переменной и запихаем в стек данных
//       print(get_n_tuplevalue(get_n_tuplevalue(get_current_args, n),1));
       push_n_stack(get_current_stackd, get_n_tuplevalue(get_n_tuplevalue(get_current_args, n),1));
//       print(get_n_tuplevalue(get_current_args, *n));
    } else if (global_type_vm_func==1) {
       // возьмем второй аргумент
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
/*
       win_printf("arg\r\n");
       print(get_n_stackvalue(get_current_stackd,global_arg_point+0));
       print(get_n_stackvalue(get_current_stackd,global_arg_point+1));
       print(get_n_stackvalue(get_current_stackd,global_arg_point+2));
       print(get_n_stackvalue(get_current_stackd,global_arg_point+3));
       win_printf("end\r\n");
*/
       p=get_n_stackvalue(get_current_stackd,global_arg_point+n);
//       print(p);
       push_n_stack(get_current_stackd, p);
    };
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

// арифметика
void cmd_add()
{
PNODE p,q;
#if USE_FLOAT
    p=n_add(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd));
#else
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
#endif
    push_n_stack(get_current_stackd,n_arith(p,q,'+'));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_sub()
{
PNODE p,q;
#if USE_FLOAT
    push_n_stack(get_current_stackd,n_sub(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
#else
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_arith(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd),'-'));
#endif
    push_n_stack(get_current_stackd,n_arith(p,q,'-'));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_mul()
{
PNODE p,q;
#if USE_FLOAT
    push_n_stack(get_current_stackd,n_mul(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
#else
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_arith(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd),'*'));
#endif
    push_n_stack(get_current_stackd,n_arith(p,q,'*'));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_div()
{
PNODE p,q;
#if USE_FLOAT
    push_n_stack(get_current_stackd,n_div(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
#else
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_arith(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd),'/'));
#endif
    push_n_stack(get_current_stackd,n_arith(p,q,'/'));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_neg()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_neg(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_neg(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// сравнение (предикаты)
void cmd_equal()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_equal(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_equal(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_neq()
{
PNODE p,q;
#if USE_FLOAT
    push_n_stack(get_current_stackd,n_numeq(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
#else
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_arith(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd),'='));
#endif
    push_n_stack(get_current_stackd,n_arith(p,q,'='));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_ngt()
{
PNODE p,q;
#if USE_FLOAT
    push_n_stack(get_current_stackd,n_numgt(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
#else
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_arith(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd),'>'));
#endif
    push_n_stack(get_current_stackd,n_arith(p,q,'>'));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_nlt()
{
PNODE p,q;
#if USE_FLOAT
    push_n_stack(get_current_stackd,n_numlt(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
#else
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_arith(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd),'<'));
#endif
    push_n_stack(get_current_stackd,n_arith(p,q,'<'));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_nge()
{
//    PNODE p1, p2;
//    p1=pop_n_stack(get_current_stackd);
//    p2=pop_n_stack(get_current_stackd);
PNODE p,q;

#if USE_FLOAT
    push_n_stack(get_current_stackd,n_logand(n_numgt(p1,p2),n_neq(p1,p2)));
#else
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_logand(n_arith(p1,p2,'>'),n_arith(p1,p2,'=')));
#endif
    push_n_stack(get_current_stackd,n_logand(n_arith(p,q,'>'),n_arith(p,q,'=')));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_nle()
{
//    PNODE p1, p2;
//    p1=pop_n_stack(get_current_stackd);
//    p2=pop_n_stack(get_current_stackd);
PNODE p,q;

#if USE_FLOAT
    push_n_stack(get_current_stackd,n_logand(n_numlt(p1,p2),n_neq(p1,p2)));
#else
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_logand(n_arith(p1,p2,'<'),n_arith(p1,p2,'=')));
#endif
    push_n_stack(get_current_stackd,n_logand(n_arith(p,q,'<'),n_arith(p,q,'=')));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_streq()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//  push_n_stack(get_current_stackd,n_stringeq(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_stringeq(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_symeq()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//  push_n_stack(get_current_stackd,n_symboleq(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_symboleq(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_lsteq()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//  push_n_stack(get_current_stackd,n_listeq(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_listeq(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_tpleq()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_tupleeq(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_tupleeq(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// строковые
void cmd_addstr()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_addstring(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_addstring(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_findstr()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_findstring(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_findstring(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_midstr()
{
PNODE p,q,s;
    p=get_n_stacksvalue(get_current_stackd,2);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    s=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(s);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_midstring(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_midstring(p,q,s));
    pool_count_down(p);
    pool_count_down(q);
    pool_count_down(s);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_lenstr()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_lenstring(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_lenstring(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// логические
void cmd_land()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_logand(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_logand(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_lor()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_logor(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_logor(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_lnot()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_lognot(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_lognot(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// прреобразования типов
#ifdef USE_FLOAT
void cmd_lng2flt()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_long_to_float(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_long_to_float(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_flt2lng()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_float_to_long(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_float_to_long(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};
#endif

void cmd_lng2str()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_long_to_string(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_long_to_string(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

#if USE_FLOAT
void cmd_flt2str()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_float_to_string(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_float_to_string(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};
#endif

void cmd_str2lng()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_string_to_long(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_string_to_long(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

#if USE_FLOAT
void cmd_str2flt()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_string_to_float(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_string_to_float(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};
#endif

void cmd_str2sym()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_string_to_symbol(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_string_to_symbol(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_sym2str()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_symbol_to_string(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_symbol_to_string(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// работа со списком
void cmd_cons()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,make_n_list(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,make_n_list(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_car()
{
    PNODE ret;
    ret=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(ret);
    if(is_n_list(ret)) {
      ret=pop_n_stack(get_current_stackd);
      push_n_stack(get_current_stackd,get_n_listcar(ret));
      pool_count_down(ret);
    } else {
      ret=pop_n_stack(get_current_stackd);
      fatal_error(" car!\n");
      push_n_stack(get_current_stackd,NIL);
      pool_count_down(ret);
    };
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_cdr()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,get_n_listcdr(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,get_n_listcdr(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_rplaca()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    set_n_listcar(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd));
    set_n_listcar(p,q);
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_rplacd()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    set_n_listcdr(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd));
    set_n_listcdr(p,q);
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// кортеж
void cmd_mkt()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_tuple(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_tuple(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_sett()
{
PNODE p,q,s;
    p=get_n_stacksvalue(get_current_stackd,2);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(q);
    s=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(s);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    n_settuplevalue(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd));
    n_settuplevalue(p,q,s);
    pool_count_down(p);
    pool_count_down(q);
    pool_count_down(s);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_gett()
{
    PNODE p, l;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    l=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(l);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_gettuplevalue(l,p));
    push_n_stack(get_current_stackd,n_gettuplevalue(l,p));
    pool_count_down(p);
    pool_count_down(l);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_resizet()
{
PNODE p,q;
    p=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(p);
    q=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(q);
    pop_n_stack(get_current_stackd);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_resizetuple(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_resizetuple(p,q));
    pool_count_down(p);
    pool_count_down(q);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_lent()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd,n_lentuple(pop_n_stack(get_current_stackd)));
    push_n_stack(get_current_stackd,n_lentuple(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// фреймы
void cmd_mkfr()
{
PNODE p;
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
    push_n_stack(get_current_stackd,n_frame(p));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_cpfr()
{
    push_n_stack(get_current_stackd,n_frame_from(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_addfrf()
{
    n_add_frame_slot(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd));
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_delfrf()
{
//    n_del_frame_slot(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd));
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_savefrf()
{
    n_set_framevalue(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd));
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_loadfrf()
{
    push_n_stack(get_current_stackd,n_get_framevalue(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd)));
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_callfrf()
{
    PNODE f;

    f=n_get_framevalue(pop_n_stack(get_current_stackd),pop_n_stack(get_current_stackd));
    n_call_func(f,1);
};

// функция
void cmd_getf()
{
    PNODE f, p;

    // загрузим имя функции
    f=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(f);
    p=find_func_name(f);
    f=pop_n_stack(get_current_stackd);
    push_n_stack(get_current_stackd, p);
    pool_count_down(f);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// типы
void cmd_tatom()
{
    PNODE p;
    // загрузим атом
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    p=pop_n_stack(get_current_stackd);
//    push_n_stack(get_current_stackd, make_n_long(get_n_type(p)));
    push_n_stack(get_current_stackd, make_n_long(get_n_type(p)));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_statom()
{
    PNODE p;
    s_int st;
    // загрузим атом
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    pop_n_stack(get_current_stackd);
    st=get_n_type(p);
    st=(( st & MASK_SUBTYPE) >> SHIFT_SUBTYPE); //get_n_stype(p);
    st=1;
    push_n_stack(get_current_stackd, make_n_long(st));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

// прерывания
void cmd_setif()
{
    PNODE p, v;
    s_int n;
    // возьмем второй аргумент
    if(global_type_vm_func==0){
       n=(s_int)((s_int)current_code+current_pc+1);
    } else if(global_type_vm_func==1){
       n=(s_int)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1);
    };
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    p=pop_n_stack(get_current_stackd);
    // возьмем значение из стека данных и сохраним
/*
    if(set_n_interruptn(get_global_inter, *n, p)==NIL) {
       add_n_interruptl(get_global_inter, *n, p);
    };
*/
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

void cmd_setifn()
{
    PNODE p, v;

    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    v=get_n_stacksvalue(get_current_stackd,1);
    pool_count_up(v);
    p=pop_n_stack(get_current_stackd);
    v=pop_n_stack(get_current_stackd);
    // возьмем значение из стека данных и сохраним
/*
    if(set_n_interruptn(get_global_inter, get_n_longvalue(v), p)==NIL) {
       add_n_interruptl(get_global_inter, get_n_longvalue(v), p);
    };
*/
    pool_count_down(p);
    pool_count_down(v);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_getif()
{
    PNODE p;
    s_int n;
    // возьмем второй аргумент
    if(global_type_vm_func==0){
       n=(s_int )((s_int)current_code+current_pc+1);
    } else if(global_type_vm_func==1){
       n=(s_int)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1);
    };
    p=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(p);
    // найдем значение переменной и запихаем в стек данных
/*
    push_n_stack(get_current_stackd, get_n_interruptn(get_global_inter, *n));
*/
//    print(get_n_tuplevalue(get_current_args, *n));
    pool_count_down(p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

void cmd_calli()
{
    s_int n;
    PNODE f;

    // возьмем второй аргумент
    if(global_type_vm_func==0){
       n=(s_int)((s_int)current_code+current_pc+1);
    } else if(global_type_vm_func==1){
       n=(s_int)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1);
    };
    // загрузим имя и найдем функцию
/*
    f=get_n_interruptn(get_global_inter, *n);
*/
//    print(f);
    n_call_func(f,0);
};

// отдадочные
void cmd_break()
{
    s_int n;
    // возьмем второй аргумент
    if(global_type_vm_func==0){
       n=(s_int)((s_int)current_code+current_pc+1);
    } else if(global_type_vm_func==1){
       n=(s_int)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1);
    };
//    win_printf("Break %ld\n",*n);
    n_break(n);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

// управляющие и специальные
void cmd_enter()
{
    push_n_stack(get_current_stackd,SLABEL);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_clear()
{
    PNODE p;
    s_int n;
    int l;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // найдем значение константы и очистим стек до метки
    if(global_type_vm_func==0){
       p=get_n_tuplevalue(get_current_const, n);
    } else if(global_type_vm_func==1){
       p=get_n_stackvalue(get_current_stackd,global_const_point-n);
    };                     

//    print(p);
    l=0;
//    win_printf("->>>>>>>>>>>>>> %d %d\r\n",get_n_stacksvalue(get_current_stackd,l),p);
//    print(get_n_stacksvalue(get_current_stackd,l));
    while(get_n_stacksvalue(get_current_stackd,l)!=p) {
//       print(get_n_stacksvalue(get_current_stackd,l));
       l++;
    };
//    l++;
    if(l!=0) {
       // очистим стек
       while(l>=0) {
          p=pop_n_stack(get_current_stackd);
//    print(p);
          l--;
       };
    } else {
    p=pop_n_stack(get_current_stackd);
//    win_printf("->>>>>>>>>>>>>> ");
//    print(p);
    };
/*
    } else {
       fatal_error(" Нарушена синхронность стека данных!\n");
       global_end_exec=3;
    };
*/
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

/*
void cmd_call1()
{
    s_int *n, l;
    PNODE p, f, c, ret=NIL;
    int (*fcommand)();
    s_int k;
    int m;

    // возьмем второй аргумент
    n=(s_int *)((s_int)current_code+current_pc+1);
    // сохраним текущее замыкание в стеке
    push_n_stack(get_current_stackd, global_frame.closure);
    // сохраним в стеке точку возврата
    current_pc=current_pc+5;
    push_n_stackp(get_current_stackc, current_pc);
    // загрузим имя и найдем функцию
    f=get_n_tuplevalue(get_current_funcs, *n);
    p=get_n_tuplevalue(f,1);
    if(is_n_func(p)) {
        // вызов демона а потом вызов того чего надо
        //
        // создадим замыкание
        c=make_n_closurefunc(p);
        unpack_closure(c);
        // загрузка аргументов из стека
        load_funcarg();
        // устанавливаем счетчик команд на 0
        set_current_closurecounter(0);
    } else {
        if(is_n_long(p)) {
             // загрузка встроенной функции или из библиотеки
             fcommand=(void *)get_n_longvalue(p);
             m=(*fcommand)();
             // очистим стек до уровня метки и считаем сохраненую функцию
             l=0;
             while((p=get_n_stacksvalue(get_current_stackd,l))!=SLabel) {
                  if(l==0) ret=p;
   //               if(p==0) break; ?????????????????????
                  l++;
             };
             // l==0 -> приводит к ошибке
             if(l<2) ret=nil;
             if(l!=0) {
                  p=get_n_stacksvalue(get_current_stackd,l-1);
                  // очистим стек
                  while(l>=0) {
                        pop_n_stack(get_current_stackd);
                        l--;
                  };
             } else {
                  fatal_error(" Нарушена синхронность стека данных!\n");
                  global_end_exec=3;
             };
             // вернем значение !!!
             if(m!=0) push_n_stack(get_current_stackd,ret);
             k=pop_n_stackp(get_current_stackc);
            } else {
                 fatal_error(" функция %s неопределена!\n",get_n_symbolvalue(get_n_tuplevalue(f,0)));
                 global_end_exec=4;
            };
    };
};
*/

void cmd_call()
{
    s_int n;
    PNODE f;

    // возьмем второй аргумент
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
       // загрузим имя и найдем функцию
       f=get_n_tuplevalue(get_current_funcs, n);
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
       f=get_n_stackvalue(get_current_stackd,global_func_point-n);
    };
//    print(f);
    n_call_func(f,0);
};

void cmd_callp()
{
    PNODE f;

    // загрузим функцию
    f=pop_n_stack(get_current_stackd);
    pool_count_up(f);
//    print(f);
    n_call_func(f,1);
    pool_count_down(f);
};

void cmd_callv()
{
    PNODE p, f;

    // загрузим из стека имя и найдем функцию
    f=get_n_stacksvalue(get_current_stackd,0);
    pool_count_up(f);
    f=pop_n_stack(get_current_stackd);
//    if((p=find_func_name(f))!=NIL) {
    if((p=find_func_s(f))!=NIL) {
        n_call_func(p,1);
        pool_count_down(f);
    } else {
        pool_count_down(f);
        fatal_error(" функция %s неопределена!\n",get_n_symbolvalue(f));
        global_end_exec=4;
    };
};

void cmd_jz()
{
    s_int n, l, f;
#if USE_FLOAT
    double d;
#endif
    PNODE p;

    // возьмем второй аргумент
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    f=0;
    if(is_n_numeric(p)) {
#ifdef USE_FLOAT
       if(is_n_long(p)) {
           l=get_n_longvalue(p);
           if(l==0) f=1;
       } else {
           d=get_n_floatvalue(p);
           if(d==0.0) f=1;
       };
#else
       l=get_n_longvalue(p);
       if(l==0) f=1;
#endif
    };
    if(f==1) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

void cmd_jnz()
{
    s_int n, l, f;
#if USE_FLOAT
    double d;
#endif
    PNODE p;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    f=0;
    if(is_n_numeric(p)) {
#if USE_FLOAT
       if(is_n_long(p)) {
           l=get_n_longvalue(p);
           if(l==0) f=1;
       } else {
           d=get_n_floatvalue(p);
           if(d==0.0) f=1;
       };
#else
       l=get_n_longvalue(p);
       if(l==0) f=1;
#endif
    };
    if(f==0) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

void cmd_jnil()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
//    n=(s_int *)((s_int)current_code+current_pc+1);
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    if(p==NIL) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

void cmd_jnnil()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    if(p!=NIL) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

void cmd_jfalse()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
//    n=(s_int *)((s_int)current_code+current_pc+1);
    // получим элемент с вершины аргумент для проверки
//   print(get_n_stacksvalue(get_current_stackd,0));
//   print(get_n_stacksvalue(get_current_stackd,1));
//   print(get_n_stacksvalue(get_current_stackd,2));
    p=get_n_stacksvalue(get_current_stackd,0);

    if(p==FALSE) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

void cmd_jnfalse()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    if(p!=FALSE) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

void cmd_jlng()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    if(is_n_long(p)) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

#if USE_FLOAT
void cmd_jfloat()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((s_int)current_code+current_pc+1);
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    if(is_n_float(p)) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};
#endif

void cmd_jstring()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    if(is_n_string(p)) {
    if(global_type_vm_func==0){
       set_current_closurecounter(n);
    } else if(global_type_vm_func==1){
       global_pc_counter=n;
    };
    } else {
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
    };
};

void cmd_jsymbol()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    if(is_n_symbol(p)) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

void cmd_jtuple()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    if(is_n_tuple(p)) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

void cmd_jlist()
{
    s_int n;
    PNODE p;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
    n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины аргумент для проверки
    p=get_n_stacksvalue(get_current_stackd,0);
    if(is_n_list(p)) {
      if(global_type_vm_func==0){
         set_current_closurecounter(n);
      } else if(global_type_vm_func==1){
         global_pc_counter=n;
      };
    } else {
      if(global_type_vm_func==0){
         set_current_closurecounter((s_int)current_pc+5);
      } else if(global_type_vm_func==1){
         global_pc_counter=global_pc_counter+5;
      };
    };
};

void cmd_jmp()
{
    s_int n;

    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    if(global_type_vm_func==0){
       set_current_closurecounter(n);
    } else if(global_type_vm_func==1){
       global_pc_counter=n;
    };
};

void cmd_end()
{
//    s_int n;

   win_printf("Окончание работы.\n",current_pc);
   exit_vm(0);
};

void cmd_dup() // копирование вершины
{
    PNODE p;

    // получим элемент с вершины
    p=get_n_stackvalue(get_current_stackd,0);
    push_n_stack(get_current_stackd,p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+1);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+1;
    };
};

void cmd_cpa() // копирование n-го аргумента
{
    PNODE p;
    s_int n;
    // возьмем второй аргумент
//    n=(s_int *)((s_int)current_code+current_pc+1);
    if(global_type_vm_func==0){
       n=get_long_value((char *)((s_int)current_code+current_pc+1));
    } else if(global_type_vm_func==1){
       n=get_long_value((char *)((s_int)get_n_stringvalue(global_current_command_string)+global_pc_counter+1));
    };
    // получим элемент с вершины
    p=get_n_stacksvalue(get_current_stackd,n);
    push_n_stack(get_current_stackd,p);
    if(global_type_vm_func==0){
       set_current_closurecounter((s_int)current_pc+5);
    } else if(global_type_vm_func==1){
       global_pc_counter=global_pc_counter+5;
    };
};

void cmd_ret()
{
    PNODE p, ret=NIL, c;
    s_int l, st, m;

//    print_n_stackvalue(get_current_stackd);

    current_ret=NIL;   // ! сохраним значение!!!!
//    win_printf("ret\r\n");
    // очистим стек до уровня метки и считаем сохраненую функцию
    l=0;
//    print(get_n_stacksvalue(get_current_stackd,l));
    while((p=get_n_stacksvalue(get_current_stackd,l))!=SLABEL) {
//    print(p);
         if(l==0)     ret=p;
         l++;
    };
    // l==0 -> приводит к ошибке
    m=l;
    if(l<1) {   // path 2
       ret=NIL;
    } else {
       pool_count_up(ret);
//       print(ret);
    };
    l++;
/*
    if(global_type_vm_func==1){
      c=get_n_stackvalue(get_current_stackd,global_prev_point);
      global_prev_func=c;
    };
*/
    if(l!=0) {
        // очистим стек
        while(l!=0) {
            pop_n_stack(get_current_stackd);
            l--;
        };
    } else {
        fatal_error("\r\nНарушена синхронность стека данных (ret) !\n");
        global_end_exec=3;
    };
//    if(global_prev_func_p!=global_prev_func){
//      print(global_prev_func);
//    };
    // вернем значение !!!
    if(m>=1) {    // path 2
       current_ret=ret;   // ! сохраним значение!!!!
       push_n_stack(get_current_stackd,ret);
       pool_count_down(ret);
    };
    // восстановим старые значения
    if(global_type_vm_func==0){
      // старое значение надо УНИЧТОЖИТЬ!!!!
      p=get_n_tuplevalue(get_global_streamclosure,global_stream_closure);
      c=get_n_closureprev(get_n_tuplevalue(get_global_streamclosure,global_stream_closure));
      // ?????????????????????????????????
      //    Почему ошибка?
      if(c==NIL) {
          win_printf("\r\nВозврат на верхний уровень!\r\n");
          call_exit_hook();
          global_end_exec=2;
          return;
      };

      // делаем текущей
      set_n_tuplevalue(get_global_streamclosure,global_stream_closure,c);

      pool_count_down(p);
    } else if(global_type_vm_func==1){
      if(global_prev_func==NIL) {
          win_printf("\r\nВозврат на верхний уровень!\r\n");
          call_exit_hook();
          global_end_exec=2;
          return;
      };
      // восстанавливаем из стекавозвратов
      global_pc_counter=pop_from_bstack();
      global_func_point=pop_from_bstack();
      global_const_point=pop_from_bstack();
      global_var_point=pop_from_bstack();
      global_prev_point=pop_from_bstack();
      global_arg_point=pop_from_bstack();
      p=get_n_stackvalue(get_current_stackd,global_prev_point-1);
      c=get_n_stackvalue(get_current_stackd,global_prev_point);
      global_prev_func=c;
      global_current_command_string=get_n_funccode(p);
    };
//    print_n_stackvalue(get_current_stackd);

    // загрузка аргументов не требуется, так как все окружение содержится в замыкании
    // устанавливаем счетчик команд
};
