//#include "lib/i2c.h"
//---------------------------------------------------------------------------
#define ARCHITECT16 1

//#define _DEBUG_ 1

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "MEMMAN.h"
#include "objects.h"
#include "vm.h"

#include "print.h"
#include "extfunc.h"

#ifdef __BORLANDC__
#include <dos.h>
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma argsused
#endif

// новая кодовая машина LISP
// смысл заключается в трансляции слов в код.
// фактически все функции имеют коды

#define EXTFUNC(p)  int p()


// функции включения часов
EXTFUNC(start_rtc)
{
#ifndef __BORLANDC__
#ifndef __LINUX
    rtc_start();
#else
#endif
#endif
    return(0);
};

EXTFUNC(set_time)
{
    PNODE sec, min, hr, day, date, mn, year;
    char  sec_, min_, hr_, day_, date_, mn_, year_;

    sec=get_n_stacksvalue(get_current_stackd,0);
    min=get_n_stacksvalue(get_current_stackd,0);
    hr=get_n_stacksvalue(get_current_stackd,0);
    day=get_n_stacksvalue(get_current_stackd,0);
    date=get_n_stacksvalue(get_current_stackd,0);
    mn=get_n_stacksvalue(get_current_stackd,0);
    year=get_n_stacksvalue(get_current_stackd,0);

    sec_=(char)get_n_longvalue(sec);
    min_=(char)get_n_longvalue(min);
    hr_=(char)get_n_longvalue(hr);
    day_=(char)get_n_longvalue(day);
    date_=(char)get_n_longvalue(date);
    mn_=(char)get_n_longvalue(mn);
    year_=(char)get_n_longvalue(year);

#ifndef __BORLANDC__
#ifndef __LINUX
    save_time_date(sec_, min_, hr_, day_, date_, mn_, year_);
#else

#endif
#endif

    return(0);
};

EXTFUNC(get_time)
{
    PNODE sec, min, hr, day, date, mn, year, lst;
    char  sec_, min_, hr_, day_, date_, mn_, year_;

#ifndef __BORLANDC__
#ifndef __LINUX
    load_time_date(&sec_, &min_, &hr_, &day_, &date_, &mn_, &year_);
#endif
#else
#ifdef __BORLANDC__
    struct date d;
    struct time t;

    getdate(&d);
    printf("The current year is: %d\n", d.da_year);
    printf("The current day is: %d\n", d.da_day);
    printf("The current month is: %d\n", d.da_mon);
    date_=d.da_day;
    mn_=d.da_mon;
    year_=d.da_year;

    gettime(&t);
    printf("The current time is: %2d:%02d:%02d.%02d\n",t.ti_hour, t.ti_min, t.ti_sec, t.ti_hund);
    sec_=t.ti_sec;
    min_=t.ti_min;
    hr_=t.ti_hour;
#endif
#endif

    sec=make_n_long(sec_);
    pool_count_up(sec);
    add_critreg(sec,0);
    min=make_n_long(min_);
    pool_count_up(min);
    add_critreg(min,1);
    hr=make_n_long(hr_);
    pool_count_up(hr);
    add_critreg(hr,2);
    day=make_n_long(day_);
    pool_count_up(day);
    add_critreg(day,3);
    date=make_n_long(date_);
    pool_count_up(date);
    add_critreg(date,4);
    mn=make_n_long(mn_);
    pool_count_up(mn);
    add_critreg(mn,5);
    year=make_n_long(year_);
    pool_count_up(year);
    add_critreg(year,6);

    lst=make_n_tuple(7);
    fill_n_tuple(lst, NIL);

    set_n_tuplevalue(lst,0,sec);
    pool_count_down(sec);
    set_n_tuplevalue(lst,1,min);
    pool_count_down(min);
    set_n_tuplevalue(lst,2,hr);
    pool_count_down(hr);
    set_n_tuplevalue(lst,3,day);
    pool_count_down(day);
    set_n_tuplevalue(lst,4,date);
    pool_count_down(date);
    set_n_tuplevalue(lst,5,mn);
    pool_count_down(mn);
    set_n_tuplevalue(lst,6,year);
    pool_count_down(year);

    push_n_stack(get_current_stackd,lst);
    free_all_critreg();
    return(1);
};

void load_extfunction_int(void)
{
// printf("функции");
// функции
    set_n_extfunc("start_rtc",make_n_extfunc(start_rtc));
    set_n_extfunc("set_time",make_n_extfunc(set_time));
    set_n_extfunc("get_time",make_n_extfunc(get_time));
}; 
