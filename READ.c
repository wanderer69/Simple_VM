#define ARCHITECT16 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MEMMAN.h"
#include "objects.h"
#include "print.h"
//������� ����� ������ - ���������� ������ � ������ ��� ������

s_int global_read_ptr;
PNODE global_read_ret;
s_int beg_str, len_str;
int parser_flag;

//����� �������
#define LEVEL 1
#define QUOTED 2
#define ADDED 4
#define D_QUOTED 8
#define POINTED  16
#define TEXT 32
#define COMMENT 64
#define ALL_FLAGS 0

int read_char(char *str)
{
    if(str[global_read_ptr]!=0) {
           global_read_ptr++;
//           printf("char> %c\n",str[global_read_ptr-1]);
           return(str[global_read_ptr-1]);
    };
    return(0);
};

void prev_char(void)
{
    global_read_ptr--;
};

#define unread_char prev_char

//����� �������
//���������� ���� ������ ���������� ����������� ��� �������� �����
// LEVEL - ��������� ����
// QUOTE - ������� ��������� quote
// D_QUOTED - ������� ����� ������
// POINT - ������� ����� �������� ����
// F_POINT - ������� ����� ��������� �����
// COMMENT - ������� ����� ������������
//��������� ���� ������ ��������������� �����
//�� ������ ��� ����������� �����

//������ � ������ �������
void set_parser_flag(unsigned int p)
{
    parser_flag=parser_flag + p;
};

int is_parser_flag(unsigned int p)
{
    if((parser_flag & p)!=0) return 1;
    return 0;
};

void clear_parser_flag(unsigned int p)
{
    if(p==ALL_FLAGS) parser_flag=0;
    else {
        if(is_parser_flag(p)) parser_flag=parser_flag - p;
    };
};

/*
void print_parser_flag()
{
    if(is_parser_flag(LEVEL)) win_printf("  LEVEL  ");
    if(is_parser_flag(ADDED)) win_printf("  ADDED  ");
    if(is_parser_flag(D_QUOTED)) win_printf("  D_QUOTED  ");
    if(is_parser_flag(POINTED)) win_printf("  POINTED  ");
    if(is_parser_flag(TEXT)) win_printf("  TEXT  ");
    win_printf("\n");
};*/

//��������� ������������� ��������
int is_number(int ch)
{
    if((ch>=48) && (ch<=57)) return 1;
    return 0;
};

int is_symb(int ch)
{
    if((!is_number(ch)) && (ch!=46)) return 1;
    return 0;
};

//������� �������������� �� ������ � ���� ���������������� ����
PNODE par2sym(char *buff, s_int beg, s_int len)
{
char buffn[32];
int nump, pointp, sign, charp; //�������� ������� (�������)
int convert;  //���� ���� ��������������
s_int i, l;
PNODE ret;

    // � ������ �������� �� ���� �������
    nump=0;
    pointp=0;
    sign=0;
    charp=0;

    for(i=0;i<len;i++) {
// printf("-} %c\n",buff[i+beg-1]);
            if(is_number(buff[i+beg-1])) {
                                //���� ����� �������� ��������������� �������
                                nump++;
            };
#if USE_FLOAT
            if(buff[i+beg-1]=='.') {
                                //���� ���������� �����
                                // �������� ��������������� �������
                                pointp++;
            };
#endif
            if((buff[i+beg-1]=='-') && (i==0)){
                                //���� ���� � ����� ������
                                // �������� ��������������� �������
                                sign++;
            };
            if(is_symb(buff[i+beg-1])) {
                                //���� ������ ������ ��������
                                //��������������� �������
                                charp++;
            };
    };
    convert=0;
    //������ �������������� ��� � ��� ����������
    if(charp==0) {
        //������ ��� ��� �����!!!
        if(pointp>1) {
                //�������!! �� ����� ���� � ����� ���� ���������� �����
                //������������ ��� ������
                convert=1;
        } else {
            if(sign>1) {
                        convert=1; //� ����� ����� ���� ������ ���� ����!!!
            };
            if(pointp==0) {
                        //��� ����� �����!!!
                        convert=2;
            } else {
                //��� "��������" �����!!
                        convert=3;
            }
        };
    } else {
        //���� �� ����� ��������� ����� ������� ����� �������.
        //�.�. �� ����� ������� ����� � ����������������� ��� ��������
        //��������.
        convert=1;
    };

    //������� ��������������
    switch(convert) {
        case 1: {
//                buffn=(char *)malloc(len+2);
                strncpy(buffn,(char *)(buff+beg-1),len);
                buffn[len]=0;
                ret=make_global_symbol(buffn);
//                free(buffn);
                break;
        }
        case 2: {
//                buffn=(char *)malloc(len+2);
                strncpy(buffn,(char *)(buff+beg-1),len);
                buffn[len]=0;
#ifdef __LINUX
                //l=strtol(get_n_stringvalue(buffn),NULL,0);
                ret=make_n_long(strtol(buffn,NULL,10));
#else
                ret=make_n_long(atoi_s(buffn));
#endif
//                free(buffn);
                break;
        }
        case 3: {
#if USE_FLOAT
                buffn=(char *)malloc(len+2);
                strncpy(buffn,(buff+beg-1),len);
                buffn[len]=0;
                ret=make_n_float(atof(buffn));
                free(buffn);
#endif
                break;
        }
        default:
                ret=NIL;
    };
return ret;
};

//���������� ������!!
PNODE par2str(char *buff, s_int beg, s_int len)
{
char *buffn;
PNODE ret;

    ret=make_n_stringn((char *)(buff+beg-1),len);
/*
    buffn=(char *)malloc(len+1);
    strncpy(buffn,(buff+beg-1),len);
    buffn[len+1]=0;
    ret=make_n_string(buffn);
    free(buffn);
*/
    return(ret);
};

//���������� ������� �� ��������� �������, ������� ......
//��� ���� ������� ���������, ������ � ������ ������������, �� ���
//������ quote
void _end_str(char *str)
{
    //��������� ������
//    printf("> %s %ld\n",(char *)(str+beg_str+1),len_str-1);
    global_read_ret=par2str(str,beg_str+1,len_str-1);
    clear_parser_flag(D_QUOTED);
};

void _end_atom(char *str)
{
    //���� �� �������� ���� �� ����������� ���
    global_read_ret=par2sym(str,beg_str,len_str);
    clear_parser_flag(TEXT);
};

//������� �����
char _read_a(char *str)
{
int end_key, ret=0;
char curr_char;

 parser_flag=0; //������� ����� �������
 end_key=0;
 //����, ������ ���� �����
    while(end_key==0) {
        //������ ������ �� ������ �����
        curr_char=read_char(str);
        //��������� ������
        switch(curr_char) {
            case '(': {
                //�������� ����������� ������
                //���� ������� ����� ����� ����� ������
                //�������������� ��� ������� �� ����� ��������� �������
                if(!is_parser_flag(D_QUOTED)) {
                     if(is_parser_flag(TEXT)) {
                        len_str++;
                        _end_atom(str);
                        unread_char();
                        end_key=1;
                        ret=0;
                     } else {
                        end_key=1;
                        ret=curr_char;
                     };
                } else {
                        //����������� ������� ����� ������
                        len_str++;
                };
                break;
            }
            case ')': {
                if(!is_parser_flag(D_QUOTED)){
                     if(is_parser_flag(TEXT)) {
                        len_str++;
                        _end_atom(str);
                        unread_char();
                        end_key=1;
                        ret=0;
                     }  else {
                       //������-�� �� �������� ��� ������ �����!!!
                         if(is_parser_flag(QUOTED)) {
                            end_key=1;
                            ret=curr_char;
                         } else {
                            end_key=1;
                            ret=curr_char;
                         };
                     };
                } else {
                    //����������� ������� ����� ������
                    len_str++;
                };
                break;
            }
            case ' ': {
                if(!is_parser_flag(D_QUOTED)) {
                        if(is_parser_flag(TEXT)) {
                            len_str++;
                            _end_atom(str);
                            unread_char();
                            end_key=1;
                            ret=0;
                        } else {
                         //������ �� ������
                        };
                }  else {
                         //����������� ������� ����� ������
                         len_str++;
                };
                break;
            }
            case 0x0d: {
                //����� �������!!!
                if(!is_parser_flag(D_QUOTED)) {
                } else {
                         //����������� ������� ����� ������
                         len_str++;
                };
                break;
            }
            case 0x0a: {
                //����� �������!!!
                if(!is_parser_flag(D_QUOTED)) {
                } else {
                       //����������� ������� ����� ������
                       len_str++;
                };
                break;
            }

            case '"':
            case  39: {
                if(is_parser_flag(D_QUOTED)) {
                         _end_str(str);

                         end_key=1;
                         ret=0;
                } else {
                         set_parser_flag(D_QUOTED);
                         beg_str=global_read_ptr;
                         len_str=0;
                };
                break;
            }

            case '.': {
                 //� ������ ������� �����
                 if(!is_parser_flag(D_QUOTED)) {
                        if(is_parser_flag(TEXT)) {
                             //����� ������� ��� ��� �������� �����
                            //����������� ������� ����� ������
                            len_str++;
                        }  else {
                            end_key=1;
                            ret=curr_char;
                        };
                 } else {
                        //����������� ������� ����� ������
                         len_str++;
                 };
                 break;
            }
            case 0 : {
                 //����� ������!!!
                 //��������� ������-�� ������� ��������������
                 if(!is_parser_flag(D_QUOTED)) {
                     if(is_parser_flag(TEXT)) {
                        len_str++;
                        _end_atom(str);
                        end_key=1;
                        ret=0;
                     } else {
                        //����� �� ���� ���� ��������� ������� ������
                        //����� ����� ������� �� �����
                        end_key=1;
                        //������!!!!
                        ret=1;
                     };
                 } else {
                    if(is_parser_flag(D_QUOTED)) {
                        len_str++;
                        _end_str(str);
                        end_key=1;
                        ret=0;
                    } else {
                        //����� �� ���� ���� ��������� ������� ������
                        //����� ����� ������� �� �����
                        end_key=1;
                        //������!!!!
                        ret=1;
                    };
                 };
                 break;
            }
            default: {
                    //����� �� ������������ ������� �������
                    if(!is_parser_flag(D_QUOTED)) {
                        if(!is_parser_flag(TEXT)) {
                                    set_parser_flag(TEXT);
                                    beg_str=global_read_ptr;
                                    len_str=0;
                        } else  {
                            //����������� ������� ����� ������
                            len_str++;
                        };
                    } else  {
                         //����������� ������� ����� ������
                         len_str++;
                    };
                    break;
            }
        };
       //����� �����
    };
return ret;
};

//������� �����
char _read_b(char *str)
{
int end_key, ret=0;
char curr_char;

 parser_flag=0; //������� ����� �������
 end_key=0;
 //����, ������ ���� �����
    while(end_key==0) {
        //������ ������ �� ������ �����
        curr_char=read_char(str);
// printf(">%c\n",curr_char);
        //��������� ������
        switch(curr_char) {
            case '[': {
                //�������� ����������� ������
                //���� ������� ����� ����� ����� ������
                //�������������� ��� ������� �� ����� ��������� �������
                if(!is_parser_flag(D_QUOTED)) {
                     if(is_parser_flag(TEXT)) {
                        len_str++;
                        _end_atom(str);
                        unread_char();
                        end_key=1;
                        ret=0;
                     } else {
                        end_key=1;
                        ret=curr_char;
                     };
                } else {
                        //����������� ������� ����� ������
                        len_str++;
                };
                break;
            }
            case ']': {
                if(!is_parser_flag(D_QUOTED)){
                     if(is_parser_flag(TEXT)) {
                        len_str++;
                        _end_atom(str);
//                        print(global_read_ret);
                        unread_char();
                        end_key=1;
                        ret=0;
                     }  else {
                        //������-�� �� �������� ��� ������ �����!!!
                        end_key=1;
                        ret=curr_char;
                     };
                } else {
                    //����������� ������� ����� ������
                    len_str++;
                };
                break;
            }
            case ' ': {
                if(!is_parser_flag(D_QUOTED)) {
                        if(is_parser_flag(TEXT)) {
                            len_str++;
                            _end_atom(str);
                            unread_char();
                            end_key=1;
                            ret=0;
                        } else {
                         //������ �� ������
                        };
                }  else {
                         //����������� ������� ����� ������
                         len_str++;
                };
                break;
            }
            case ',': {
                if(!is_parser_flag(D_QUOTED)) {
                        if(is_parser_flag(TEXT)) {
                            len_str++;
                            _end_atom(str);
                            unread_char();
                            end_key=1;
                            ret=0;
                        } else {
                            end_key=1;
                            ret=curr_char;
                        };
                }  else {
                         //����������� ������� ����� ������
                         len_str++;
                };
                break;
            }
            case 0x0d: {
                //����� �������!!!
                if(!is_parser_flag(D_QUOTED)) {
                } else {
                         //����������� ������� ����� ������
                         len_str++;
                };
                break;
            }
            case 0x0a: {
                //����� �������!!!
                if(!is_parser_flag(D_QUOTED)) {
                } else {
                       //����������� ������� ����� ������
                       len_str++;
                };
                break;
            }

            case 39:
            case '"': {
                if(is_parser_flag(D_QUOTED)) {
                         _end_str(str);
                         end_key=1;
                         ret=0;
                } else {
                         set_parser_flag(D_QUOTED);
                         beg_str=global_read_ptr;
                         len_str=0;
                };
                break;
            }

            case '.': {
                 //� ������ ������� �����
                 if(!is_parser_flag(D_QUOTED)) {
                        if(is_parser_flag(TEXT)) {
                             //����� ������� ��� ��� �������� �����
                            //����������� ������� ����� ������
                            len_str++;
                        }  else {

                        };
                 } else {
                        //����������� ������� ����� ������
                         len_str++;
                 };
                 break;
            }
            case 0 : {
                 //����� ������!!!
                 //��������� ������-�� ������� ��������������
                 if(!is_parser_flag(D_QUOTED)) {
                     if(is_parser_flag(TEXT)) {
                        len_str++;
                        _end_atom(str);
                        end_key=1;
                        ret=0;
                     } else {
                        //����� �� ���� ���� ��������� ������� ������
                        //����� ����� ������� �� �����
                        end_key=1;
                        //������!!!!
                        ret=1;
                     };
                 } else {
                    if(is_parser_flag(D_QUOTED)) {
                        len_str++;
                        _end_str(str);
                        end_key=1;
                        ret=0;
                    } else {
                        //����� �� ���� ���� ��������� ������� ������
                        //����� ����� ������� �� �����
                        end_key=1;
                        //������!!!!
                        ret=1;
                    };
                 };
                 break;
            }
            default: {
                    //����� �� ������������ ������� �������
                    if(!is_parser_flag(D_QUOTED)) {
                        if(!is_parser_flag(TEXT)) {
                                    set_parser_flag(TEXT);
                                    beg_str=global_read_ptr;
                                    len_str=0;
                        } else  {
                            //����������� ������� ����� ������
                            len_str++;
                        };
                    } else  {
                         //����������� ������� ����� ������
                         len_str++;
                    };
                    break;
            }
        };
       //����� �����
    };
return ret;
};

void _read_0(char *str);
void _read_1(char *str);

PNODE _readl(char *str)
{
int r1;
PNODE ret;
    global_read_ret=NIL;

    beg_str=0;
    len_str=0;
    global_read_ptr=0;

    if ((r1=_read_a(str))=='(') {
//         global_read_ret=make_n_list(nil,nil);
//         ret=global_read_ret;
         _read_0(str);
//         ret=get_n_listcdr(ret);
         ret=global_read_ret;
    } else {
         if((r1==')') && (r1=='.')) {
              global_read_ret=NIL;
         } else {

         };
         ret=global_read_ret;
    };
    return(ret);
};

PNODE _readt(char *str)
{
int r1;
PNODE ret;
    global_read_ret=NIL;

    beg_str=0;
    len_str=0;
    global_read_ptr=0;

    if ((r1=_read_b(str))=='[') {
         _read_1(str);
         ret=global_read_ret;
    } else {
         if((r1==']') && (r1==',')) {
              global_read_ret=NIL;
         } else {

         };
         ret=global_read_ret;
    };
    return(ret);
};

void _read_0(char *str)
{
 int r2;
 PNODE ptr, ret;

 ret=make_n_list(NIL,NIL);
 ptr=ret;

 label_a:
       r2=_read_a(str);
       if(r2==1){
            //������  - ���������� ������
            global_read_ret=NIL;
            goto label_err;
       } else {
         if(r2=='(') {
                  _read_0(str);
         } else {
            if(r2==')') {
                goto label_e;
            } else {
               if(r2=='.') {
    //             printf("����� �����!\n");
                  goto label_b;
               } else {
               };
            };
         };
         set_n_listcdr(ptr,make_n_list(global_read_ret,NIL));
         ptr=get_n_listcdr(ptr);
         goto label_a;
       };
 label_b:
       r2=_read_a(str);

       if(r2==1) {
          //������  - ���������� ������
          global_read_ret=NIL;
          goto label_err;
       } else {
             if(r2=='(') {
                      global_read_ret=make_n_list(NIL,NIL);
                      _read_0(str);
             } else {
                if((r2==')') || (r2=='.')) {
                    global_read_ret=NIL;
                    goto label_err;
                };
             };
             set_n_listcdr(ptr,global_read_ret);
             ptr=get_n_listcdr(ptr);
             r2=_read_a(str);
             if(r2!=')') {
                        global_read_ret=NIL;
                        goto label_err;
             };
       };
 label_e:
       global_read_ret=get_n_listcdr(ret);
       return;
 label_err:
       win_printf("������ ��� ������� ������!\n");
       global_read_ret=NIL;
       return;
};

// ������ ���������� ��������� � �������
int _number_of_tuple(char *str)
{
long back;
char curr_char;
int scorer, end_key, parity;

 back=global_read_ptr; // �������� ���������
 parser_flag=0; //������� ����� �������
 end_key=0;
 parity=0;
 scorer=0;
 //����, ������ ���� �����
 while(end_key==0) {
        //������ ������ �� ������ �����
        curr_char=read_char(str);
        //��������� ������
        switch(curr_char) {
            case '[': {
                //�������� ����������� ������
                if(!is_parser_flag(D_QUOTED)) {
                        parity++;
                };
                break;
            }
            case ']': {
                if(!is_parser_flag(D_QUOTED)){
                      if(parity>0) {
                        parity--;
                      } else {
                        end_key=1;
                        scorer++;
                      };
                };
                break;
            }
            case ',': {
                if(!is_parser_flag(D_QUOTED)) {
                      if(parity==0) scorer++;
                };
                break;
            }
            case '"': {
                if(is_parser_flag(D_QUOTED)) {
                         clear_parser_flag(D_QUOTED);
                } else {
                         set_parser_flag(D_QUOTED);
                };
                break;
            }
            case 0 : {
                        end_key=1;
                        //������!!!!
                        scorer=-1;
                 break;
            }
            default: {
                    //����� �� ������������ ������� �������
                    break;
            }
        };
       //����� �����
    };

global_read_ptr=back; // �������� ���������
return(scorer);
};

void _read_1(char *str)
{
 int r2, num, len;

 PNODE ptr, ret;

 len=_number_of_tuple(str);
 if(len<0) goto label_err;
// printf("> %ld\n",len);
 ret=make_n_tuple(len);
 fill_n_tuple(ret, NIL);
 ptr=ret;
 num=0;

 label_a:
       r2=_read_b(str);
       if(r2==1){
            //������  - ���������� ������
            global_read_ret=NIL;
            goto label_err;
       } else {
         if(r2=='[') {
             _read_1(str);
         } else {
            if(r2==']') {
                goto label_e;
            } else {
               if(r2==',') {
                  num++;
                  goto label_a;
               } else {
                   if (r2==' ') {
                      goto label_a;
                   };
               };
            };
         };
//         print(global_read_ret);
         set_n_tuplevalue(ptr,num,global_read_ret);
         goto label_a;
       };
 label_e:
       global_read_ret=ret;
       return;
 label_err:
       win_printf("������ ��� ������� �������!\n");
       global_read_ret=NIL;
       return;
};
