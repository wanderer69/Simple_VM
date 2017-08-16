#define ARCHITECT16 1
//#define DEBUG_GC

#include <stdlib.h>
#include "MEMMAN.h"
#include "objects.h"
#include "vm.h"
#include "win_printf.h"

#ifdef __BORLANDC__
#include <windows.h>
//#include <dos.h>
#endif

#define cm(p) is_block_mark(p-sizeof(MEM_HEADER_REC))
#define cb(p) is_block_ptr(p-sizeof(MEM_HEADER_REC))
#define cf(p) gc_ismarkf(p-sizeof(MEM_HEADER_REC))

//#define rm(p,1) set_block_mark(p)
//#define rm(p,0) clear_block_mark(p)
#define rn(p,a)

#define set_block_mark_(p) set_block_mark(p-sizeof(MEM_HEADER_REC))
#define clear_block_mark_(p) clear_block_mark(p-sizeof(MEM_HEADER_REC))

#define get_block_value_count_(p) get_block_value_count(p-sizeof(MEM_HEADER_REC))

#define set_block_ptr_(p) set_block_ptr(p-sizeof(MEM_HEADER_REC))
#define clear_block_ptr_(p) clear_block_ptr(p-sizeof(MEM_HEADER_REC))

#define cm_p gc_ismarkm_p
#define cb_p gc_ismarkb_p
#define cf_p gc_ismarkf_p

// ������������ ������ ��� �������� �������
void object_free(PNODE a1)
{
PNODE r,r1,r2;
long l;

r=a1;
r1=NIL;

of_a:
// ����� A
if(is_n_atom(r)) {
   pool_count_down_int(r);
} else
if(is_n_list(r)) {
                // �������� �������
                pool_count_down_int(r);
                // ��������� ���� �� ���������� ���� - ������� ���� ����� 0 ��� ���?
                if(get_block_value_count_((char *)r)==0) {
                  r2=r1;            // ���!!!
                  r1=r;             // �������� ���������� �� �������
                  r=get_n_listcar(r1);        // ����� ������ � ����������� �������
                  clear_block_ptr_(r1);         // �������� ������ ������� ��� ���������
                  set_n_listcar1(r1,r2);    // ����������� (�������� ������ ���������)
                  goto of_a;  // �������� ����� � �������
                };
} else
if(is_n_tuple(r)) {
                if(get_n_tuplelen(r)==0) goto of_b;
                // �������� �������
                pool_count_down_int(r);
                if(get_block_value_count_((char *)r)==0) {
                  l=0;       // ��������� ������� 0
                  r2=r1;
                  r1=r;             // �������� ���������� ������ �� �������
                  r=get_n_tuplevalue(r1,l);   // ����� ������� � ��������� ��� ������� �������
                  set_n_tuplevalue1(r1,l,r2);  // ����������� (�������� ������ ���������)
                  set_n_tuplerest(r1,l); // ��������� ����� ���������� �������� �������
                  goto of_a;  // �������� ����� � �������
                };
} else {

};
// ����� ����� a

// ����� B
of_b:
if(is_n_atom(r1)) {
   goto of_end;
} else
if(is_n_list(r1)) {
   if(r1!=NIL) {  // ��� ����� ������ ?
       if(cb(r1)==0) {
            r2=get_n_listcar(r1);
            set_n_listcar1(r1,r);
            set_block_ptr_(r1);
            r=get_n_listcdr(r1);
            set_n_listcdr1(r1,r2);
            goto of_a;
       };
       r2=get_n_listcdr(r1);
       set_n_listcdr1(r1,r);
       clear_block_ptr_(r1);
       r=r1;
       r1=r2;
       goto of_b;
   };
   goto of_end;
} else
if(is_n_tuple(r1)) {
   // ������ �������
   l=get_n_tuplerest(r1);  // ������� ����������� �����
   if(r1!=NIL) {
of_tuple_b1:
       if(get_n_tuplelen(r1)==0) {
           r2=get_n_tuplevalue(r1,l);
           set_n_tuplevalue1(r1,l,r);
           r=r1;
           r1=r2;
           goto of_b;
       } else {
         if(l<(get_n_tuplelen(r1)-1)) {  // ��� ����� ������� ?
                  r2=get_n_tuplevalue(r1, l);
                  set_n_tuplevalue1(r1,l,r);
                  l++;
                  r=get_n_tuplevalue(r1,l);
                  if(get_block_value_count_((char *)r)==0) {
                    set_n_tuplevalue1(r1,l,r2);
                    set_n_tuplerest(r1,l);
                    goto of_a;
                  } else {
                    goto of_tuple_b1;
                  };
         } else {
             r2=get_n_tuplevalue(r1,l);
             set_n_tuplevalue1(r1,l,r);
             r=r1;
             r1=r2;
             goto of_b;
         };
       };
   };
};
of_end:
   return;  // ����� ��������
};

// �������� ������ ��� �������� �������
void mark(PNODE a1)
{
PNODE r,r1,r2;
int l, i;
int m_atom, m_list, m_tuple,m_element;

r=a1;
r1=NIL;

m_atom=0;
m_list=0;
m_tuple=0;
m_element=0;

// ����� A
mark_a:
if(is_n_atom(r)) {
   if(cm(r)==0) {  // ������� ����?
        set_block_mark_(r);   // ��������
        rn(r,1);   // �������� ���������
        m_atom++;
   };
} else
if(is_n_list(r)) {
   if(cm(r)==0) {      // �������� �������?
                set_block_mark_(r);   // ��������
                rn(r,1);   // �������� ���������
                m_list++;
                // �������� �������
                // ��������� ���� �� ���������� ���� - ������� ���� ����� 0 ��� ���?
                r2=r1;            // ���!!!
                r1=r;             // �������� ���������� �� �������
                r=get_n_listcar(r1);        // ����� ������ � ����������� �������
                clear_block_ptr_(r1);         // �������� ������ ������� ��� ���������
                set_n_listcar1(r1,r2);    // ����������� (�������� ������ ���������)
                goto mark_a;  // �������� ����� � �������
   };
} else  {
  if(is_n_tuple(r)) {
     if(is_block_mark(r-sizeof(MEM_HEADER_REC))==0) {          // ������� ������?
                  set_block_mark_(r);   // ��������
                  rn(r,1);   // �������� ���������
                  m_tuple++;
                  if(get_n_tuplelen(r)==0) goto mark_b;
                  // �������� �������
                  // ��������� ���� �� ���������� ���� - ������� ���� ����� 0 ��� ���?
                  l=0;       // ��������� ������� 0
                  r2=r1;
                  r1=r;             // �������� ���������� ������ �� �������
                  r=get_n_tuplevalue(r1,l);   // ����� ������� � ��������� ��� ������� �������
                  set_n_tuplevalue1(r1,l,r2);  // ����������� (�������� ������ ���������)
                  set_n_tuplerest(r1,l); // ��������� ����� ���������� �������� �������
                  goto mark_a;  // �������� ����� � �������
     };
     // ����� ����� a
  } else {
//    win_printf("-> %d\r\n",get_n_type(r));
    set_block_mark_(r);   // ��������
    m_element++;
    // �������� �����
    for(i=0;i<get_n_stackptr(r);i++){
       mark(get_n_stacksvalue(r,i));
    };
  };
};
// ����� B
mark_b:
if(is_n_atom(r1)) {
   goto mark_end;
} else
if(is_n_list(r1)) {
   if(r1!=NIL) {  // ��� ����� ������ ?
       if(cb(r1)==0) {
            r2=get_n_listcar(r1);
            set_n_listcar1(r1,r);
            set_block_ptr_(r1);
            r=get_n_listcdr(r1);
            set_n_listcdr1(r1,r2);
            goto mark_a;
       };
       r2=get_n_listcdr(r1);
       set_n_listcdr1(r1,r);
       clear_block_ptr_(r1);
       r=r1;
       r1=r2;
       goto mark_b;
   };
   goto mark_end;
} else
if(is_n_tuple(r1)) {
   // ������ �������
   l=get_n_tuplerest(r1);  // ������� ����������� �����
   if(r1!=NIL) {
marktuple_b1:
       if(get_n_tuplelen(r1)==0) {
           r2=get_n_tuplevalue(r1,l);
           set_n_tuplevalue1(r1,l,r);
           r=r1;
           r1=r2;
           goto mark_b;
       } else {
         if(l<(get_n_tuplelen(r1)-1)) {  // ��� ����� ������� ?
                  r2=get_n_tuplevalue(r1, l);
                  set_n_tuplevalue1(r1,l,r);
                  l++;
                  r=get_n_tuplevalue(r1,l);
                  set_n_tuplevalue1(r1,l,r2);
                  set_n_tuplerest(r1,l);
                  goto mark_a;
         } else {
             r2=get_n_tuplevalue(r1,l);
             set_n_tuplevalue1(r1,l,r);
             r=r1;
             r1=r2;
             goto mark_b;
         };
       };
   };
};
mark_end:
#ifdef DEBUG_GC
win_printf("Mark m_atom %d m_list %d m_tuple %d m_element %d\r\n",m_atom, m_list, m_tuple,m_element);
#endif
   return;  // ����� ��������
};

// ���������� �������
void unmark(PNODE a1)
{
PNODE r,r1,r2;
long l, i;
int m_atom, m_list, m_tuple,m_element;

r=a1;
r1=NIL;

m_atom=0;
m_list=0;
m_tuple=0;
m_element=0;

unmark_a:
// ����� A
if(is_n_atom(r)) {
   if(cm(r)==1) {  // ������� ����?
        clear_block_mark_(r);   // ��������
        rn(r,0);   // �������� ���������
        m_atom++;
   };
} else
if(is_n_list(r)) {
   if(cm(r)==1) {      // �������� �������?
                clear_block_mark_(r);   // ��������
                rn(r,0);   // �������� ���������
                m_tuple++;
                // ��������� ���� �� ���������� ���� - ������� ���� ����� 0 ��� ���?
                r2=r1;            // ���!!!
                r1=r;             // �������� ���������� �� �������
                r=get_n_listcar(r1);        // ����� ������ � ����������� �������
                clear_block_ptr_(r1);         // �������� ������ ������� ��� ���������
                set_n_listcar1(r1,r2);    // ����������� (�������� ������ ���������)
                goto unmark_a;  // �������� ����� � �������
   };
} else {
  if(is_n_tuple(r)) {
     if(cm(r)==1) {          // ������� ������?
                  clear_block_mark_(r);   // ��������
                  rn(r,0);   // �������� ���������
                  if(get_n_tuplelen(r)==0) goto unmark_b;
                    l=0;       // ��������� ������� 0
                    r2=r1;
                    r1=r;             // �������� ���������� ������ �� �������
                    r=get_n_tuplevalue(r1,l);   // ����� ������� � ��������� ��� ������� �������
                    set_n_tuplevalue1(r1,l,r2);  // ����������� (�������� ������ ���������)
                    set_n_tuplerest(r1,l); // ��������� ����� ���������� �������� �������
                    goto unmark_a;  // �������� ����� � �������
     };
  } else {
    set_block_mark_(r);   // ��������
    m_element++;
    // �������� �����
    for(i=0;i<get_n_stackptr(r);i++){
       unmark(get_n_stackvalue(r,i));
    };
  };
};
// ����� ����� a

// ����� B
unmark_b:
if(is_n_atom(r1)) {
   goto unmark_end;
} else
if(is_n_list(r1)) {
   if(r1!=NIL) {  // ��� ����� ������ ?
       if(cb(r1)==0) {
            r2=get_n_listcar(r1);
            set_n_listcar1(r1,r);
            set_block_ptr_(r1);
            r=get_n_listcdr(r1);
            set_n_listcdr1(r1,r2);
            goto unmark_a;
       };
       r2=get_n_listcdr(r1);
       set_n_listcdr1(r1,r);
       clear_block_ptr_(r1);
       r=r1;
       r1=r2;
       goto unmark_b;
   };
   goto unmark_end;
} else
if(is_n_tuple(r1)) {
   // ������ �������
   l=get_n_tuplerest(r1);  // ������� ����������� �����
   if(r1!=NIL) {
       if(get_n_tuplelen(r1)==0) {
           r2=get_n_tuplevalue(r1,l);
           set_n_tuplevalue1(r1,l,r);   // 1
           r=r1;
           r1=r2;
           goto unmark_b;
       } else {
         if(l<(get_n_tuplelen(r1)-1)) {  // ��� ����� ������� ?
                  r2=get_n_tuplevalue(r1, l);
                  set_n_tuplevalue1(r1,l,r);
                  l++;
                  r=get_n_tuplevalue(r1,l);
                  set_n_tuplevalue1(r1,l,r2);
                  set_n_tuplerest(r1,l);
                  goto unmark_a;
         } else {
             r2=get_n_tuplevalue(r1,l);
             set_n_tuplevalue1(r1,l,r);
             r=r1;
             r1=r2;
             goto unmark_b;
         };
       };
   };
};
unmark_end:
#ifdef DEBUG_GC
win_printf("Unmark m_atom %d m_list %d m_tuple %d m_element %d\r\n",m_atom, m_list, m_tuple,m_element);
#endif
   return;  // ����� ��������
};

void gc_mark(PNODE a1)
{
  if(is_n_tuple(a1)){
     pool_count_up_int(a1);
     object_free(a1);
  } else if(is_n_list(a1)){
     pool_count_up_int(a1);
     object_free(a1);
  };
};

void gc_all(void)
{
#ifdef __BORLANDC__
  unsigned int WorkingTime, WorkingTimeA;
     // �������� �����
     WorkingTime = GetTickCount();
#endif
     mark(ROOT);
     collect_free_block();
     unmark(ROOT);
#ifdef __BORLANDC__
     WorkingTimeA = GetTickCount();
     global_timing[256].score=global_timing[256].score+1;
     global_timing[256].time=global_timing[256].time+(WorkingTimeA-WorkingTime)+1;
#endif
};
