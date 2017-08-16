#define ARCHITECT16 1
//#define DEBUG_GC

#include <stdlib.h>
#include "MEMMAN.h"
#include "objects.h"
#include "print.h"
#include "win_printf.h"

int command_code_int;
int command_code_flag;

#define _DEBUG 1

extern s_int global_debug_code;

// Это новый менеджер памяти базирующийся на том постулате, что паспорт участка памяти это одно, а вот точ то хранится в этой памяти - совсем другое.
// таким образом отделим мух от котлет - объекты в памяти это вложенная информация в один объект - паспорт участка.
// таким образом структура заголовка участка памяти:
//  байт заголовка <7 бит использования> <3 бит статус свопа 0 -  сброшен 1 - не сброшен> <2 бит функциональности 0 - объект памяти 1 - служебный объект> <1-0 биты количество байт длины участка 00 - запрещ ,01 - 1, 10 - 2, 11 - 4>
//  4 байта  длины объекта
//  2 байта счетчика ссылок (впервые делаю систему со счетчиком ссылок)
//  [объект]  < передается именно этот указатель

//unsigned char mem_array[0x2000];

//Пул памяти сборщика мусора
//длина общего блока памяти
s_int gbuffer_data_length;

//указатель на текущую позицию
s_int gbuffer_data_p;

//указатель на блок памяти
char *gbuffer_data;

// концевой адрес памяти
s_int gbuffer_data_end;

s_int gbuffer_free_pnum; // счетчик свободных ячеек в массиве

//промежуточная функция печати
//void win_printf(char *format, ...);

//определяет допустимый размер общей памяти
s_int memfull()
{
  return(64535);
};

//реализация функции memcreate
int memcreate(s_int size, int debug)
{
  s_int i;

  gbuffer_data_length=size;

  //выделение пулов памяти!!!
#ifdef __BORLANDC__
  gbuffer_data=(char *)malloc(gbuffer_data_length);
#else
#ifdef __LINUX
  gbuffer_data=(char *)malloc(gbuffer_data_length);
#else
  gbuffer_data=(char *)(0x81000000);
#endif
#endif

//  memset(gbuffer_data,size,0);

//  gbuffer_data=mem_array;
  if(gbuffer_data==NULL) {
         win_printf("Авария! Нет памяти для создания пула данных!\n");
         return(-1);
  };
  // обязательная очистка
//  for (i=0; i<gbuffer_data_length; i++) {
//     gbuffer_data[i]=0;
//  };
  gbuffer_data_p=0;
  gbuffer_data_end=(s_int)(gbuffer_data+gbuffer_data_length);
  gbuffer_free_pnum=0;

  if(debug) {
    win_printf(
               " gbuffer_data: beg %lx end %lx, size %ld \r\n",
                 gbuffer_data,(s_int)gbuffer_data+gbuffer_data_length,gbuffer_data_length);
  };
  printf("!!!!\n");
  return(0);
};

#ifdef __BORLANDC__
#define uart0Getch getch
#endif
#ifdef __LINUX
#define uart0Getch
#endif

#ifndef __LINUX
unsigned char get_char()
{
 int ch;
      while ((ch = uart0Getch()) < 0) {
	    }
	 return ch;
};
#endif

void stat_info(void)
{
  s_int pos, i;
  TMEM_HEADER_REC p;
  s_int len;
  char *s;
  win_printf("stat_info: gbuffer_data_p=%x %x\r\n",gbuffer_data_p,sizeof(MEM_HEADER_REC));
  for (pos=0; pos<gbuffer_data_p;) {
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
//    s=(char *)p;
//    for(i=0;i<sizeof(MEM_HEADER_REC);i++) {
//      win_printf(" %x",s[i]);
//    };
//    win_printf("\r\n");

//    len=((long)(*((long *)((long)p+1))) & BLOCK_VALUE_LENGTH_MASK);
    len=get_block_value_length((char *)p);

//    win_printf("%x %x len=%d, %d, %d, %d, %x, %d\r\n",p,len,get_block_length(p),get_block_func(p),get_block_swap(p),get_block_use(p),p->len,p->count);
//    get_char();

    pos=pos+sizeof(MEM_HEADER_REC)+len;
  };
  win_printf("<end>\r\n");
};

void stat_info_new(void)
{
  s_int pos, i;
  TMEM_HEADER_REC p;
  s_int len, score;
  int sc1, sc2;
  int sc1_n, sc2_n;
  char *s;
  char *ret;
  int c;

  sc1=0;
  sc2=0;
  sc1_n=0;
  sc2_n=0;
  score=0;
  win_printf("<stat_info: gbuffer_data_p=%x gbuffer_data=%x size MEM_HEADER_REC %d>\r\n",gbuffer_data_p,gbuffer_data,sizeof(MEM_HEADER_REC));
  for (pos=0; pos<gbuffer_data_p;) {
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
    len=get_block_value_length((char *)p);
//    score=score+1;
    if (get_block_use((char *)p)==0) {
//      sc1=sc1+p->len;
      sc1=sc1+len;
      sc1_n=sc1_n+1;
    } else {
//      sc2=sc2+p->len;
      sc2=sc2+len;
      sc2_n=sc2_n+1;
    };
//    win_printf("Ptr=%06x len=%04x b_len=%03d b_func=%02d b_swap=%02d b_use=%02d p_len=%04x p_cnt=%04d\r\n",p,len,get_block_length(p),get_block_func(p),get_block_swap(p),get_block_use(p),p->len,p->count);
    if(get_block_use((char *)p)==0){
//    getch();
    } else {
    if(len>3000){
    win_printf("\r\nPtr=%06x(%04x) len=%d b_len=%03d b_func=%02d b_swap=%02d b_use=%02d p_cnt=%04d\r\n",p,pos,len,get_block_length((char *)p),get_block_func((char *)p),get_block_swap((char *)p),get_block_use((char *)p),get_block_value_count((char *)p));
    ret=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
    win_printf("\r\n %d \r\n",get_n_type(ret));
    print_i(ret);
    };

//    win_printf("pos=%d gbuffer_data_p=%d score=%d next pos=%d\r\n",pos,gbuffer_data_p,score,pos+sizeof(MEM_HEADER_REC)+len);
//    win_printf("\rPtr=%06x(%04x) len=%d b_len=%03d b_func=%02d b_swap=%02d b_use=%02d p_cnt=%04d\r\n",p,pos,len,get_block_length((char *)p),get_block_func((char *)p),get_block_swap((char *)p),get_block_use((char *)p),get_block_value_count((char *)p));

//    ret=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
//    print_i(ret);
    };
//    getch();

//    c=getch();
//    if(c==32)print(ret);

    pos=pos+sizeof(MEM_HEADER_REC)+len;
    score++;
  };
  win_printf("<Used %d %d %d Free %d %d %d>\r\n",sc2,sc2_n,sc2_n*sizeof(MEM_HEADER_REC)+sc2,sc1,sc1_n,sc1_n*sizeof(MEM_HEADER_REC)+sc1);
};

void stat_info_use(void)
{
  s_int pos, i;
  TMEM_HEADER_REC p;
  s_int len, score;
  int sc1, sc2;
  int sc1_n, sc2_n;
  char *s;
  char *ret;
  int c;

  sc1=0;
  sc2=0;
  sc1_n=0;
  sc2_n=0;
  score=0;
//  win_printf("<stat_info: gbuffer_data_p=%x gbuffer_data=%x size MEM_HEADER_REC %d>\r\n",gbuffer_data_p,gbuffer_data,sizeof(MEM_HEADER_REC));
  for (pos=0; pos<gbuffer_data_p;) {
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
    len=get_block_value_length((char *)p);
    if (get_block_use((char *)p)==0) {
      sc1=sc1+len;
      sc1_n=sc1_n+1;
    } else {
      sc2=sc2+len;
      sc2_n=sc2_n+1;
    };
/*
    if(len>3000){
    win_printf("\rPtr=%06x(%04x) len=%d b_len=%03d b_func=%02d b_swap=%02d b_use=%02d p_cnt=%04d\r\n",p,pos,len,get_block_length((char *)p),get_block_func((char *)p),get_block_swap((char *)p),get_block_use((char *)p),get_block_value_count((char *)p));
    ret=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
    print_i(ret);
    getch();
    };
*/
//    win_printf("Ptr=%06x len=%04x b_len=%03d b_func=%02d b_swap=%02d b_use=%02d p_len=%04x p_cnt=%04d\r\n",p,len,get_block_length(p),get_block_func(p),get_block_swap(p),get_block_use(p),p->len,p->count);
    if(get_block_use((char *)p)!=0){
    if(pos>=6073){
//    win_printf("pos=%d gbuffer_data_p=%d score=%d next pos=%d\r\n",pos,gbuffer_data_p,score,pos+sizeof(MEM_HEADER_REC)+len);
    win_printf("\r\nPtr=%06x(%05d) len=%d b_len=%03d b_func=%02d b_swap=%02d b_use=%02d p_cnt=%04d\r\n",p,pos,len,get_block_length((char *)p),get_block_func((char *)p),get_block_swap((char *)p),get_block_use((char *)p),get_block_value_count((char *)p));
    ret=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
    print(ret);
    };
    };
    pos=pos+sizeof(MEM_HEADER_REC)+len;
    score++;
  };
//  win_printf("<Used %d %d %d Free %d %d %d>\r\n",sc2,sc2_n,sc2_n*sizeof(MEM_HEADER_REC)+sc2,sc1,sc1_n,sc1_n*sizeof(MEM_HEADER_REC)+sc1);
};

void info_block(s_int pos_new)
{
  s_int pos, i;
  TMEM_HEADER_REC p;
  s_int len, score;
  char *s;
  char *ret;
  int c;

  for (pos=0; pos<gbuffer_data_p;) {
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
    len=get_block_value_length((char *)p);
    if(pos==pos_new){
       win_printf("\rBlock len=%d use=%d p_cnt=%d\r\n",pos,len,get_block_use((char *)p),get_block_value_count((char *)p));
       ret=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
       print_i(ret);
    };
//    getch();
    pos=pos+sizeof(MEM_HEADER_REC)+len;
  };
};

/*
void compact_free_block_old(void)  // компактифицируются блоки
{
  s_int pos, pos_next;
  TMEM_HEADER_REC p, p_next;
  s_int len, len_c, ret;

  for (pos=0; pos<gbuffer_data_p;) {
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
    if (get_block_use(p)==0) {

    ret=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
    print_i(ret);
       // флаг сброшен
       // смотрим, пуст ли следующий блок
       pos_next=pos;
       len_c=0;
l:
       p_next=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos_next);
       pos_next=(pos_next+sizeof(MEM_HEADER_REC)+p_next->len);
       win_printf("\r\nfree pos_next=%d(%x) gbuffer_data_p=%d(%x) \r\n",pos_next,pos_next,gbuffer_data_p,gbuffer_data_p);
       if(pos_next<gbuffer_data_p){
         if (get_block_use(p_next)==0) {
            len_c=len_c+sizeof(MEM_HEADER_REC)+p_next->len;
            // если блок существует  и его адрес не превышает (s_int)gbuffer_data+gbuffer_data_p
            goto l;
         };
       };
       if (pos_next>pos) {
          // компактируем блоки
          p->len=p->len+len_c;
          win_printf("len=%d    ",p->len);
          pos=pos_next;
       } else {
          pos=pos+sizeof(MEM_HEADER_REC)+p->len;
       };
       win_printf("\r\nfree pos=%d(%x) gbuffer_data_p=%d(%x) \r\n",pos,pos,gbuffer_data_p,gbuffer_data_p);
       getch();
    } else {
//  win_printf("\r\nUsed pos=%d(%x) gbuffer_data_p=%d(%x) \r\n",pos,pos,gbuffer_data_p,gbuffer_data_p);
    pos=pos+sizeof(MEM_HEADER_REC)+p->len;
    };
  };
  win_printf("\r\n");
};
*/

int s_flag;
TMEM_HEADER_REC p_old;

void compact_free_block(void)  // компактифицируются блоки
{
  s_int pos, pos_next;
  TMEM_HEADER_REC p, p_next;
  s_int len_c, ret;
  int flag, pos_old, len, sc;
  char * ret1;

  flag=0;
  len=0;
  sc=0;
//  win_printf("\r\nCompact free block\r\n");
  for (pos=0; pos<gbuffer_data_p;) {
    p_old=p;
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
//    if(s_flag){
//        win_printf("\r\nPtr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), (TMEM_HEADER_REC)((s_int)gbuffer_data+gbuffer_data_p));
//    };
//    if((int)p==0x99bc25) {
//     win_printf("\r\n-->Ptr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), (TMEM_HEADER_REC)((s_int)gbuffer_data+gbuffer_data_p));
//    };
//    if((int)p==0x998853) {
//     win_printf("\r\n-->>Ptr=%06x len=%d use %d \r\n",p_old,get_block_value_length((char *)p_old),get_block_use((char *)p_old));
//     win_printf("\r\n-->>Ptr=%06x len=%d use %d \r\n",p,get_block_value_length((char *)p),get_block_use((char *)p));
//     ret1=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
//     print(ret1);
//    };
//    if(get_block_value_length((char *)p)>3000) {
//     win_printf("\r\n-->Ptr=%06x len=%d use %d\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p));
//    ret1=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
//    print(ret1);
//    };
    switch(flag){
      case 0:
         if (get_block_use((char *)p)==0) {
            // блок не используется.
            len=get_block_value_length((char *)p);
            flag=1;
            pos_old=pos;
            sc=1;
         };
      break;
      case 1:
         if (get_block_use((char *)p)==0) {
            // блок не используется.
            flag=1;
            len=len+get_block_value_length((char *)p);
            sc++;
         } else {
            if(sc>1){
//              win_printf("\r\nNext1 Ptr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p));
              p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos_old);
              set_block_length_byte((char *)p);
              set_block_value_length((char *)p,(len+(sc-1)*sizeof(MEM_HEADER_REC)));
              set_block_value_count((char *)p,0);
              clear_block_use((char *)p);
              if((pos_old+sizeof(MEM_HEADER_REC)+get_block_value_length((char *)p))!=pos){
              win_printf("\r\nError Ptr=%06x len=%d use %d  len %d sizeof(MEM_HEADER_REC) %d\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), len, sizeof(MEM_HEADER_REC));
              };
//              win_printf("\r\nOld Ptr=%06x len=%d use %d  len %d sizeof(MEM_HEADER_REC) %d\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), len, sizeof(MEM_HEADER_REC));
              p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
//              win_printf("\r\nNext2 Ptr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p));
//////              pos=pos_old;
//              p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
//              win_printf("\r\nNew Ptr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p));
            } else {
            };
            flag=0;
            sc=0;
         };
      break;
    };
    pos=pos+sizeof(MEM_HEADER_REC)+get_block_value_length((char *)p);
  };
  if(sc>1){
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos_old);
    set_block_length_byte((char *)p);
    clear_block_use((char *)p);
    set_block_value_length((char *)p,len+(sc-1)*sizeof(MEM_HEADER_REC));
    set_block_value_count((char *)p,0);
//    win_printf("\r\nOld Ptr=%06x len=%d use %d  len %d sizeof(MEM_HEADER_REC) %d\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), len, sizeof(MEM_HEADER_REC));
  } else {
  };
//  win_printf("\r\nEnd Compact free block\r\n");
};

TMEM_HEADER_REC get_free_block(s_int l1)  // передается длина, ищется максимально похожий по длине блок
{
  s_int pos, pos_next;
  TMEM_HEADER_REC p, p_next;
  s_int ret, i;
  int len,l;
  char *ret1;

  l=l1;

  for (pos=0; pos<gbuffer_data_p;) {
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
    if (get_block_use((char *)p)==0) {
      len=get_block_value_length((char *)p);
      if (len>=l) {
         // нашли
         if ((int)((int)(len-(l+sizeof(MEM_HEADER_REC)))-(int)(sizeof(s_int)+sizeof(MEM_HEADER_REC)))>0) {
           // делим блок так как есть место для самого маленького атома
           pos_next=(pos+sizeof(MEM_HEADER_REC)+l);
           p_next=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos_next);
           set_block_length_byte((char *)p_next);
           clear_block_use((char *)p_next);
           set_block_value_length((char *)p_next,(int)(get_block_value_length((char *)p)-(l+sizeof(MEM_HEADER_REC))));
           set_block_value_count((char *)p_next,0);
           set_block_value_length((char *)p,l);
//           if()
//           win_printf("\r\nOld Ptr=%06x len=%d use %d  len %d sizeof(MEM_HEADER_REC) %d\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), len, sizeof(MEM_HEADER_REC));
         };
         // иначе всю длину возвращаем. ))))
         // очистить
         ret1=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
         for(i=0;i<l;i++){
             ret1[i]=0;
         };
         return(p);
      };
    };
    pos=pos+sizeof(MEM_HEADER_REC)+get_block_value_length((char *)p);
  };
  return(NULL);
};

#define NEW_GC 1

// это функция проходит по памяти и считывает размаркированные блоки
void collect_free_block(void)  // компактифицируются блоки
{
  s_int pos, pos_next;
  TMEM_HEADER_REC p, p_next;
  s_int len_c, ret;
  int flag, pos_old, len, sc;

  flag=0;
  len=0;
  sc=0;
//  win_printf("\r\nCollect free block\r\n");
  for (pos=0; pos<gbuffer_data_p;) {
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
//    if(ret==0x99bc25) {
//     win_printf("\r\nPtr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), (TMEM_HEADER_REC)((s_int)gbuffer_data+gbuffer_data_p));
//    };
#ifdef NEW_GC
    if (get_block_use((char *)p)==0) {
      // блок не используется.
      len=get_block_value_length((char *)p);
    } else {
      if(is_block_mark((char *)p)) {
        // блок помечен оставляем его как есть
      } else {
//    if((int)p==0x998853) {
//     win_printf("\r\nFree Ptr=%06x len=%d use %d \r\n",p,get_block_value_length((char *)p),get_block_use((char *)p));
//    };
//    win_printf("\r\nPtr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), (TMEM_HEADER_REC)((s_int)gbuffer_data+gbuffer_data_p));
        // блок не помечен - надо его освободить несмотря ни на что.
        clear_block_use((char *)p);
      };
    };
#else
      if(is_block_mark((char *)p)) {
        // блок помечен оставляем его как есть
        len=get_block_value_length((char *)p);
      } else {
        if (get_block_use((char *)p)==0) {
        // блок не помечен - надо его освободить несмотря ни на что.
        clear_block_use((char *)p);
        };
      };
#endif
    pos=pos+sizeof(MEM_HEADER_REC)+get_block_value_length((char *)p);
  };
//  win_printf("\r\nEnd Collect free block\r\n");
};

void print_free_block(void)  // передается длина, ищется максимально похожий по длине блок
{
  s_int pos, pos_next;
  TMEM_HEADER_REC p, p_next;
  s_int ret;

  for (pos=0; pos<gbuffer_data_p;) {
    p=(TMEM_HEADER_REC)((s_int)gbuffer_data+pos);
    if (get_block_use((char *)p)==0) {
      ret=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
      print_i(ret);
      win_printf("\r\nfree pos=%d(%x) gbuffer_data_p=%d(%x) len %d\r\n",pos,pos,gbuffer_data_p,gbuffer_data_p,get_block_value_length((char *)p));
    };
    pos=pos+sizeof(MEM_HEADER_REC)+get_block_value_length((char *)p);
  };
};

//убиение главного пула и пулов сборщика мусора
void memdelete()
{
//  if(gbuffer_data!=NULL) free(gbuffer_data);
};

//функция выделяющая память для NODE
char* pool_memmake(s_int length)
{
  char * ret;
//  char * p;
  s_int i, n, len;
  short int *p1;
  TMEM_HEADER_REC p;

  s_flag=0;
  i=0;
  n=0;
  while(i!=10) {
     switch(i) {
       case 0:
         if((gbuffer_data_p+length+sizeof(MEM_HEADER_REC))>gbuffer_data_length) {
//           win_printf(">>\r\n");
           if(n>0) {
//              s_flag=1;
//              gc();
//              p=get_free_block(length);
//              if (p!=NULL) {
//              };
//              stat_info_new();
              //закончилась память
              //идет либо перераспределение памяти, либо останов VM
//              win_printf("Закончилась память данных!\n");
              win_printf("Data pool end!\r\n");
//              stat_info_use();
//              exit_vm(-2);
              ret=NULL;
              while(1){
              };
           };
           n=0;
           i=5;
         } else {
           i=9;
         };
       break;
       case 5:
         // память закончилась, следовательно, надо с самого начала искать блоки которые свободны
         p=get_free_block(length);
         if (p!=NULL) {
           ret=(char *)((s_int)p+sizeof(MEM_HEADER_REC));
           set_block_value_count((char *)p,0);
           set_block_use((char *)p);
           i=10;
         } else {
           if(n==0){
             gc();
             i=5;
             n=1;
           } else {
             i=0;
           };
         };
       break;
       case 9:
         // ищем узел под который можно запихать ячейку
         //выделим столько сколько просили
         p=(TMEM_HEADER_REC)((s_int)gbuffer_data+gbuffer_data_p);
//         if(p==0x99bc25) {
//     win_printf("\r\nPtr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), (TMEM_HEADER_REC)((s_int)gbuffer_data+gbuffer_data_p));
//         };
/*
         if (length<256) {
            set_block_length_byte((char *)p);
            set_block_use((char *)p);
            set_block_value_length((char *)p,length);
         } else if (length<65536) {
            set_block_length_word((char *)p);
            set_block_use((char *)p);
            set_block_value_length((char *)p,length);
         } else if (length>65535) {
            set_block_length_dword((char *)p);
            set_block_use((char *)p);
            set_block_value_length((char *)p,length);
         };
*/
         set_block_length_dword((char *)p);
         set_block_use((char *)p);
         set_block_value_length((char *)p,length);
         set_block_value_count((char *)p,0);
//         if(length>300) {
//      win_printf("\r\nPtr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), (TMEM_HEADER_REC)((s_int)gbuffer_data+gbuffer_data_p));
//         };

        ret=(char *)((s_int)gbuffer_data+gbuffer_data_p+sizeof(MEM_HEADER_REC));
        memset(ret,length,0);
        gbuffer_data_p=gbuffer_data_p+length+sizeof(MEM_HEADER_REC);
/*
         if(ret==0x99bc25) {
     win_printf("\r\nPtr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), (TMEM_HEADER_REC)((s_int)gbuffer_data+gbuffer_data_p));
         };
         if(((s_int)gbuffer_data+gbuffer_data_p)==0x99bc25) {
     win_printf("\r\nPtr=%06x len=%d use %d gbuffer_data_p %x\r\n",p,get_block_value_length((char *)p),get_block_use((char *)p), (TMEM_HEADER_REC)((s_int)gbuffer_data+gbuffer_data_p));
         };
*/
         i=10;
       break;
     };
  };

  return(ret);
};

//функция компактификации и выдачи статистики
void gc(void)
{
#ifdef DEBUG_GC
  win_printf("Garbage collector Start\r\n");
//  stat_info_new();     // выводим статистикику.
#endif
  gc_all();
  compact_free_block(); // запускаем компактификацию
#ifdef DEBUG_GC
  stat_info_new();     // выводим статистикику.
  win_printf("Garbage collector End\r\n");
#endif
};

// реальные изменения счетчиков
// использовать только внутри сборщика мусора и оберточных функций изменения счетчика

void pool_count_up_int(char *n)
{
  TMEM_HEADER_REC p;
    p=(TMEM_HEADER_REC)((s_int)n-sizeof(MEM_HEADER_REC));
    // проверим на вечность
    if(get_block_value_count((char *)p)!=65535){
      // проверить на использование этого элемента
      if(get_block_value_count((char *)p)==0){
          set_block_use((char *)p);
      };
      set_block_value_count((char *)p,(get_block_value_count((char *)p)+1));
    };
};

int pool_count_down_int(char *n)
{
  TMEM_HEADER_REC p;
//  s_int ret;
  int ret=0;

    p=(TMEM_HEADER_REC)((s_int)n-sizeof(MEM_HEADER_REC));
    // проверим на вечность
    if(get_block_value_count((char *)p)!=65535){
    // проверить на использование этого элемента
      if(get_block_value_count((char *)p)!=0){
        set_block_value_count((char *)p,(get_block_value_count((char *)p)-1));
        if(get_block_value_count((char *)p)==0) {
            command_code_flag=1;
            clear_block_use((char *)p);
        } else {
          ret=3;
        };
      } else {
        ret=2;
      };
    } else {
      ret=1;
    };
    return(ret);
};

//функция увеличивает счетчик обращений к элементу
void pool_count_up(char *n)
{
    pool_count_up_int(n);
};

void pool_count_down(char *n)
{
    if(pool_count_down_int(n)==0){
       gc_mark(n);
    };
};

//функция устанавливает вечный аргумент
void pool_count_always(char *n)
{
  TMEM_HEADER_REC p;
    p=(TMEM_HEADER_REC)(((s_int)(n))-(s_int)(sizeof(MEM_HEADER_REC)));
    // проверить на использование этого элемента
    set_block_value_count((char *)p,65535);
};

//функция распомечает ячейку элемента сразу (это опасно так как можно попасть под сборщик мусора)
void pool_memfree(char *n)
{
  TMEM_HEADER_REC p;
    p=(TMEM_HEADER_REC)((s_int)n-sizeof(MEM_HEADER_REC));
    // проверить на использование этого элемента
    clear_block_use((char *)p);
};

s_int get_pool_len(char *n)
{
  TMEM_HEADER_REC p;
    p=(TMEM_HEADER_REC)((s_int)n-sizeof(MEM_HEADER_REC));
    return(get_block_value_length((char *)p));
};

void copy_array_to_struct(char *n,TMEM_HEADER_REC p)
{
 char *s;
 int i;
    s=(char *)p;
    for(i=0;i<sizeof(MEM_HEADER_REC);i++) {
      s[i]=n[i];
    };
};

void copy_struct_to_array(char *n,TMEM_HEADER_REC p)
{
 char *s;
 int i;
    s=(char *)p;
    for(i=0;i<sizeof(MEM_HEADER_REC);i++) {
      n[i]=s[i];
    };
};

unsigned short int get_block_length(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 return(((p.header) & BLOCK_LENGTH_MASK));
};

void set_block_length(char *p1,unsigned short int a)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) | (a & BLOCK_LENGTH_MASK));
 copy_struct_to_array(p1,&p);
};

void set_block_length_byte(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) | BLOCK_LENGTH_BYTE);
 copy_struct_to_array(p1,&p);
};

void set_block_length_word(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) | BLOCK_LENGTH_WORD);
 copy_struct_to_array(p1,&p);
};

void set_block_length_dword(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) | BLOCK_LENGTH_DWORD);
 copy_struct_to_array(p1,&p);
};

unsigned int get_block_value_length(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 return(p.len1 & BLOCK_VALUE_LENGTH_MASK);
};

void set_block_value_length(char *p1,unsigned int a)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.len1=((s_int)(a & BLOCK_VALUE_LENGTH_MASK));
 copy_struct_to_array(p1,&p);
};

unsigned short int get_block_value_count(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 return(p.count1 & BLOCK_VALUE_COUNT_MASK);
};

void set_block_value_count(char *p1,unsigned short int a)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.count1=((short int)(a & BLOCK_VALUE_COUNT_MASK));
 copy_struct_to_array(p1,&p);
};

int is_block_length_byte(char *p)
{
// MEM_HEADER_REC p;
// copy_array_to_struct(p1,&p);
 return(get_block_length(p) == BLOCK_LENGTH_BYTE);
};

int is_block_length_word(char *p)
{
// MEM_HEADER_REC p;
// copy_array_to_struct(p1,&p);
 return(get_block_length(p) == BLOCK_LENGTH_WORD);
};

int is_block_length_dword(char *p)
{
// MEM_HEADER_REC p;
// copy_array_to_struct(p1,&p);
 return(get_block_length(p) == BLOCK_LENGTH_DWORD);
};

unsigned short int get_block_func(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 return((p.header) & BLOCK_FUNC_MASK);
};

void set_block_func(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) & BLOCK_FUNC_NOT_MASK) | BLOCK_FUNC_MASK;
 copy_struct_to_array(p1,&p);
};

void clear_block_func(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) & BLOCK_FUNC_NOT_MASK);
 copy_struct_to_array(p1,&p);
};

int is_block_func(char *p)
{
// MEM_HEADER_REC p;
// copy_array_to_struct(p1,&p);
 return(get_block_func(p) == BLOCK_FUNC);
};

unsigned short int get_block_use(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 return((p.header) & BLOCK_USE_MASK);
};

void set_block_use(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) & BLOCK_USE_NOT_MASK) | BLOCK_USE_MASK;
 copy_struct_to_array(p1,&p);
};

void clear_block_use(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) & BLOCK_USE_NOT_MASK);
 copy_struct_to_array(p1,&p);
};

int is_block_use(char *p)
{
// MEM_HEADER_REC p;
// copy_array_to_struct(p1,&p);
 return(get_block_use(p) == BLOCK_USE);
};


unsigned short int get_block_mark(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 return((p.header) & BLOCK_MARK_MASK);
};

void set_block_mark(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) & BLOCK_MARK_NOT_MASK) | BLOCK_MARK_MASK;
// p.header+=0x8000;
 copy_struct_to_array(p1,&p);
};

void clear_block_mark(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) & BLOCK_MARK_NOT_MASK);
 copy_struct_to_array(p1,&p);
};

int is_block_mark(char *p)
{
// MEM_HEADER_REC p;
// copy_array_to_struct(p1,&p);
 return(get_block_mark(p) == BLOCK_MARK);
};

unsigned short int get_block_ptr(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 return((p.header) & BLOCK_PTR_MASK);
};

void set_block_ptr(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) & BLOCK_PTR_NOT_MASK) | BLOCK_PTR_MASK;
 copy_struct_to_array(p1,&p);
};

void clear_block_ptr(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 p.header=((p.header) & BLOCK_PTR_NOT_MASK);
 copy_struct_to_array(p1,&p);
};

int is_block_ptr(char *p)
{
 return(get_block_ptr(p) == BLOCK_PTR);
};

unsigned short int get_block_swap(char *p1)
{
 MEM_HEADER_REC p;
 copy_array_to_struct(p1,&p);
 return((p.header) & BLOCK_SWAP_MASK);
};

/*
#define get_block_func(p) (((p)->header) & BLOCK_FUNC_MASK)
#define set_block_func(p) ((p)->header=(((p)->header) & BLOCK_FUNC_NOT_MASK) | BLOCK_FUNC_MASK))
#define clear_block_func(p) ((p)->header=(((p)->header) & BLOCK_FUNC_NOT_MASK))
#define is_block_func(p)  (get_block_func(p) == BLOCK_FUNC)

#define get_block_swap(p) (((p)->header) & BLOCK_SWAP_MASK)
#define set_block_swap(p) ((p)->header=(((p)->header) & BLOCK_SWAP_NOT_MASK) | BLOCK_SWAP_MASK))
#define clear_block_swap(p) ((p)->header=(((p)->header) & BLOCK_SWAP_NOT_MASK))
#define is_block_swap(p)  (get_block_swap(p) == BLOCK_SWAP)

#define get_block_use(p) (((p)->header) & BLOCK_USE_MASK)
#define set_block_use(p) ((p)->header=((((p)->header) & BLOCK_USE_NOT_MASK) | BLOCK_USE_MASK))
#define clear_block_use(p) ((p)->header=(((p)->header) & BLOCK_USE_NOT_MASK))
#define is_block_use(p)  (get_block_use(p) == BLOCK_USE)

//#define get_block_value_length(p) ((long)(*((long *)((long)p+1))) & BLOCK_VALUE_LENGTH_MASK)
//#define set_block_value_length(p,a) (*((long *)((long)p+1))=((((long)p+1) & BLOCK_VALUE_LENGTH_NOT_MASK) | ((long)a & BLOCK_VALUE_LENGTH_MASK)))

#define get_block_value_length(p) ((p)->len & BLOCK_VALUE_LENGTH_MASK)
#define set_block_value_length(p,a) ((p)->len=((s_int)(a & BLOCK_VALUE_LENGTH_MASK)))

#define get_block_value_count(p) ((p)->count & BLOCK_VALUE_COUNT_MASK)
#define set_block_value_count(p,a) ((p)->count=((short int)(a & BLOCK_VALUE_COUNT_MASK)))
*/


