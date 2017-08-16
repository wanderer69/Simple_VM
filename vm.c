//---------------------------------------------------------------------------
#define ARCHITECT16 1

#include <stdlib.h>
#include <string.h>
#ifdef __BORLANDC__
#include <conio.h>
#endif
//PNODE make_n_envext();
void load_command_array();

#include "vm.h"
#include "MEMMAN.h"
#include "objects.h"
#define VM_CODE 1
#include "COMCODE.H"
#include "print.h"
#include "READ.H"
#include "win_printf.h"

#ifdef __BORLANDC__
#include <windows.h>
//#include <dos.h>
#endif

#ifndef __BORLANDC__
#ifndef __LINUX
#define printf uart0Puts
#endif
#endif

//#include "../em/Ext_Mod/recieve_packet.h"
extern int flag_settings;
void GunReturn(void);

//#pragma hdrstop

void add_critreg(PNODE p,int a)
{
  set_n_tuplevalue(CRIT_REG,a,p);
};

void free_critreg(int a)
{
  set_n_tuplevalue(CRIT_REG,a,NIL);
};

void free_all_critreg(void)
{
int i;
  for(i=0;i<5;i++){
  set_n_tuplevalue(CRIT_REG,i,NIL);
  };
};

//---------------------------------------------------------------------------
//#pragma package(smart_init)
/* environvent ext
в отличие от простого окружения расширенное окружение представляет
собой список кортежей в которых нулевой элемент представляет собой ключ,
первый элемент - значение ключа, второй элемент - ссылка на функцию-демон чтения в
из переменной, третий - ссылка на функцию-демон записи в переменную, четвертый -  ссылка на
функцию-демон вызова функции.

В вершине расширенного окружения стоит ячейка в голове у которой находится
 указатель на список кортежей, а хвостом является хэш-массив или в общем
случае стоит nil.
*/

PNODE make_n_envext(void)
{
 PNODE p;
     p=make_n_list(NIL,NIL);
//     set_n_stype(p,NENVES);
     return(p);
};

//#define _DEBUG 1
// добавить
PNODE add_n_envext(PNODE p, PNODE k, PNODE v)
{
 PNODE n, s;
 PNODE m, t;

#ifdef _DEBUG
    printf("\r\nadd_n_envext begin\r\n");
#endif
     pool_count_up(k);
//     printf("1\n");
     pool_count_up(v);
//     printf("2\n");
//     add_critreg(k,0);
//     add_critreg(v,1);

     n=make_n_tuple(2);
//     printf("3\n");
     fill_n_tuple(n, NIL);
//     printf("4\n");
     pool_count_up(n);
//     printf("5\n");
//     add_critreg(n,2);
     set_n_tuplevalue(n,0,k);
//     printf("6\n");
     set_n_tuplevalue(n,1,v);
//     printf("7\n");
     pool_count_down(k);
//     printf("8\n");
     pool_count_down(v);
//     printf("9\n");
//     print(p);
     t=get_n_listcar(p);
//     printf("10\n");
//     print(n);
//     print(t);
     s=make_n_list(n,NIL);
//     printf("11\n");
//     print(s);
     set_n_listcdr(s,t);
//     printf("12\n");
//     print(s);
//     s=make_n_list(n,get_n_listcar(p));
//     print(s);
     set_n_listcar(p,s);
//     print(p);
//     printf("13\n");
     pool_count_down(n);
//     printf("14\n");
//     free_all_critreg();
#ifdef _DEBUG
    printf("add_n_envext end\r\n");
#endif
     return(n);
};

// найти в словаре символьное сочетание
char* find_n_envexts(PNODE p, int k)
{
 PNODE n, s, q;
 char *c;
 int i, j=0;

#ifdef _DEBUG
 printf("find_n_envexts begin  ");
#endif

 n=get_n_listcar(p);
 c=0L;
 while(n!=NIL) {
    s=get_n_listcar(n);
    q=get_n_tuplevalue(s,0);
    if(j==k) {
	c=get_n_symbolvalue(q);
        break;
    } else {
        n=get_n_listcdr(n);
      j++;
    };
 };
#ifdef _DEBUG
 printf("get_n_envexts end\r\n");
#endif
 return(c);
};

// найти в словаре символьное сочетание
PNODE get_n_envexts(PNODE p, char *k)
{
 PNODE n, s, q;
 char *c;
 char b[100];
 int i;

#ifdef _DEBUG
 printf("get_n_envexts begin  ");
#endif
// print(p);
 n=get_n_listcar(p);

// printf("1 ");

 while(n!=NIL) {
    s=get_n_listcar(n);
// printf("2 ");
// win_printf("\r\n>>>>>>>>>>>>> ");
//    print(s);
    q=get_n_tuplevalue(s,0);
//    print(q);
    c=get_n_symbolvalue(q);
// printf("3 ");

// sprintf(b," (%x %x %x %x %x %x %x %x %x = %x %x %x %x %x %x %x %x %x) ",c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7],c[8],k[0],k[1],k[2],k[3],k[4],k[5],k[6],k[7],k[8]);

// printf(b);
//sprintf(b," (%x %x)\r\n",c,k);

// printf(b);

    i=strcmp(c,k);
//sprintf(b," (%x)\r\n",i);

// printf(b);
    if(i==0) {
// printf("get_n_envexts end\r\n");
        return(get_n_tuplevalue(s,1));
    } else {
// printf("5 ");
        n=get_n_listcdr(n);
    };
// printf("6 ");
 };
#ifdef _DEBUG
 printf("get_n_envexts end\r\n");
#endif
 return(0);
};

// найти в словаре символьное сочетание
PNODE gettuple_n_envexts(PNODE p, char *k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
//    print(get_n_tuplevalue(s,0));
//    print(get_n_symbolvalue(get_n_tuplevalue(s,0)));
    if(strcmp(get_n_symbolvalue(get_n_tuplevalue(s,0)),k)==0) {
        return(s);
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(0);
};

// найти значение
PNODE get_n_envext(PNODE p, PNODE k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
    if(get_n_tuplevalue(s,0)==k) {
        return(get_n_tuplevalue(s,1));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(NIL);
};

// найти значение
PNODE gettuple_n_envext(PNODE p, PNODE k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
//  print(get_n_tuplevalue(s,0));
    if(get_n_tuplevalue(s,0)==k) {
        return(s);
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(NIL);
};

// присвоить
PNODE set_n_envext(PNODE p, PNODE k, PNODE v)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
    if(get_n_tuplevalue(s,0)==k) {
//  print(get_n_tuplevalue(s,0));
        set_n_tuplevalue(s,1,v);
        return(get_n_tuplevalue(s,0));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(NIL);
};

/*
// добавить демон чтения
PNODE set_n_envextreaddemon(PNODE p, PNODE k, PNODE d)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
    if(get_n_tuplevalue(s,0)==k) {
        if(get_n_tuplelen(s)<3) {
            s=resize_n_tuple(s,3);
            set_n_listcar(n,s);
        };
        set_n_tuplevalue(s,2,d);
        return(s);
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(NIL);
};

PNODE get_n_envextreaddemon(PNODE p, PNODE k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
    if(get_n_tuplevalue(s,0)==k) {
        return(get_n_tuplevalue(s,2));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(NIL);
};

// добавить демон записи
PNODE set_n_envextwritedemon(PNODE p, PNODE k, PNODE d)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
    if(get_n_tuplevalue(s,0)==k) {
        if(get_n_tuplelen(s)<4) {
            s=resize_n_tuple(s,4);
            set_n_listcar(n,s);
        };
        set_n_tuplevalue(s,3,d);
        return(s);
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(NIL);
};

PNODE get_n_envextwritedemon(PNODE p, PNODE k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
    if(get_n_tuplevalue(s,0)==k) {
        return(get_n_tuplevalue(s,3));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(NIL);
};

// добавить демон вызова
PNODE set_n_envextcalldemon(PNODE p, PNODE k, PNODE d)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
    if(get_n_tuplevalue(s,0)==k) {
        if(get_n_tuplelen(s)<5) {
            s=resize_n_tuple(s,5);
            set_n_listcar(n,s);
        };
        set_n_tuplevalue(s,4,d);
        return(s);
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(NIL);
};

PNODE get_n_envextcalldemon(PNODE p, PNODE k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=NIL) {
    s=get_n_listcar(n);
    if(get_n_tuplevalue(s,0)==k) {
        return(get_n_tuplevalue(s,4));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(NIL);
};
*/
// удалить из вершины
void del_n_envext(PNODE p)
{
 PNODE n;
     n=get_n_listcar(p);
     set_n_listcar(p,get_n_listcar(n));
};

/*
// interrupt
// объект окружение со специфическим типом ключа (целое) и
// функцией в качестве значения
PNODE make_n_interrupt()
{
 PNODE p;
     p=make_n_env();
     return(p);
}

// добавить
PNODE add_n_interruptl(PNODE p, long k, PNODE v)
{
 PNODE n, s, l;
 long lp, lv;
     lp=add_critreg(p);
     lv=add_critreg(v);
     l=make_n_long(k);
     v=get_critreg(lv);
     n=make_n_list(l,v);
     set_critreg(lv,n);
     p=get_critreg(lp);
     s=make_n_list(n,get_n_listcar(p));
     v=get_critreg(lv);
     p=del_critreg(lp);
     set_n_listcar(p,s);
     return(n);
};

// найти в словаре прерывание
PNODE get_n_interruptl(PNODE p, long k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=nil) {
    s=get_n_listcar(n);
    if(get_n_longvalue(get_n_listcar(s))==k) {
        return(get_n_listcdr(s));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(0);
};

// найти в словаре символьное сочетание
PNODE getlist_n_interruptl(PNODE p, long k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=nil) {
    s=get_n_listcar(n);
    if(get_n_longvalue(get_n_listcar(s))==k) {
        return(s);
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(0);
};

// найти
PNODE get_n_interrupt(PNODE p, PNODE k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=nil) {
    s=get_n_listcar(n);
    if(get_n_longvalue(get_n_listcar(s))==get_n_longvalue(k)) {
        return(get_n_listcdr(s));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(nil);
};

PNODE get_n_interruptn(PNODE p, long k)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=nil) {
    s=get_n_listcar(n);
    if(get_n_longvalue(get_n_listcar(s))==k) {
        return(get_n_listcdr(s));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(nil);
};

// присвоить
PNODE set_n_interrupt(PNODE p, PNODE k, PNODE v)
{
 PNODE n, s;
 n=get_n_listcar(p);
 while(n!=nil) {
    s=get_n_listcar(n);
    if(get_n_longvalue(get_n_listcar(s))==get_n_longvalue(k)) {
        set_n_listcdr(s,v);
        return(get_n_listcar(s));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(nil);
};

PNODE set_n_interruptn(PNODE p, long k, PNODE v)
{
 PNODE n, s;
 n=get_n_listcar(p);
// print(n);
 while(n!=nil) {
    s=get_n_listcar(n);
//    win_printf("> %d ",k); print(get_n_listcar(s));
    if(get_n_longvalue(get_n_listcar(s))==k) {
        set_n_listcdr(s,v);
        return(get_n_listcar(s));
    } else {
        n=get_n_listcdr(n);
    };
 };
 return(nil);
};

// удалить из вершины
void del_n_interrupt(PNODE p)
{
 PNODE n;
     n=get_n_listcar(p);
     set_n_listcar(p,get_n_listcar(n));
};
*/

// function
PNODE make_n_func(void)
{
  PNODE ret;
  ret=make_n_tuple(7);
  fill_n_tuple(ret, NIL);
  set_n_nodetype(ret, N_FUNC);

  return(ret);
};

// closure
PNODE make_n_closure(void)
{
  PNODE ret;
  ret=make_n_tuple(12);
  fill_n_tuple(ret, NIL);
  set_n_nodetype(ret, N_CLOS);

/*
  set_n_tuplevalue(ret,0,nil); // const
  set_n_tuplevalue(ret,1,nil); // arg
  set_n_tuplevalue(ret,2,nil); // loc
  set_n_tuplevalue(ret,3,nil); // env
  set_n_tuplevalue(ret,4,nil); // func
  set_n_tuplevalue(ret,5,nil); // code
  set_n_tuplevalue(ret,6,nil); // stackc
  set_n_tuplevalue(ret,7,nil); // stackd
  set_n_tuplevalue(ret,8,nil); // extenv
  set_n_tuplevalue(ret,9,nil); // module name (for debug)
  set_n_tuplevalue(ret,10,nil); // previous closure
  set_n_tuplevalue(ret,11,nil); // counter
*/

  return(ret);
};

// совсем новый вариант. Размещаем вызов функции в стеке
// в этом варианте переменные не имеют связи со своим именем.
int global_arg_point; // точка от которой отсчитываются аргументы
int global_var_point; // точка от которой отсчитываются переменные
PNODE global_current_command_string; // текущая строка кода
int global_pc_counter; // положение
int global_const_point; // точка от которой отсчитываются константы
int global_func_point; // точка от которой отсчитывают список функций
int global_prev_point; // точка в которой лежит предыдушая функция если 0 то значит это стартовая функция
PNODE global_prev_func; // предыдушая функция

PNODE global_prev_func_p; // предыдушая функция
PNODE global_current_func; // текущая функция

// Для отладки создаем псевдозамыкание

// из структуры функции копируются в стек строка кода, затем имя модуля для
// отладки, потом переменные
void copy_func_to_stack(PNODE f)
{
int len_arg, arg_in_stack, len_con, len_func, len_arg1, len_arg2;
PNODE arg, var, con, func, s, c;
int len_var;
int i;
char *sp;
   // во первых надо проверить количество аргументов в стеке
   arg=get_n_funcarg(f);
   len_arg=get_n_tuplelen(arg);
   arg_in_stack=0;
// закомментарил блок!!!
//   win_printf(">>\r\n");
   while(get_n_stacksvalue(get_current_stackd,arg_in_stack)!=SLABEL) {
//		 print(get_n_stacksvalue(get_current_stackd,arg_in_stack));
		 arg_in_stack++;
   };
//   win_printf("end\r\n");

   // проверим, последняя переменная в кортеже системная?
   len_arg2=0;
   if(len_arg>0){
		s=get_n_tuplevalue(arg,len_arg-1);
		sp=get_n_stringvalue(get_n_tuplevalue(s,0));
//		win_printf(">> %s\n",sp);
		if(strcmp(sp,"_sys_arg_list")==0) len_arg2=1;
   };
   // если надо, добавляем
   if(arg_in_stack<len_arg){
	   for(i=len_arg-1;i>=arg_in_stack;i--){
		 s=get_n_tuplevalue(arg,i);
//         print(s);
		 if(is_n_tuple(s)) {
		   push_n_stack(get_current_stackd,get_n_tuplevalue(s,1));
		 } else {
		   push_n_stack(get_current_stackd,NIL);
		 };
	   };
   };
   if((len_arg>0) && ((arg_in_stack>len_arg) || (len_arg2))){
	 // копируем остаток в последнюю переменную кортеж
	 len_arg1=arg_in_stack-(len_arg-1);
	 c=make_n_tuple(len_arg1);
	 set_n_tuplevalue(c,0,NIL);
	 for(i=len_arg1;i>=1;i--){
		s=pop_n_stack(get_current_stackd);
//        print(s);
		set_n_tuplevalue(c,i-1,s);
	 };
//     print(c);
	 push_n_stack(get_current_stackd,c);
   };
   // запоминаем положение стека
   global_arg_point=get_n_stackptr(get_current_stackd)-len_arg;
/*
   if(arg_in_stack<=len_arg){
     global_arg_point=get_n_stackptr(get_current_stackd)-len_arg;
   } else {
     global_arg_point=get_n_stackptr(get_current_stackd)-arg_in_stack;
   };
*/
//   global_arg_point=get_n_stackptr(get_current_stackd)-len_arg;
   // копируем саму функцию
   push_n_stack(get_current_stackd,f);
   // копируем предыдущую функцию
   global_prev_func_p=global_prev_func;
   global_current_func=f;
   push_n_stack(get_current_stackd,global_prev_func);
   global_prev_point=get_n_stackptr(get_current_stackd)-1;
   // копируем переменные в стек
   var=get_n_funcloc(f);
   len_var=get_n_tuplelen(var);
   for(i=len_var-1;i>=0;i--){
       s=get_n_tuplevalue(var,i);
       if(is_n_tuple(s)) {
           push_n_stack(get_current_stackd,get_n_tuplevalue(s,1));
//           push_n_stack(get_current_stackd,s);
       } else {
           push_n_stack(get_current_stackd,NIL);
       };
   };
   global_var_point=get_n_stackptr(get_current_stackd)-1;
   con=get_n_funcconst(f);
   len_con=get_n_tuplelen(con);
//   win_printf("Const\r\n");
   for(i=len_con-1;i>=0;i--){
       s=get_n_tuplevalue(con,i);
//       print(s);
       push_n_stack(get_current_stackd,s);
   };
//   win_printf("End\r\n");
   global_const_point=get_n_stackptr(get_current_stackd)-1;
//   win_printf("Const ptr %d\r\n",global_const_point);
   func=get_n_funcfunc(f);
   len_func=get_n_tuplelen(func);
   for(i=len_func-1;i>=0;i--){
       s=get_n_tuplevalue(func,i);
       push_n_stack(get_current_stackd,s);
   };
   global_func_point=get_n_stackptr(get_current_stackd)-1;
   // первый запуск - в начале должен стоять код SLABEL
   global_current_command_string= get_n_funccode(f);
};

// новый вариант - порождение замыкания из функции
PNODE make_n_closurefunc(PNODE f, PNODE curr_closure)
{
  PNODE c, p, s, n, pa, pl, pe, pe1, ee;
  long l, i, lf, lp, lpa, lc, lpe;

  c=make_n_tuple(12);
  pool_count_up(c);
  fill_n_tuple(c, NIL);
  set_n_nodetype(c, N_CLOS);
  add_critreg(c,0);

  // копируем окружение в окружение с созданием кортежа аргументов и кортежа переменных
  // копируем аргументы
  p=get_n_funcarg(f);

  l=get_n_tuplelen(p);
  pa=make_n_tuple(l);
  fill_n_tuple(pa, NIL);
  pool_count_up(pa);
  add_critreg(pa,2);

  // окружение порождается из ячейки где хранится указатель на глобальное окружение текущего !!! фрейма
  ee=get_n_closureextenv(curr_closure);
  if (ee==NIL) {
    pe=NIL;
  } else {
    pe=get_n_listcar(ee);
  };
  pool_count_up(pe);
  add_critreg(pe,3);

  for(i=0;i<l;i++) {
     s=get_n_tuplevalue(p,i);
     if(is_n_tuple(s)) { // кортеж??
        n=copy_n_tuple(s);
        set_n_tuplevalue(pa,i,n);
        if(pe==NIL) {
           pe=make_n_list(n,NIL);
        } else {
           pe=make_n_list(n,pe);
        };
        add_critreg(pe,3);
//        pe1=pe;
//        pool_count_up(pe);
//        set_critreg(lpe,pe);
     };
  };

//  lpa=add_critreg(pa);

  // копируем локальные переменные
  p=get_n_funcloc(f);
  l=get_n_tuplelen(p);

  pl=make_n_tuple(l);
  fill_n_tuple(pl, NIL);
  pool_count_up(pl);
  add_critreg(pl,4);

  for(i=0;i<l;i++) {
     s=get_n_tuplevalue(p,i);
     if(is_n_tuple(s)) {
        n=copy_n_tuple(s);
        set_n_tuplevalue(pl,i,n);
        if(pe==NIL) {
           pe=make_n_list(n,NIL);
        } else {
           pe=make_n_list(n,pe);
//           pool_count_down(pe1);
        };
        add_critreg(pe,3);
//        pe1=pe;
//        pool_count_up(pe);
//        set_critreg(lpe,pe);
     };
  };
  // доделаем окружение
  pe=make_n_list(pe,NIL);

  add_critreg(p,1);

//  set_n_stype(pe,NENVES);

//  f=del_critreg(lf);  // уничтожим
  // установим соответствующие параметры
  set_n_closureconst(c,get_n_funcconst(f));
  set_n_closurearg(c,pa);
  pool_count_down(pa);
  set_n_closureloc(c,pl);
  pool_count_down(pl);
  set_n_closureenv(c,pe);
  set_n_closurefunc(c,get_n_funcfunc(f));
  set_n_closurecode(c,get_n_funccode(f));
  set_n_tuplevalue(c,6,NIL);
  set_n_tuplevalue(c,7,NIL);
  set_n_tuplevalue(c,8,pe);
  set_n_tuplevalue(c,9,get_n_funcmname(f));
  set_n_tuplevalue(c,10,curr_closure);
  set_n_tuplevalue(c,11,make_n_long(0));
  free_all_critreg();
  return(c);
};

// стартовый вариант!!!
PNODE make_n_closurefuncn(PNODE f)
{
  PNODE c, p, s, n, pa, pl, pe;
  s_int l, i, lf, lp, lpa;

//  c=make_n_tuple(10);
//  f=get_critreg(lf);
//  set_n_stype(c,NCLOSURES);

  c=make_n_tuple(12);
  pool_count_up(c);
  fill_n_tuple(c, NIL);
  set_n_nodetype(c, N_CLOS);

  // копируем окружение в окружение с созданием кортежа аргументов и кортежа переменных
  // копируем аргументы
  p=get_n_funcarg(f);
  l=get_n_tuplelen(p);
  pa=make_n_tuple(l);
  fill_n_tuple(pa, NIL);
  pool_count_up(pa);

  // окружение порождается из глобального окружения
  pe=get_n_listcar(get_global_symbol);
//  push_n_stack(get_current_stackd,f);
  for(i=0;i<l;i++) {
//     p=get_critreg(lp);
     s=get_n_tuplevalue(p,i);
     if(is_n_tuple(s)) { // кортеж??
//        add_critreg(pe);
//        add_critreg(pa);
        n=copy_n_tuple(s);
//        pa=get_critreg(CR_LAST);
//        pe=get_critreg(CR_LAST);
        set_n_tuplevalue(pa,i,n);
//        add_critreg(pa);
        if(pe==NIL) {
           pe=make_n_list(n,NIL);
        } else {
           pe=make_n_list(n,pe);
        };
//        pa=get_critreg(CR_LAST);
     };
  };
//  lpa=add_critreg(pa);

  // копирем локальные переменные
//  f=get_critreg(lf);
  p=get_n_funcloc(f);
//  print(p);
//  lp=add_critreg(p);
  l=get_n_tuplelen(p);
  pl=make_n_tuple(l);
  fill_n_tuple(pl, NIL);
  pool_count_up(pl);

//  push_n_stack(get_current_stackd,f);
  for(i=0;i<l;i++) {
//     p=get_critreg(lp);
     s=get_n_tuplevalue(p,i);
     if(is_n_tuple(s)) {
//        add_critreg(pe);
//        add_critreg(pl);
        n=copy_n_tuple(s);
//        pl=get_critreg(CR_LAST);
//        pe=get_critreg(CR_LAST);
        set_n_tuplevalue(pl,i,n);
//        add_critreg(pl);
        if(pe==NIL) {
           pe=make_n_list(n,NIL);
        } else {
           pe=make_n_list(n,pe);
        };
//        pl=get_critreg(CR_LAST);
     };
  };
  // доделаем окружение
//  add_critreg(pl);
  pe=make_n_list(pe,NIL);

//  set_n_stype(pe,NENVES);
//  pl=get_critreg(CR_LAST);

//  pa=get_critreg(lpa);
//  f=del_critreg(lf);

  // установим соответствующие параметры
//  print(get_n_funcconst(f));
  set_n_closureconst(c,get_n_funcconst(f));
  set_n_closurearg(c,pa);
  pool_count_down(pa);
//  print(pl);
  set_n_closureloc(c,pl);
  pool_count_down(pl);
  set_n_closureenv(c,pe);
  set_n_closurefunc(c,get_n_funcfunc(f));
  set_n_closurecode(c,get_n_funccode(f));
  set_n_tuplevalue(c,6,NIL);
  set_n_tuplevalue(c,7,NIL);
  set_n_tuplevalue(c,8,pe);
  set_n_tuplevalue(c,9,get_n_funcmname(f));
  set_n_tuplevalue(c,10,NIL);
  set_n_tuplevalue(c,11,make_n_long(0));

  return(c);
};

// построение глобального дерева
void make_gtree_root(void)
{
    PNODE p, p1;
    long i;

//win_printf("make_gtree_root\r\n");
    ROOT=make_n_tuple(10);
//win_printf("make_gtree_root1\r\n");
    pool_count_always(ROOT);
//win_printf("make_gtree_root2\r\n");
    fill_n_tuple(ROOT, NIL);
//win_printf("make_gtree_root3\r\n");
//    stat_info();
//    get_char();
/*
    0 - список символов системы и глобальные переменные
    1 - список функций системы
    2 - список прерываний
    3 - список стеков данных
    4 - список стеков программы
    5 - список встроенных функций
    6 - список библиотечных функций (считываются как ссылки)
    7 - настройка
    8 - список потоков замыканий
    9 - список критических регистров
*/

//win_printf("make_gtree_root4\r\n");
    p=make_n_envext();
//    print(p);
    set_n_tuplevalue(ROOT,0,p);
//    print(ROOT);
//    stat_info();
//    get_char();

//win_printf("make_gtree_root5\r\n");
    p=make_n_envext();
    set_n_tuplevalue(ROOT,1,p);
//    stat_info();
//    get_char();

//    p=make_n_interrupt();
//    set_n_tuplevalue(ROOT,2,p);

/*
    p=make_n_env();
    set_n_tuplevalue(global_node_root,3,p);
    p=make_n_env();
    set_n_tuplevalue(global_node_root,4,p);
*/
//win_printf("make_gtree_root6\r\n");
    p=make_n_envext();
    set_n_tuplevalue(ROOT,5,p);
//    stat_info();
//    get_char();

//win_printf("make_gtree_root7\r\n");
    p=make_n_tuple(5);
    fill_n_tuple(p, NIL);
    set_n_tuplevalue(ROOT,7,p);
    for(i=0;i<5;i++) {
       p1=make_n_tuple(1);
       fill_n_tuple(p1, NIL);
       set_n_tuplevalue(p,i,p1);
    };
//    stat_info();
//    get_char();

//win_printf("make_gtree_root8\r\n");
    p=make_n_tuple(10);
    fill_n_tuple(p, NIL);
    set_n_tuplevalue(ROOT,8,p);
//    stat_info();
//    get_char();
//    print(ROOT);
    p=make_n_tuple(10);
    fill_n_tuple(p, NIL);
    CRIT_REG=p;
    set_n_tuplevalue(ROOT,9,p);
//win_printf("make_gtree_root end\r\n");
};

// определение глобального символа
PNODE make_global_symbol(char *f)
{
   PNODE ret, p;
   p=get_global_symbol;
//   print(p);
   ret=gettuple_n_envexts(p,f);
   if(ret==0) {
       ret=make_n_symbol(f);
       add_n_envext(p,ret,NIL);
       return(ret);
   };
   return(get_n_tuplevalue(ret,0));
};

PNODE find_func_s(PNODE fname)
{
   PNODE ret, p, p1;
   char *f;

   f=get_n_stringvalue(fname);
   p=get_global_symbol;

   p1=gettuple_n_envexts(p,f);
   if(p1==0) {
       ret=NIL;
   } else {
       ret=get_n_envext(get_global_func,get_n_tuplevalue(p1,0));
   };
   return(ret);
};

PNODE find_func_name(PNODE fname)
{
    PNODE p, ret;

    p=get_global_func;
    ret=get_n_envext(p, fname);
    if(ret==NIL) {
       p=get_global_extfunc;
       ret=get_n_envext(p, fname);
    };
    return(ret);
};

/*
// распакуем замыкание во фрейм
void unpack_closure(PNODE c)
{
  PNODE p;
   global_frame.closure=c;
   p=get_n_closurecode(c);
   current_code=get_n_stringvalue(p);
   #ifdef DEBUG_VER
       p=get_n_closureconst(c);
       get_current_consts=p;
       p=get_n_closurearg(c);
       global_frame.args=p;
       p=get_n_closureloc(c);
       get_current_locs=p;
       p=get_n_closurefunc(c);
       get_current_funcs=p;
       p=get_n_closureenv(c);
       get_current_env=p;
       p=get_n_closuremname(c);
       get_current_mname=p;
   #else
//       set_n_closurestackc(c,global_frame.stackc);
//       set_n_closurestackd(c,global_frame.stackd);
   #endif
};
*/

// идея заключается в том, что вызовы функций сохраняются тоже внутри
// таким образом в функции сохраняется указатель на предыдущее замыкание
// кроме этого сохраняются в кортеже параллельные вызовы.

#define COMMAND_ARRAY_LEN 256
char command_array[COMMAND_ARRAY_LEN]; // всего может быть 256 команд

// заполнение массива команд
void load_command_array(void)
{
   s_int k;
   k=0;
   while(aword[k].name[0]!=0) {
//        printf(">> %s, %ld\n",aword[k].name,(long)aword[k].command);
        command_array[(s_int)aword[k].code]=k;
        k++;
   };
};

s_int global_stream_closure; // номер исполняемого глобального потока
unsigned int *global_bstack; // указатель на стек возвратов
unsigned int global_bstack_size; // размер стека
unsigned int global_bstack_ptr; // текущее значение стека
PCOMMAND_TIMING global_timing; // указатель на стек возвратов

void print_timing(void)
{
 int i;
  for(i=0;i<257;i++){
    if(global_timing[i].score>0){
       if(i==256){
         win_printf("gc, score %d all time %d avg time %d\r\n",global_timing[i].score,global_timing[i].time,global_timing[i].time/global_timing[i].score);
       } else {
         win_printf("cmd=%d, %s, score %d all time %d avg time %d\r\n",i,aword[i].name,global_timing[i].score,global_timing[i].time,global_timing[i].time/global_timing[i].score);
       };
    };
  };
};

// инициализация (загрузка команд, создание стеков и вызов исполнения)
void initialize_loop(int size_dstack, int size_bstack)
{
 PNODE s;
 int i;
  // загрузка команд
  load_command_array();

  // задаем стек данных
  s=make_n_stack(size_dstack);
  set_global_stack(s);
  // стек возвратов
  global_bstack=(unsigned int *)malloc(size_bstack*sizeof(int));
  global_bstack_ptr=0;
  global_bstack_size=size_bstack;
  global_timing=(PCOMMAND_TIMING)malloc(257*sizeof(COMMAND_TIMING));
  for(i=0;i<257;i++) {
     global_timing[i].score=0;
     global_timing[i].time=0;
  };
  // устанавливаем номер текущего процесса (потока замыканий)
  global_stream_closure=0;
};

void push_to_bstack(unsigned int v)
{
  if(global_bstack_ptr<global_bstack_size) {
     global_bstack[global_bstack_ptr]=v;
     global_bstack_ptr++;
  };
};

unsigned int pop_from_bstack(void)
{
  unsigned int ret;
  if((global_bstack_ptr>0) && (global_bstack_ptr<global_bstack_size)) {
    global_bstack_ptr--;
    ret=global_bstack[global_bstack_ptr];
  };
  return ret;
};

s_int global_end_exec;
s_int global_debug_code;
s_int global_reg_error;
s_int global_interrupt_ext;

PNODE current_ret;

// загрузка аргументов
void load_funcarg()
{
   PNODE f, p;
   s_int l, k, i=0, j=0;
    // считываем аргументы так.
    // ищем в стеке метку 0, затем сохраняем функцию  возврата в переменной
    // и загружаем по списку аргументы (связывая их с соответствующим именем)
    // затем опять загоняем в стек функцию

    l=0;
    while(get_n_stacksvalue(get_current_stackd,l)!=SLABEL) {
//         print(get_n_stacksvalue(get_current_stackd,l));
         l++;
    };

    if(l!=0) {
        // случай когда количество аргументов =0 ????
        if(l>get_n_tuplelen(get_current_args)) {
             // количество слов в стеке больше количества аргументов
             i=l;
             if(get_n_tuplelen(get_current_args)>0) {
                 for(k=0;k<(get_n_tuplelen(get_current_args)-1);k++) {
                    if(l>0) {
                        // пристегнем аргумент
                        l--;
                        set_n_tuplevalue(get_n_tuplevalue(get_current_args,k), 1, get_n_stacksvalue(get_current_stackd,l));
                    };
                 };
                 // закончилось присваивание без одного
    //             add_critreg(f);
                 // построим кортеж
                 p=make_n_tuple(l);
    //             set_n_stype(p,1);  // патч !!!
                 set_n_tuplevalue(get_n_tuplevalue(get_current_args,k),1, p);
                 // добавим оставшиеся аргументы
                 j=0;
                 while(l>0) {
                     l--;
    //                 print(get_n_stacksvalue(get_current_stackd,l));
                     set_n_tuplevalue(p,j,get_n_stacksvalue(get_current_stackd,l));
                     j++;
                 };
    //             f=del_critreg(CR_LAST);
             } else {
                 for(k=0;k<get_n_tuplelen(get_current_args);k++) {
                    if(l>0) {
                        // пристегнем аргумент
                        l--;
                        set_n_tuplevalue(get_n_tuplevalue(get_current_args,k),1, get_n_stacksvalue(get_current_stackd,l));
                    };
                 };
             };
             // очистим стек
             while(i>0) {
                 pop_n_stack(get_current_stackd);
                 i--;
             };
        } else {
             if(l==get_n_tuplelen(get_current_args)) {
                 // количество слов в стеке равно количеству аргументов
                 // самый левый аргумент самый глубокий
                 i=l;
                 for(k=0;k<get_n_tuplelen(get_current_args);k++) {
                    if(l>0) {
                        // пристегнем аргумент
                        l--;
                        set_n_tuplevalue(get_n_tuplevalue(get_current_args,k),1, get_n_stacksvalue(get_current_stackd,l));
                    };
                 };
                 // очистим стек
                 while(i>0) {
                      pop_n_stack(get_current_stackd);
                      i--;
                 };

             } else {
                if(l<get_n_tuplelen(get_current_args)) {
                     i=l;
                     // количество слов в стеке меньше количества аргументов
                     for(k=0;k<get_n_tuplelen(get_current_args);k++) {
                        if(l>0) {
                            l--;
                            // пристегнем аргумент
                            set_n_tuplevalue(get_n_tuplevalue(get_current_args,k),1, get_n_stackvalue(get_current_stackd,l));
                        };
                     };
                     // очистим стек
                     while(i>0) {
                        pop_n_stack(get_current_stackd);
                        i--;
                     };
                };
             };
        };
    };
    // обратно функцию возврата
};

void exit_vm(s_int q)
{

};

void call_exit_hook(void)
{

};

PNODE make_n_lists(char * s, s_int len)
{
  PNODE ret;
  // сложная функция создания списка
  ret=_readl(s);
  return(ret);
};

PNODE make_n_tuples(char * s, s_int len)
{
  PNODE ret;
  // сложная функция создания кортежа
  ret=_readt(s);
  return(ret);
};

void command_print(unsigned char *cmd, s_int cmd_ptr)
{
  s_int n, i;

            win_printf("Command  %d  %s",aword[cmd_ptr].code,aword[cmd_ptr].name);
            if (aword[cmd_ptr].narg>0) {
               // возьмем второй аргумент
               n=get_long_value((char *)((s_int)cmd+current_pc+1));
               win_printf(" %ld ",n);
            };
            if (aword[cmd_ptr].sarg>0) {
               win_printf("(");
               i=aword[cmd_ptr].sarg;
               while(i>0) {
//                  win_printf(" %x ",get_n_stacksvalue(get_current_stackd,i));
//                  princ(get_n_stacksvalue(get_current_stackd,i));
                  win_printf(" ");
                  i--;
               };
               win_printf(")");
            };
            win_printf("\r\n");
};

#ifdef __BORLANDC__
#ifdef _WIN
void RunCycle(void);
#else
#endif
#else
#endif

//командный цикл
int command_loop(int debug_code)
{
  void (*command)(void);
  void (*fcommand)(void);
  unsigned char global_command;
  s_int cmd_ptr, l;
  PNODE p, c;
  s_int n, i;
  unsigned char *cmd;
  int c1;
  unsigned int WorkingTime, WorkingTimeA;

  // поставляется уже заполненый первичный фрейм с командами инициализации и запуска
  if (current_code==NULL) {
      return(255); // выход по отсутствию кода
  };

  //current_sp=0;
  if(global_type_vm_func==0){
     set_current_closurecounter(0);
  } else if(global_type_vm_func==1){
     global_pc_counter=0;
  };
  //внешний цикл исполнения
  global_end_exec=0; //конец исполнения

//  global_debug_code=0;
       win_printf("global_debug_code %d\r\n",global_debug_code);

  while(global_end_exec==0) {
     //выделяем команду
#ifdef DEBUG_VM
       win_printf("current_pc %d\r\n",current_pc);
#endif

#ifdef __BORLANDC__
#ifdef _WIN
       RunCycle();
#else
#endif
#else
#endif
       if(global_pc_counter==23){
          win_printf("");
       };
       if(global_type_vm_func==0){
           cmd=current_code;
       } else if(global_type_vm_func==1){
           cmd=get_n_stringvalue(global_current_command_string);
       };
       if(global_type_vm_func==0){
           global_command=cmd[current_pc];
       } else if(global_type_vm_func==1){
           global_command=cmd[global_pc_counter];
       };
       cmd_ptr=command_array[(long)global_command];
       command=aword[cmd_ptr].command;
       if(global_debug_code>0) {
            win_printf("<%010d> %d  %s",global_pc_counter,aword[cmd_ptr].code,aword[cmd_ptr].name);
            if (aword[cmd_ptr].narg>0) {
               // возьмем второй аргумент
               n=get_long_value((char *)((s_int)cmd+global_pc_counter+1));
               win_printf(" %ld ",n);
            };
            if (aword[cmd_ptr].sarg>0) {
               win_printf("(");
               i=aword[cmd_ptr].sarg;
               while(i>0) {
                  win_printf(" %x ",get_n_stacksvalue(get_current_stackd,i-1));
                  princ(get_n_stacksvalue(get_current_stackd,i-1));
                  win_printf(" ");
                  i--;
               };
               win_printf(")");
            };
            win_printf("\r\n");
       };
       if(global_debug_code==2) {
#ifdef __BORLANDC__
            getch();
#else
#ifdef __LINUX

#else
       while ((c1 = uart0Getch()) < 0) {};
       uart0Putch(c1);
#endif
#endif
       };
       if(command==NULL) {
               win_printf("\r\nНеизвестная команда %d!\r\nОстанавливаюсь!\r\n",global_command);
               global_end_exec=1;
               return(global_end_exec);
       } else {
         global_reg_error=0; // сбрасываем регистр ошибок
         command_code_int=cmd_ptr;
         command_code_flag=0;
         // сюда надо воткнуть профилировку.
#ifdef __BORLANDC__
         // засекаем время
         WorkingTime = GetTickCount();
#endif
         (*command)();
#ifdef __BORLANDC__
         WorkingTimeA = GetTickCount();
         global_timing[cmd_ptr].score=global_timing[cmd_ptr].score+1;
         global_timing[cmd_ptr].time=global_timing[cmd_ptr].time+(WorkingTimeA-WorkingTime)+1;
#endif

//         if(global_prev_func_p!=global_prev_func){
//           win_printf("global_prev_func ");print(global_prev_func);
//         };
//         print_n_stackvalue(get_current_stackd);

         // смотрим время и записываем его... в массив структур вида <команда><время выполнения><количество выполненых команд>
/*
         if(command_code_flag==1){
            command_print(cmd, cmd_ptr);
         };
*/
         if(global_reg_error>0) {
            global_end_exec=1;
         };

#ifdef _JUMP_
         // принудительный выход, если подвисли
         if(!flag_settings) GunReturn();
#endif
/*
         if(global_interrupt_ext>0) {
           // воткнуть сюда очередь прерываний!!!!
//   win_printf(">> %ld\n",global_interrupt_ext);
           global_reg_error=global_interrupt_ext;
           global_interrupt_ext=0;
         };
         // проверим на прерывание
         if(global_reg_error>0) {
//            win_printf(">>> %ld\n",global_reg_error);
                // ищем требуемое прерывание
                p=get_n_interruptl(get_global_inter,global_reg_error);
                if(p!=NIL) {
//                print_stack();
//                win_printf("\n");
                push_n_stack(global_frame.stackd, SLabel);
                // аргументов нет!!!
                push_n_stack(global_frame.stackd,global_frame.closure);
                push_n_stackp(get_current_stackc, current_pc);
//                push_n_stack(global_frame.stackd,global_frame.closure); // это аргумент
                if(is_n_func(p)) {
                    // создадим замыкание
                    c=make_n_closurefunc(p);
                    unpack_closure(c);
                    // загрузка аргументов из стека
                    load_funcarg();
                    // устанавливаем счетчик команд на 0
                    current_pc=0;
                } else {
                    if(is_n_long(p)) {
                         // загрузка встроенной функции или из библиотеки
                         fcommand=(void *)get_n_longvalue(p);
                         (*fcommand)();
                         l=0;
                         while((p=get_n_stacksvalue(get_current_stackd,l))!=SLabel) {
      //                        if(p==0) break; ????????????????????
                              l++;
                         };
                         // l==0 -> приводит к ошибке
                         if(l!=0) {
                              p=get_n_stacksvalue(get_current_stackd,l-1);
                              // очистим стек
                              while(l>=0) {
                                    pop_n_stack(get_current_stackd);
                                    l--;
                              };
                         } else {
                              fatal_error(" Нарушена синхронность стека данных (irq) !\n");
                              global_end_exec=3;
                         };
                         pop_n_stackp(get_current_stackc);
                    };
                };
                };
         };
*/
       };
  };
  print_timing();

  win_printf("Exit\r\n");
  return(global_end_exec); // выход по отсутствию кода
};
