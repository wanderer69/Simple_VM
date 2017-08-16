#define ARCHITECT16 1

#include <stdio.h>
#include <stdlib.h>
#include "MEMMAN.h"
#include "vm.h"
#include "objects.h"
#include "print.h"
#include "win_printf.h"

// эта функция пока без защиты !!!
//типы данных транслятора
#define tLONG    1
#if USE_FLOAT
#define tFLOAT   2
#endif
#define tSTRING  3
#define tSYMBOL  4
#define tLIST    5
#define tTUPLE   6

// промежуточная структура для загрузки
typedef struct SLOADLIST{
    PNODE n;             /// !!! проблемы!
    s_int addr;
    struct SLOADLIST *next;
} LOADLIST;

// загрузка окружения
PNODE load_global_environment(char *tconst, s_int lenc,
                             char *tglobal, s_int leng)
{
    s_int numc, flag;
    s_int lv, l, len, *plen;
#if USE_FLOAT
    double *dv;
#endif
    PNODE p=NIL, pt, pa, st=NIL, pe, t;
    LOADLIST *ll, *pll, *global_llc, *global_lla;

    /*
     загрузка идет сл. обр.
     вначале грузим массивы в соответствующие списки
     аргументы и локальные переменные загружаются в окружение.
     затем строим промежуточные кортежи(таблицы ссылок) по этим
     массивам и по окружению для прямого доступа
    */
    /*
      Идея загрузки без выделения памяти заключается в первоначальном определении количества загружаемого, построения окружения и, затем загрузки элементов.
    */
    // первый эта - подсчет
    l=0;
    numc=0;
    while(l<lenc) {
        switch(tconst[l]) {
            case tLONG:
                // целое
                l=l+5;
            break;
#if USE_FLOAT
            case tFLOAT:
                // плавучее
                l=l+9;
            break;
#endif
            case tSYMBOL:
                // символ
                len=tconst[l+1];
                l=l+1+1+len;
            break;
            case tSTRING:
                // строка
                len=get_long_value((char *)((s_int)tconst+l+1));
                l=l+1+4+len;
            break;
            case tLIST:
                // строка
                len=get_long_value((char *)((s_int)tconst+l+1));
                l=l+1+4+len;
            break;
            case tTUPLE:
                // строка
                len=get_long_value((char *)((s_int)tconst+l+1));
                l=l+1+4+len;
            break;
        };
        numc++;
    };
    // второй этап - создание окружения
    // строим кортеж!!!
    pt=make_n_tuple(numc);
//    temp=make_n_tuple(numc);
    // третий этап - считывание и подстыковка
    l=0;
    numc=0;
    while(l<lenc) {
        switch(tconst[l]) {
            case tLONG:
                // целое
                lv=get_long_value(tconst+l+1);
                p=make_n_long(lv);
                l=l+5;
            break;
#if USE_FLOAT
            case tFLOAT:
                // плавучее
                dv=get_long_value(tconst+l+1);
                p=make_n_float(dv);
                l=l+9;
            break;
#endif
            case tSYMBOL:
                // символ
                len=tconst[l+1];
                // проверим, может быть символ уже есть
                // загрузим глобальный список символов
                // gps=get_global_symbol;
                t=gettuple_n_envexts(get_global_symbol,(char *)((s_int)tconst+l+2));
                if(t!=0) {
                    p=get_n_tuplevalue(t,0);
                } else {
                    p=make_n_symbol((char *)(tconst+l+2));
                    // добавим в глобальное окружение
                    add_n_envext(get_global_symbol,p,NIL);
                };
                l=l+1+1+len;
            break;
            case tSTRING:
                // строка
                len=get_long_value(tconst+l+1);
                p=make_n_stringn((char *)(tconst+l+5),len-1);
                l=l+1+4+len;
            break;
            case tLIST:
                // строка
                len=get_long_value(tconst+l+1);
                p=make_n_lists((char *)(tconst+l+5),len);
                l=l+1+4+len;
            break;
            case tTUPLE:
                // строка
                len=get_long_value(tconst+l+1);
                p=make_n_tuples((char *)(tconst+l+5),len);
                l=l+1+4+len;
            break;
        };
//        print(p);
        set_n_tuplevalue(pt,numc,p);
        numc++;
    };

    // теперь проставим аргументы
    // повторим для окружения.
    l=0;
    numc=0;
    while(l<leng) {
        lv=get_long_value(tglobal+l+1);
        p=get_n_tuplevalue(pt,lv);
        lv=get_long_value(tglobal+l+1+4);
        // прочитали номер константы
        if((lv)==0x00ffffff) {
         //   st=add_n_envext(pe, p, NIL);
        } else {
            // свяжем по номеру
            set_n_envext(get_global_symbol,p,get_n_tuplevalue(pt,lv));
        };
        l=l+1+4+4;
        numc++;
    };
    return(p);
};

//PNODE number1, number2;

// загрузка окружения
PNODE load_environment(char *tconst, s_int lenc,
                       char *targ, s_int lena,
                       char *tloc, s_int lenl,
                       char *tfun, s_int lenf,
                       char *tcode, s_int lenp, char *name_module)
{
    s_int numc, flag;
    s_int lv, l, len, *plen;
#if USE_FLOAT
    double *dv;
#endif
    PNODE p=NIL, pt, pa, pl, st=NIL, pp, pe, t, pf;
    LOADLIST *ll, *pll, *global_llc, *global_lla, *global_lll, *global_llf;

    /*
     загрузка идет сл. обр.
     вначале грузим массивы в соответствующие списки
     аргументы и локальные переменные загружаются в окружение.
     затем строим промежуточные кортежи(таблицы ссылок) по этим
     массивам и по окружению для прямого доступа
     */

    /*
      Идея загрузки без выделения памяти заключается в первоначальном определении количества загружаемого, построения окружения и, затем загрузки элементов.
    */
    // первый эта - подсчет
    l=0;
    numc=0;
    while(l<lenc) {
        switch(tconst[l]) {
            case tLONG:
                // целое
                l=l+5;
            break;
#if USE_FLOAT
            case tFLOAT:
                // плавучее
                l=l+9;
            break;
#endif
            case tSYMBOL:
                // символ
                len=tconst[l+1];
                l=l+1+1+len;
            break;
            case tSTRING:
                // строка
                len=get_long_value(tconst+l+1);
                l=l+1+4+len;
            break;
            case tLIST:
                // строка
                len=get_long_value(tconst+l+1);
                l=l+1+4+len;
            break;
            case tTUPLE:
                // строка
                len=get_long_value(tconst+l+1);
                l=l+1+4+len;
            break;
        };
        numc++;
    };

    // строим кортеж!!!
    pt=make_n_tuple(numc);

    l=0;
    numc=0;
    while(l<lenc) {
        switch(tconst[l]) {
            case tLONG:
                // целое
                lv=get_long_value((char *)((s_int)tconst+l+1));
//                win_printf(">> %x ",lv);
                p=make_n_long(lv);
//                print(p);
//                if(lv==2){
//                 number1=p;
//                };
//                if(lv==4){
//                 number2=p;
//                };
                l=l+5;
            break;
#if USE_FLOAT
            case tFLOAT:
                // плавучее
                dv=get_long_value(tconst+l+1);
                p=make_n_float(dv);
                l=l+9;
            break;
#endif
            case tSYMBOL:
                // символ
                len=tconst[l+1];
                // проверим, может быть символ уже есть
                // загрузим глобальный список символов
                // gps=get_global_symbol;
                t=gettuple_n_envexts(get_global_symbol,(char *)((s_int)tconst+l+2));
                if(t!=0) {
                    p=get_n_tuplevalue(t,0);
                } else {
                    p=make_n_symbol((char *)(tconst+l+2));
                    // добавим в глобальное окружение
                    add_n_envext(get_global_symbol,p,NIL);
                };
                l=l+1+1+len;
            break;
            case tSTRING:
                // строка
                len=get_long_value((char *)((s_int)tconst+l+1));
                p=make_n_stringn((char *)((s_int)tconst+l+5),len-1);
                l=l+1+4+len;
            break;
            case tLIST:
                // строка
                len=get_long_value((char *)((s_int)tconst+l+1));
                p=make_n_lists((char *)((s_int)tconst+l+5),len);
                l=l+1+4+len;
            break;
            case tTUPLE:
                // строка
                len=get_long_value((char *)((s_int)tconst+l+1));
                p=make_n_tuples((char *)((s_int)tconst+l+5),len);
                l=l+1+4+len;
            break;
        };
        set_n_tuplevalue(pt,numc,p);
        numc++;
    };

//    print(pt);

    l=0;
    numc=0;
    while(l<lena) {
        l=l+1+4+4;
        numc++;
    };
    // строим кортеж!!!
    pa=make_n_tuple(numc);
    // построим расширенное окружение
    pe=make_n_envext();
    // теперь проставим аргументы
    l=0;
    numc=0;
    while(l<lena) {
        lv=get_long_value(targ+l+1);
        p=get_n_tuplevalue(pt,lv);
        lv=get_long_value(targ+l+1+4);
        if(lv==0x00ffffff) {
            st=add_n_envext(pe, p, NIL);
        } else {
            st=add_n_envext(pe, p, get_n_tuplevalue(pt,lv));
//            win_printf(">>> ");
//            print(get_n_tuplevalue(pt,lv));
        };
        l=l+1+4+4;
        set_n_tuplevalue(pa,numc,st);
        numc++;
    };
//    win_printf(">>>> ");
//    print(pa);
    // теперь проставим локальные переменные
    l=0;
    numc=0;
    while(l<lenl) {
        l=l+1+4+4;
        numc++;
    };
    pl=make_n_tuple(numc);
    l=0;
    numc=0;
    while(l<lenl) {
        lv=get_long_value(tloc+l+1);
        p=get_n_tuplevalue(pt,lv);
        lv=get_long_value(tloc+l+1+4);
        if(lv==0x00ffffff) {
            st=add_n_envext(pe, p, NIL);
        } else {
            st=add_n_envext(pe, p, get_n_tuplevalue(pt,lv));
        };
        l=l+1+4+4;
        set_n_tuplevalue(pl,numc,st);
        numc++;
    };

    // теперь проставим функции из списка
    l=0;
    numc=0;
    while(l<lenf) {
        l=l+1+4;
        numc++;
    };
    // строим кортеж!!!
    pf=make_n_tuple(numc);
    l=0;
    numc=0;
    flag=0;
    while(l<lenf) {
        lv=get_long_value(tfun+l+1);
        p=get_n_tuplevalue(pt,lv);
        // найти функцию которая возможно уже была определена.
        st=gettuple_n_envext(get_global_func, p);
        if(st==NIL) {
                st=gettuple_n_envext(get_global_extfunc, p);
                if(st==NIL){
                        st=add_n_envext(get_global_func, p, NIL);
                };
        };
        l=l+1+4;
        set_n_tuplevalue(pf,numc,st);
        numc++;
    };

    pp=make_n_stringn(tcode,lenp);
    p=make_n_func();
    set_n_funcconst(p, pt);
    set_n_funcarg(p, pa);
    set_n_funcloc(p, pl);
    set_n_funcfunc(p, pf);
    set_n_funccode(p, pp);
    set_n_funcenv(p, pe);
    pp=make_n_string(name_module);
    set_n_funcmname(p, pp);
    
    return(p);
};

/*
#if EMBEDDED
#else
// функции из файла
void load_function(char *file1)
{
  FILE *f;
  char lab[4], *sfname, *sfcode, *sfconst, *sfloc, *sfarg, *sffun, *sfglb;
  s_int nf, len, i, lenc, lena, lenl, lenf, leng;
  PNODE fn, root, name;

      f=fopen(file1,"rb");
      if(f==NULL) {
             win_printf("Отсутствует файл %s!\n",file1);
             exit_vm(2);
      };
      // считываем метку
      fread(lab,4,1,f);
      // Загружаем глобальные переменные
      // считываем длину секции констант
      fread(&lenc,4,1,f);
      if(lenc>0) {
         sfconst=(char *)malloc(lenc);
         fread(sfconst,lenc,1,f);
         // считываем длину секции аргументов
         fread(&leng,4,1,f);
         sfglb=(char *)malloc(leng);
         fread(sfglb,leng,1,f);
         load_global_environment(sfconst, lenc, sfglb, leng);
      };
      // считываем количество функций
      fread(&nf,4,1,f);
      for(i=0;i<nf;i++) {
        // считываем длину имени
        fread(&len,4,1,f);
        sfname=(char *)malloc(len+1);
        fread(sfname,len,1,f);
        // считываем длину секции констант
        fread(&lenc,4,1,f);
        sfconst=(char *)malloc(lenc+1);
        fread(sfconst,lenc,1,f);
        // считываем длину секции аргументов
        fread(&lena,4,1,f);
        sfarg=(char *)malloc(lena+1);
        fread(sfarg,lena,1,f);
        // считываем длину секции переменных
        fread(&lenl,4,1,f);
        sfloc=(char *)malloc(lenl+1);
        fread(sfloc,lenl,1,f);
        // считываем длину секции функций
        fread(&lenf,4,1,f);
        sffun=(char *)malloc(lenf*2);
        fread(sffun,lenf,1,f);
        // считываем длину секции кода
        fread(&len,4,1,f);
        sfcode=(char *)malloc(len+1);
        fread(sfcode,len,1,f);
        // загрузка функции
        fn=load_environment(sfconst, lenc, sfarg, lena, sfloc, lenl, sffun, lenf, sfcode, len, file1);
        // проверим на наличие ссылки в функциях
        root=gettuple_n_envexts(get_global_func,sfname);
        if(root==0) {
             name=get_n_envexts(get_global_symbol,sfname);
             if((s_int)(name)==0) {
                name=make_global_symbol(sfname);
                add_n_envext(get_global_symbol,name,NIL);
             };
             add_n_envext(get_global_func,name,fn);
        } else {
             set_n_tuplevalue(root,1,fn);
        };

      };
      fclose(f);
};
#endif
*/

// функции из строки
void load_function_string(char *b,  char * file1)
{
  char *f;
  char lab[4], *sfname, *sfcode, *sfconst, *sfloc, *sfarg, *sffun, *sfglb;
  s_int nf, len, i, lenc, lena, lenl, lenf, leng;
  PNODE fn, root, name;

  if (b==NULL) {
    win_printf("Отсутствует буфер !\n");
    return;
  };
  f=b;
  // считываем метку
  lab[0]=f[0];
  lab[1]=f[1];
  lab[2]=f[2];
  lab[3]=f[3];
  
  if ((lab[0]=='F') && (lab[1]=='C')) {

      f=(char *)((s_int)f+4);
      // Загружаем глобальные переменные
      // считываем длину секции констант
      //lenc=(s_int)(*f);
      lenc=get_long_value(f);
      f=(char *)((s_int)f+4);
      if(lenc>0) {
         sfconst=f;
         f=(char *)((s_int)f+lenc);
         // считываем длину секции аргументов
         leng=get_long_value(f);
         f=(char *)((s_int)f+4);
         sfglb=f;
         f=(char *)((s_int)f+leng);
         load_global_environment(sfconst, lenc, sfglb, leng);
      };
      // считываем количество функций
      nf=get_long_value(f);
      f=(char *)((s_int)f+4);
      for(i=0;i<nf;i++) {
        // считываем длину имени
        len=get_long_value(f);
        f=(char *)((s_int)f+4);
        sfname=f;
        f=(char *)((s_int)f+len);
        // считываем длину секции констант
        lenc=get_long_value(f);
        f=(char *)((s_int)f+4);
        sfconst=f;
        f=(char *)((s_int)f+lenc);
        // считываем длину секции аргументов
        lena=get_long_value(f);
        f=(char *)((s_int)f+4);
        sfarg=f;
        f=(char *)((s_int)f+lena);
        // считываем длину секции переменных
        lenl=get_long_value(f);
        f=(char *)((s_int)f+4);
        sfloc=f;
        f=(char *)((s_int)f+lenl);
        // считываем длину секции функций
        lenf=get_long_value(f);
        f=(char *)((s_int)f+4);
        sffun=f;
        f=(char *)((s_int)f+lenf);
        // считываем длину секции кода
        len=get_long_value(f);
        f=(char *)((s_int)f+4);
        sfcode=f;
        f=(char *)((s_int)f+len);
        // загрузка функции
        fn=load_environment(sfconst, lenc, sfarg, lena, sfloc, lenl, sffun, lenf, sfcode, len, file1);
        // проверим на наличие ссылки в функциях
        root=gettuple_n_envexts(get_global_func,sfname);
        if(root==0) {
             name=get_n_envexts(get_global_symbol,sfname);
			 if((s_int)(name)==0) {
				name=make_global_symbol(sfname);
				add_n_envext(get_global_symbol,name,NIL);
			 } else if(name==NIL){
                name=make_global_symbol(sfname);
			 };
             add_n_envext(get_global_func,name,fn);
        } else {
             set_n_tuplevalue(root,1,fn);
        };

      };
  } else {
    win_printf("Неправильный формат файла!\n");
    return;
  };
};
