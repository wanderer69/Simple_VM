// объекты ВМ
#define ARCHITECT16 1

#include <stdlib.h>
#include <string.h>
#include "MEMMAN.h"
#include "objects.h"
#include "print.h"

#ifndef __BORLANDC__
#define printf uart0Puts
#endif

#define _DEBUG 1

PNODE global_node_root;
PNODE global_const_nil;
PNODE global_const_false;
PNODE global_const_true;
PNODE global_const_slabel;
PNODE CRIT_REG;

int get_long_value(char *p)
{
  int ret;
  char *s;
  s=(char *)(&ret);
  s[0]=p[0];
  s[1]=p[1];
  s[2]=p[2];
  s[3]=p[3];
//   win_printf(">> %x %x %x %x\r\n",s[0],s[1],s[2],s[3]);
  return(ret);
};

void set_long_value(char *p, int r)
{
  char *s;
  s=(char *)(&r);

  p[0]=s[0];
  p[1]=s[1];
  p[2]=s[2];
  p[3]=s[3];
};

// присваивание PNODE
PNODE set_n_value(PNODE v, PNODE *n) {
unsigned int r;
 // присваиваем значение v в переменную n
//  win_printf("set_n_value %x",get_long_value(n));
  r=get_long_value((char *)n);
//  win_printf(">>> %x\r\n",r);
#ifndef __BORLANDC__
  if(r>=0x80000000) { //будем считать что там старый указатель, его счетчик надо уменьшить
#else
  if(r!=0) { //будем считать что там старый указатель, его счетчик надо уменьшить
#endif
//     win_printf("%c %c %c %c\n",((char *)r)[0],((char *)r)[1],((char *)r)[2],((char *)r)[3]);
//     win_printf("a1\r\n");
//   pool_count_down((char *)get_long_value((char *)n));
     pool_count_down((char *)r);
//     win_printf("a2\r\n");
     //*n=v;
//     win_printf("a3\r\n");
     set_long_value((char *)n,(int)v);
//     win_printf("a4\r\n");
     pool_count_up(v);
//     win_printf("a5\r\n");
  } else {
//     win_printf("b1\r\n");
     //*n=v;
     set_long_value((char *)n,(int)v);
//     win_printf("b2\r\n");
     pool_count_up(v);
//     win_printf("b3\r\n");
  };
  return(v);
};

PNODE set_n_value1(PNODE v, PNODE *n) {
 // присваиваем значение v в переменную n
 set_long_value((char *)n,(int)v);
  return(v);
};

// отвязать значение
void clear_n_value(PNODE *n) { // присваиваем значение NULL в переменную n
  if(*((char*)n)!=NULL) { //будем считать что там старый указатель, его счетчик надо уменьшить
     pool_count_down(*n);
     //*n=NULL;
     set_long_value((char *)n,NULL);
  };
};

// целое байт типа и 4 байта значения
PNODE make_n_long(s_int l) {
   PNODE ret;
   s_int *p;

   ret=pool_memmake(1+N_LONG_SIZE);
   set_n_nodetype(ret, N_LONG);
   //p=(s_int *)((s_int)ret+N_HEADER_SIZE);
   //*p=l;
//   printf("\r\n>> %x\r\n",l);
//   getch();
   set_long_value((char *)((s_int)ret+N_HEADER_SIZE),l);

   // обязательно сохранить в критическом регистре иначе сборка мусора уберет этот указатель
   return(ret);
};

s_int get_n_longvalue(PNODE p) {
   s_int ret;
   s_int *p1;

//   p1=(s_int *)((s_int)p+N_HEADER_SIZE);
//   ret=*p1;
   ret=get_long_value((char *)((s_int)p+N_HEADER_SIZE));
   return(ret);
};

void set_n_longvalue(PNODE p, s_int l) {
   s_int *p1;

//   p1=(s_int *)((s_int)p+N_HEADER_SIZE);
//   *p1=l;
   set_long_value((char *)((s_int)p+N_HEADER_SIZE),l);
};

#if ARCHITECT16
#if USE_FLOAT
PNODE make_n_float(s_int l) {
   PNODE ret;
   s_flt *p;

   ret=pool_memmake(1+N_FLOAT_SIZE);
   set_n_nodetype(ret, N_FLOAT);
   p=(s_flt *)((s_int)ret+N_HEADER_SIZE);
   *p=l;
   // обязательно сохранить в критическом регистре иначе сборка мусора уберет этот указатель
   return(ret);
};

s_int get_n_floatvalue(PNODE p) {
   s_int ret;
   s_flt *p1;

   p1=(s_flt *)((s_int)p+N_HEADER_SIZE);
   ret=*p1;
   return(ret);
};

void set_n_floatvalue(PNODE p, s_int l) {
   s_flt *p1;

   p1=(s_flt *)((s_int)p+N_HEADER_SIZE);
   *p1=l;
};
#endif
#endif

// строка байт типа и сколько то байт длины
PNODE make_n_string(char *s) {
   PNODE ret;
   char *p;
   s_int i, len;

   len=strlen(s);
   ret=pool_memmake(N_HEADER_SIZE+len+1);
   set_n_nodetype(ret, N_STRING);
   p=(char *)((s_int)ret+N_HEADER_SIZE);
   for(i=0;i<len;i++){ p[i]=s[i]; };
   *(char *)((s_int)p+len)=0;
//   info_block(4498);
   // обязательно сохранить в критическом регистре иначе сборка мусора уберет этот указатель
   return(ret);
};

// строка байт типа и сколько то байт длины
PNODE make_n_symbol(char *s) {
   PNODE ret;
   ret=make_n_string(s);
   set_n_nodetype(ret, N_SYMBOL);
   return(ret);
};

PNODE make_n_stringn(char * s, s_int len)
{
  PNODE ret;
   char *p;
   s_int i;

   ret=pool_memmake(N_HEADER_SIZE+len+1);
   set_n_nodetype(ret, N_STRING);
   p=(char *)((s_int)ret+N_HEADER_SIZE);
   for(i=0;i<len;i++){ p[i]=s[i]; };
   *(char *)((s_int)p+len+1)=0;
   return(ret);
};

char *get_n_stringvalue(PNODE p) {
   char *ret;
   char *p1;

   p1=(char *)((s_int)p+N_HEADER_SIZE);
   return(p1);
};

void set_n_stringvalue(PNODE p, char *s) {
   char *p1;
   s_int i, len;

   // надо проверить, чтобы длина строки была НЕ БОЛЬШЕ!!!!
   len=strlen(s);
   p1=(char *)((s_int)p+N_HEADER_SIZE);
   for(i=0;i<len;i++){ p1[i]=s[i]; };
   *(char *)((s_int)p1+len+1)=0;
};

PNODE cont_n_string(PNODE p1, PNODE p2)
{
  PNODE ret;
  s_int len;
  char *c;

//  int i;
//  char *s;

  len=get_n_stringlen(p1)+get_n_stringlen(p2)-1;
  ret=pool_memmake(N_HEADER_SIZE+len+1);
  set_n_nodetype(ret, N_STRING);

//  set_n_ftype(ret)=NFREE;
//  set_n_type(ret,NSTRING);
//  set_n_stringlen(ret,len);

  c=(char *)((s_int)ret+N_HEADER_SIZE);
//  c=(char *)(((char *)(get_p_node(ret)))+6);

//  win_printf("len1=%d len2=%d\r\n",get_n_stringlen(p1),get_n_stringlen(p2));
//  win_printf("c1=%x \r\n",(int)c);
//  len=get_n_stringlen(p1);
//  s=get_n_stringvalue(p1);
//  for(i=0;i<len;i++){c[i]=s[i];};
  memcpy(c,get_n_stringvalue(p1),get_n_stringlen(p1));
//  print(ret);
  c=(char *)((int)c+get_n_stringlen(p1)-2);
//  win_printf("c2=%x \r\n",(int)c);
//  len=get_n_stringlen(p2);
//  s=get_n_stringvalue(p2);
//  for(i=0;i<len;i++){c[i]=s[i];win_printf("%c\r\n",s[i]);};
  memcpy(c,get_n_stringvalue(p2),get_n_stringlen(p2));
//  print(ret);
  return(ret);
};

long find_n_string(PNODE p1, PNODE p2)
{
//  long ret;
  s_int l1, l2, i, j, k, l;
  char *c1, *c2;

  // алгоритм поиска в строке подстроки
  l1=get_n_stringlen(p1);
  l2=get_n_stringlen(p2);
  c1=get_n_stringvalue(p1);
  c2=get_n_stringvalue(p2);
/*
  j=0;
  k=0;
  l=0;
  for(i=0;i<l1;) {
  win_printf("c1[i]= %d %x c2[j]= %d %x\r\n",i, c1[i],j, c2[j]);
    if(c1[i]==c2[j]) {
        if(j==0) { k=i; l=i; };
        if(j<(l2-1)) {
            j++;
        } else {
            return(k);
        };
    } else {
        j=0;
    };
    if(l>0) i=l;
    i++;
  };
*/
  j=0;
  k=0;
  // двигаемся по с1 сравниваем с c2
  for(i=0;i<(l1-1);){
//    win_printf("c1[i]= %d %c c2[j]= %d %c\r\n",i, c1[i],j, c2[j]);
    if(c1[i]==c2[j]) {
       // запускаем локальный счетчик
       if(j==0) {k=i; l=i;};
       // условие окончания сканирования
       j++;
       if(c2[j]==0) {
//       } else {
            return(k);
       };
    } else {
       if(j>0){
         j=0;
         i=l;
       };
    };
    i++;
  };
  return(-1);
};

PNODE mid_n_string(PNODE p, s_int b, s_int l)
{
  PNODE ret;
  char *c, *c1;

  c=get_n_stringvalue(p);
  c=(char *)(c+b);

  ret=pool_memmake(N_HEADER_SIZE+l+1);
  set_n_nodetype(ret, N_STRING);

//  ret=(PNODE)node_memmake(NSTRING_LEN+l+1);
//  set_n_ftype(ret)=NFREE;
//  set_n_type(ret,NSTRING);
//  set_n_stringlen(ret,l+1);

  c1=(char *)((s_int)ret+N_HEADER_SIZE);

//  c1=(char *)(((char *)(get_p_node(ret)))+6);
  memcpy(c1,c,l);
  c1[l+1]=0;
  return(ret);
};

// конс байт типа и два двойных слова на указатель
PNODE make_n_list(PNODE a, PNODE d) {
   PNODE ret;
   PNODE *p;
//char buff[256];
#ifdef DEBUG_
printf("\n\rmake_n_list: begin\r\n");
#endif
   ret=pool_memmake(N_HEADER_SIZE+N_PTR_SIZE+N_PTR_SIZE);
//printf("make_n_list:2\n");
   set_n_nodetype(ret, N_CONS);
//printf("make_n_list:3\n");
   p=(PNODE *)((s_int)ret+N_HEADER_SIZE);
//printf("make_n_list:4\n");
   // Вот тут то мы присваивание и сделаем
//   sprintf(buff,">%x %x\r\n",a,p);
//   printf(buff);
   set_n_value(a,p);
//printf("make_n_list:5\n");
// так было раньше  *p=a;
   p=(PNODE *)((s_int)ret+N_HEADER_SIZE+N_PTR_SIZE);
//printf("make_n_list:6\n");
   // Вот тут то мы присваивание и сделаем
   set_n_value(d,p);
#ifdef DEBUG_
printf("make_n_list: end\r\n");
#endif
   // так было раньше  *p=d;
   // обязательно сохранить в критическом регистре иначе сборка мусора уберет этот указатель
   return(ret);
};

PNODE get_n_listcar(PNODE p) {
   PNODE ret;
   PNODE *p1;

   ret=(PNODE)get_long_value((char *)((s_int)p+N_HEADER_SIZE));
   return(ret);
};

PNODE get_n_listcdr(PNODE p) {
   PNODE ret;
   PNODE *p1;

   ret=(PNODE)get_long_value((char *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE));
   return(ret);
};

void set_n_listcar(PNODE p, PNODE l) {
   char *p1;

   p1=(char *)((s_int)p+N_HEADER_SIZE);
   // Вот тут то мы присваивание и сделаем
   set_n_value(l,(PNODE *)p1);
// так было раньше  *p1=l;
};

void set_n_listcdr(PNODE p, PNODE l) {
   char *p1;

   p1=(char *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE);
   // Вот тут то мы присваивание и сделаем
//   print(l);
   set_n_value(l,(PNODE *)p1);
   // так было раньше  *p1=l;
};

void set_n_listcar1(PNODE p, PNODE l) {
   char *p1;

   p1=(char *)((s_int)p+N_HEADER_SIZE);
   // Вот тут то мы присваивание и сделаем
   set_n_value1(l,(PNODE *)p1);
// так было раньше  *p1=l;
};

void set_n_listcdr1(PNODE p, PNODE l) {
   char *p1;

   p1=(char *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE);
   // Вот тут то мы присваивание и сделаем
   set_n_value1(l,(PNODE *)p1);
   // так было раньше  *p1=l;
};

// кортеж байт типа, двойное слово длины и далее кортеж
PNODE make_n_tuple(s_int l) {
   PNODE ret;
   s_int *p;
   int i;

   ret=pool_memmake(N_HEADER_SIZE+N_BACK_SIZE+N_PTR_SIZE+N_PTR_SIZE*l);
   set_n_nodetype(ret, N_TUPLE);
//   p=(s_int *)((s_int)ret+N_HEADER_SIZE);
   // длину прописываем
//   *p=l;

   for(i=0;i<l;i++) {
     set_long_value((char *)((s_int)ret+N_HEADER_SIZE+N_BACK_SIZE+N_PTR_SIZE+N_PTR_SIZE*i),0);
   };

   set_long_value((char *)((s_int)ret+N_HEADER_SIZE+N_BACK_SIZE),l);
   // обязательно сохранить в критическом регистре иначе сборка мусора уберет этот указатель
   return(ret);
};

s_int get_n_tuplelen(PNODE p) {
   s_int ret;
   s_int *p1;
   s_int l;

   l=((s_int)p+N_HEADER_SIZE+N_BACK_SIZE);

//   win_printf("l> %x %x %x\r\n",p,N_HEADER_SIZE,l);
   ret=get_long_value((char *)l);

//   p1=(s_int *)((s_int)p+N_HEADER_SIZE);
//   ret=*p1;
   return(ret);
};

void set_n_tuplerest(PNODE p, s_int l) {
   set_long_value((char *)((s_int)p+N_HEADER_SIZE),l);
};

s_int get_n_tuplerest(PNODE p) {
   s_int ret;
   s_int *p1;
   s_int l;

   l=((s_int)p+N_HEADER_SIZE);

//   win_printf("l> %x %x %x\r\n",p,N_HEADER_SIZE,l);
   ret=get_long_value((char *)l);

//   p1=(s_int *)((s_int)p+N_HEADER_SIZE);
//   ret=*p1;
   return(ret);
};


PNODE get_n_tuplevalue(PNODE p, s_int l) {
   PNODE ret;
   PNODE *p1;

   ret=(PNODE)get_long_value((char *)((s_int)p+N_HEADER_SIZE+N_BACK_SIZE+N_PTR_SIZE+N_PTR_SIZE*l));
//   p1=(PNODE *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE+N_PTR_SIZE*l);
//   ret=*p1;
//   print(ret);
   return(ret);
};

void set_n_tuplevalue(PNODE p, s_int l, PNODE a) {
   PNODE *p1;
   s_int l1;
   l1=(s_int)((s_int)p+N_HEADER_SIZE+N_BACK_SIZE+N_PTR_SIZE+N_PTR_SIZE*l);
//   win_printf("set_n_tuplevalue %x %x\r\n",l,l1);
//   p1=(PNODE *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE+N_PTR_SIZE*l);
   // Вот тут то мы присваивание и сделаем

   //   set_n_value(a,(s_int)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE+N_PTR_SIZE*l));
   set_n_value(a,(PNODE *)l1);

// так было раньше  *p1=l;
//   win_printf("set_n_tuplevalue end\r\n");
};

void set_n_tuplevalue1(PNODE p, s_int l, PNODE a) {
   PNODE *p1;
   s_int l1;
   l1=(s_int)((s_int)p+N_HEADER_SIZE+N_BACK_SIZE+N_PTR_SIZE+N_PTR_SIZE*l);
   // Вот тут то мы присваивание и сделаем
   set_n_value1(a,(PNODE *)l1);
};

void fill_n_tuple(PNODE p, PNODE v) {
   s_int i;
   s_int len;
   len=get_n_tuplelen(p);
//   win_printf("len> %d\r\n",len);
   for(i=0;i<get_n_tuplelen(p);i++) {
     set_n_tuplevalue(p,i,v);
   };
//   win_printf("fill_n_tuple end \r\n");

};

// изменяет количество элементов кортежа
PNODE resize_n_tuple(PNODE p, s_int l)
{
  PNODE ret;
  char *cs, *cd;
  s_int i;
  s_int *p1;

   ret=pool_memmake(N_HEADER_SIZE+N_BACK_SIZE+N_PTR_SIZE+N_PTR_SIZE*l);
   set_n_nodetype(ret, N_TUPLE);
//   p1=(s_int *)((s_int)ret+N_HEADER_SIZE);

   // длину прописываем
//   *p1=l;
   set_long_value((char *)((s_int)ret+N_HEADER_SIZE+N_BACK_SIZE),l);
   fill_n_tuple(ret,NIL);
//   for(i=0;i<get_n_tuplelen(ret);i++) { set_n_tuplevalue(ret,i,NIL);};
   for(i=0;i<get_n_tuplelen(p);i++){
     set_n_tuplevalue(ret,i,get_n_tuplevalue(p,i));
   };
  return(ret);
};

// копирует кортеж
PNODE copy_n_tuple(PNODE p)
{
  PNODE ret;
  s_int i;
  s_int *p1;

   ret=pool_memmake(N_HEADER_SIZE+N_BACK_SIZE+N_PTR_SIZE+N_PTR_SIZE*get_n_tuplelen(p));
   set_n_nodetype(ret, N_TUPLE);
//   p1=(s_int *)((s_int)ret+N_HEADER_SIZE);
   // длину прописываем
   //*p1=get_n_tuplelen(p);
   set_long_value((char *)((s_int)ret+N_HEADER_SIZE+N_BACK_SIZE),get_n_tuplelen(p));

   for(i=0;i<get_n_tuplelen(p);i++){
     set_n_tuplevalue(ret,i,get_n_tuplevalue(p,i));
   };
  return(ret);
};

// стек байт типа, двойное слово длины и двойное слово указателя и далее сам массив
PNODE make_n_stack(s_int l) {
   PNODE ret;
   s_int *p;

   ret=pool_memmake(N_HEADER_SIZE+N_PTR_SIZE+N_PTR_SIZE+N_PTR_SIZE*l);
   set_n_nodetype(ret, N_STACK);
//   p=(s_int *)((s_int)ret+N_HEADER_SIZE);
   // длину прописываем
//   *p=l;
   set_long_value((char *)((s_int)ret+N_HEADER_SIZE),l);
//   p=(s_int *)((s_int)ret+N_HEADER_SIZE+N_PTR_SIZE);
   // прописываем указатель
//   *p=0;
   set_long_value((char *)((s_int)ret+N_HEADER_SIZE+N_PTR_SIZE),0);
   // обязательно сохранить в критическом регистре иначе сборка мусора уберет этот указатель
   return(ret);
};

s_int get_n_stacklen(PNODE p) {
   s_int ret;
   s_int *p1;

//   p1=(s_int *)((s_int)p+N_HEADER_SIZE);
   ret=get_long_value((char *)((s_int)p+N_HEADER_SIZE));
   return(ret);
};

PNODE get_n_stackvalue(PNODE p, s_int l) {
   PNODE ret;
   PNODE *p1;

//   p1=(PNODE *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE*2+N_PTR_SIZE*l);
   ret=(PNODE)get_long_value((char *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE*2+N_PTR_SIZE*l));
   return(ret);
};

void set_n_stackvalue(PNODE p, s_int l, PNODE a) {
   PNODE *p1;

   p1=(PNODE *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE*2+N_PTR_SIZE*l);
   // Вот тут то мы присваивание и сделаем
//   set_n_value(a,p1);
   set_long_value((char *)p1,(int)a);
   pool_count_up(a);
// так было раньше  *p1=l;
};

s_int get_n_stackptr(PNODE p) {
   s_int ret;
   s_int *p1;

//   p1=(s_int *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE);
   ret=get_long_value((char *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE));
   return(ret);
};

void set_n_stackptr(PNODE p, s_int l) {
   s_int *p1;

//   p1=(s_int *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE);
//   *p1=l;
   set_long_value((char *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE),l);
};

PNODE pop_n_stack(PNODE p)
{
  PNODE ret;

  set_n_stackptr(p,get_n_stackptr(p)-1);
  ret=get_n_stackvalue(p,get_n_stackptr(p));
  pool_count_down(ret);
//  set_n_stackvalue1(p,get_n_tupleptr(p),NIL);
  return(ret);
};

void push_n_stack(PNODE p, PNODE v)
{
  // добавление в стек
  if(get_n_stackptr(p)>=STACK_SIZE){
     global_reg_error=1001;
     win_printf("Stack FULL\r\n");
  } else {
  set_n_stackvalue(p,get_n_stackptr(p),v);
  // в случае возникновения ошибки, изменяется длина стека по операции добавления
  set_n_stackptr(p,get_n_stackptr(p)+1);
  };
//  set_n_stackvalue1(p,get_n_tupleptr(p),NIL);
};

void print_n_stackvalue(PNODE p) {
   PNODE ret;
   int i;
   win_printf("stack begin----------------->\r\n");
   for(i=get_n_stackptr(p)-1;i>=0;i--){
      ret=(PNODE)get_long_value((char *)((s_int)p+N_HEADER_SIZE+N_PTR_SIZE*2+N_PTR_SIZE*i));
      win_printf("<%05d> ",i); print(ret);
   };
   win_printf("stack end<-------------------\r\n");
};

// выдает значение относительно вершины стека
PNODE get_n_stacksvalue(PNODE p, s_int l)
{
  PNODE ret;
//  print(get_n_stackvalue(p,get_n_stackptr(p)));
//  print(get_n_stackvalue(p,get_n_stackptr(p)-1));
//  print(get_n_stackvalue(p,get_n_stackptr(p)-2));
//  print(get_n_stackvalue(p,get_n_stackptr(p)-3));
  if((int)(get_n_stackptr(p)-l-1)>=0){
//     win_printf(">>> %d\r\n",get_n_stackptr(p));
     ret=get_n_stackvalue(p,get_n_stackptr(p)-l-1);
  } else {
     global_reg_error=1002;
     win_printf(">>> ptr %d l %d\r\n",get_n_stackptr(p),l);

     while(1);
//     ret=get_n_stackvalue(p,get_n_stackptr(p)-l-1);
     ret=NIL;
  };
  return(ret);
};

// frame
PNODE make_n_frame(PNODE n)
{
  PNODE ret, ret1;

  ret=make_n_tuple(1);
  set_n_tuplevalue(ret,0,NIL);
//  fill_n_tuple(ret,NIL);
//  set_n_stype(ret1,N_FRAME);

//  print(ret);


  ret1=make_n_tuple(3);  // Создаем управляющий кортеж
                         // 0 - имя фрейма 1 - список предков предоставивших свои поля и методы
  fill_n_tuple(ret1,NIL);
  set_n_tuplevalue(ret1,0,n); // Добавим имя

//  print(ret1);

//  pool_count_up(ret1);

//  set_n_tuplevalue(ret1,2,ret); // Управляющий кортеж

//  pool_count_down(ret1);
//  print(ret1);
//  set_n_tuplevalue(ret1,2,ret); // Управляющий кортеж
//  print(ret1);
  set_n_nodetype(ret1, N_FRAME);
//  print(ret1);
  return(ret1);
};

/*
void free_n_frame(PNODE p)
{
  node_memfree(p);
};
*/

/*
void set_n_framename(PNODE p, PNODE n)
{
  set_n_tuplevalue(get_n_tuplevalue(p,0),0,n); // сменим имя
};

PNODE get_n_framename(PNODE p)
{
  PNODE ret;
  ret=get_n_tuplevalue(get_n_tuplevalue(p,0),0); // считаем имя
  return(ret);
};
*/

PNODE copy_n_frame(PNODE n, PNODE k)
{
  PNODE p, ret, ret1, ret2;
  unsigned int i, l;

//  add_critreg(n);
//  add_critreg(k);

  ret1=make_n_tuple(3);  // Создаем управляющий кортеж
                         // 0 - имя фрейма 1 - список предков предоставивших свои поля и методы
  fill_n_tuple(ret1,NIL);
  set_n_tuplevalue(ret1,0,n); // Добавим имя

//  add_critreg(ret1);
//  print(k);
//  print(get_n_tuplevalue(k,0));
//  print(get_n_tuplevalue(get_n_tuplevalue(k,0),1));
//  ret=make_n_list(get_n_tuplevalue(get_n_tuplevalue(k,0),0),get_n_tuplevalue(get_n_tuplevalue(k,0),1));
//  set_n_tuplevalue(ret1,1,ret); // Добавим список предков

  p=get_n_tuplevalue(k,2);
  ret=make_n_tuple(get_n_tuplelen(p));
  fill_n_tuple(ret,NIL);
  set_n_nodetype(ret1,N_FRAME);
  set_n_tuplevalue(ret1,2,ret); // Управляющий кортеж
//  add_critreg(ret);
  // добавим копии каждого слота
  l=get_n_tuplelen(p);
  for(i=0;i<l;i++) {
       if(get_n_tuplevalue(p,i)!=NIL){
       ret2=copy_n_tuple(get_n_tuplevalue(p,i));
       set_n_tuplevalue(ret,i,ret2);
       };
  };
//  del_critreg(CR_LAST);
//  del_critreg(CR_LAST);
//  del_critreg(CR_LAST);
//  del_critreg(CR_LAST);
  return(ret1);
};

//
void add_n_frame_slot(PNODE p1, PNODE n)
{
 PNODE p, t, v, f;
 int i, l;
// PSNODE k;
   p=get_n_tuplevalue(p1,2);
//   print(p);
   if(is_n_symbol(n)) {
        // создаем новый слот
        f=make_n_tuple(5); // 0 - наименование 1 - тип 2 - значение 3 - демон чтения 4 -демон записи
        fill_n_tuple(f,NIL);
        // добавим
        set_n_tuplevalue(f,0,n); // добавим имя

//        pool_count_up(p);
        // Изменение длины в обход resize_n_tuple ???
//        win_printf("%d\r\n",get_n_tuplelen(p));
        if(p!=NIL) {
        l=get_n_tuplelen(p);
        v=make_n_tuple(l+1);
        fill_n_tuple(v,NIL);
        for(i=0;i<l;i++){
//           print(get_n_tuplevalue(p,i));
           t=get_n_tuplevalue(p,i);
//           print(t);
           set_n_tuplevalue(v,i,t);
        };
        set_n_tuplevalue(v,l-1,f); // добавим слот
        } else {
        v=make_n_tuple(1+1);
        fill_n_tuple(v,NIL);
        l=0;
        set_n_tuplevalue(v,l,f); // добавим слот
        };
//        win_printf("v=");
//        print(v);

//        pool_count_down(p);

//        v=resize_n_tuple(p, get_n_tuplelen(p)+1);
//        pool_count_up(v);
//        print(v);
//        k=get_p_node(p);
//        p=v;
//        set_p_node(p, (long)(get_p_node(v)));
//        set_p_node(v, (long)(k));

        // теперь в старом узле то что и было до этого
//        print(p1);

        set_n_tuplevalue(p1,2,v);

//        print(p1);


//        print(v);
//        win_printf("%d\r\n",get_n_tuplelen(v));
        l=get_n_tuplelen(v);
//        win_printf("\r\n %d \r\n",l);  //get_n_tuplelen(v)
//        print(t);
//        pool_count_down(p);

//        pool_count_down(v);
//        print(v);
//        print(v);
//        print(p1);
//        del_critreg(CR_LAST);
//        del_critreg(CR_LAST);
   };
};

void del_n_frame_slot(PNODE p1, PNODE n)
{
 PNODE p, t, v;
// PSNODE k;
 long i,j;
   p=get_n_tuplevalue(p1,2);
   if(get_n_tuplelen(p)-1>0) {
//        add_critreg(n);
//        add_critreg(p);
        // Изменение длины в обход resize_n_tuple ???
        v=resize_n_tuple(p, get_n_tuplelen(p)-1);
        set_n_tuplevalue(p1,2,v);
        p=v;
        j=1;
        for(i=1;i<get_n_tuplelen(p);i++) {
            if(n_symboleq(get_n_tuplevalue(get_n_tuplevalue(p,i),0),n)!=FALSE) {

            } else {
               set_n_tuplevalue(v,j,get_n_tuplevalue(p,i));
               j++;
            };
        };
        if(i>j) {
//                k=get_p_node(p);
//                set_p_node(p,(long)(get_p_node(v)));
//                set_p_node(v,(long)(k));
        };
//        del_critreg(CR_LAST);
//        del_critreg(CR_LAST);
   };
};

void set_n_frame_slot_value(PNODE p1, PNODE n, PNODE v)
{
 PNODE p, p2;
 long i;

   p=get_n_tuplevalue(p1,2);
   for(i=0;i<get_n_tuplelen(p)-1;i++) {
//   print(n);
//   p2=get_n_tuplevalue(p,i);
//   print(p2);
//   print(get_n_tuplevalue(p2,0));
//   print(n);
//   print(get_n_tuplevalue(get_n_tuplevalue(p,i),2));print(n);
//   print(get_n_tuplevalue(get_n_tuplevalue(p,i),3));print(n);
       if(n_symboleq(get_n_tuplevalue(get_n_tuplevalue(p,i),0),n)!=FALSE) {
           // добавить проверку типа
           if((is_n_func(v)) || (is_n_closure(v))) {
                set_n_tuplevalue(get_n_tuplevalue(p,i),1,TRUE); // добавим тип
           } else {
                set_n_tuplevalue(get_n_tuplevalue(p,i),1,NIL); // добавим тип
           };
           set_n_tuplevalue(get_n_tuplevalue(p,i),2,v); // добавим значение
           break;
       };
   };
};

PNODE get_n_frame_slot_value(PNODE p1, PNODE n)
{
 long i;
 PNODE p, ret;
   ret=NIL;

//   print(p);
//   print(n);

   p=get_n_tuplevalue(p1,2);
//   win_printf("%d\r\n",get_n_tuplelen(p));
   for(i=0;i<get_n_tuplelen(p)-1;i++) {
       if(n_symboleq(get_n_tuplevalue(get_n_tuplevalue(p,i),0),n)!=FALSE) {
           // добавить проверку типа
           ret=get_n_tuplevalue(get_n_tuplevalue(p,i),2); // получим значение
           break;
       };
   };
   return(ret);
};


PNODE get_n_frame_slot_type(PNODE p1, PNODE n)
{
 long i;
 PNODE p, ret;
   ret=NIL;
   p=get_n_tuplevalue(p1,2);
   for(i=1;i<get_n_tuplelen(p);i++) {
       if(n_symboleq(get_n_tuplevalue(get_n_tuplevalue(p,i),0),n)!=FALSE) {
           // добавить проверку типа
           ret=get_n_tuplevalue(get_n_tuplevalue(p,i),1); // получим значение
           break;
       };
   };
   return(ret);
};

PNODE find_n_frame_slot(PNODE p1, PNODE n)
{
 long i;
 PNODE p, ret;
   ret=NIL;
   p=get_n_tuplevalue(p1,2);
   for(i=1;i<get_n_tuplelen(p);i++) {
//     print(get_n_tuplevalue(get_n_tuplevalue(p,i),0));print(n);
       if(n_symboleq(get_n_tuplevalue(get_n_tuplevalue(p,i),0),n)!=FALSE) {
           // добавить проверку типа
           ret=get_n_tuplevalue(p,i);
           break;
       };
   };
   return(ret);
};

PNODE find_n_frame_slot_by_num(PNODE p1, int l)
{
 long i;
 PNODE p, ret;
   ret=FALSE;
   p=get_n_tuplevalue(p1,2);
//   win_printf(">>> %d\r\n",l);
   if((l>=0) && (l<get_n_tuplelen(p))){
      ret=get_n_tuplevalue(p,l);
//      print(ret);
   };
   return(ret);
};

int get_n_slot_num(PNODE p1)
{
 int l;
 PNODE p;
   p=get_n_tuplevalue(p1,2);
   l=get_n_tuplelen(p);
   return(l);
};

