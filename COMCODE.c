// #include <conio.h>
// #include <stdio.h>
 #include <stdlib.h>

 #define VM_CODE 1

#ifdef VM_CODE
 #include "vm.h"
 #include "COMMAND.H"
#endif

 #include "COMCODE.H"

// типы 1-переменная 2-целое из списка аргументов 3-метка 4-целое из списка локальных переменных 5- целое из списка функций

AWORD aword[]= {
   {"NOP"    ,0,0,  0,0 ,0
#ifdef VM_CODE
   ,cmd_nop
#endif
   },
   {"PUSHC"  ,1,1, 11,4 ,0 
#ifdef VM_CODE
   ,cmd_pushc
#endif
   },
   {"DROP"   ,0,0, 12,0 ,1
#ifdef VM_CODE
   ,cmd_drop
#endif
   },
   {"DUP"    ,0,0,122,0 ,1
#ifdef VM_CODE
   ,cmd_dup
#endif
   },
   {"SAVEV"  ,0,0, 13,0 ,2
#ifdef VM_CODE
   ,cmd_savev
#endif
   },
   {"SETDVS" ,0,0, 14,0 ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELDVS" ,0,0, 15,0 ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"LOADV"  ,0,0, 16,0 ,1
#ifdef VM_CODE
   ,cmd_loadv
#endif
   },
   {"SETDVL" ,0,0, 17,0 ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELDVL" ,0,0, 18,0 ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"CPA"    ,1,6, 19,4 ,0
#ifdef VM_CODE
   ,cmd_cpa
#endif
   },
   {"SAVEVL" ,1,4, 20,4 ,1
#ifdef VM_CODE
   ,cmd_savevl
#endif
   },
   {"LOADVL" ,1,4, 21,4 ,0
#ifdef VM_CODE
   ,cmd_loadvl
#endif
   },
   {"SAVEAL" ,1,2, 22,4 ,1
#ifdef VM_CODE
   ,cmd_saveal
#endif
   },
   {"LOADAL" ,1,2, 23,4 ,0
#ifdef VM_CODE
   ,cmd_loadal
#endif
   },
   {"SAVEA"  ,0,0, 24,0 ,2
#ifdef VM_CODE
   ,NULL //cmd_savea
#endif
   },
   {"SETDAS" ,0,0, 25,0 ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELDAS" ,0,0, 26,0 ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"LOADA"  ,0,0, 27,0 ,1
#ifdef VM_CODE
   ,NULL    //cmd_loada
#endif
   },
   {"SETDAL" ,0,0, 28,0  ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELDAL" ,0,0, 29,0  ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"ADD"    ,0,0, 31,0  ,2
#ifdef VM_CODE
   ,cmd_add
#endif
   },
   {"SUB"    ,0,0, 32,0  ,2
#ifdef VM_CODE
   ,cmd_sub
#endif
   },
   {"MUL"    ,0,0, 33,0  ,2
#ifdef VM_CODE
   ,cmd_mul
#endif
   },
   {"DIV"    ,0,0, 34,0  ,2
#ifdef VM_CODE
   ,cmd_div
#endif
   },
   {"ADDS"   ,0,0, 41,0  ,2
#ifdef VM_CODE
   ,cmd_addstr
#endif
   },
   {"FINDS"  ,0,0, 42,0  ,2
#ifdef VM_CODE
   ,cmd_findstr
#endif
   },
   {"MIDS"   ,0,0, 43,0  ,3
#ifdef VM_CODE
   ,cmd_midstr
#endif
   },
   {"LENS"   ,0,0, 44,0  ,3
#ifdef VM_CODE
   ,cmd_lenstr
#endif
   },
   {"CONS"   ,0,0, 50,0  ,2
#ifdef VM_CODE
   ,cmd_cons
#endif
   },
   {"CAR"    ,0,0, 51,0  ,1
#ifdef VM_CODE
   ,cmd_car
#endif
   },
   {"CDR"    ,0,0, 52,0  ,1
#ifdef VM_CODE
   ,cmd_cdr
#endif
   },
   {"RPLACA" ,0,0, 53,0  ,2
#ifdef VM_CODE
   ,cmd_rplaca
#endif
   },
   {"RPLACD" ,0,0, 54,0  ,2
#ifdef VM_CODE
   ,cmd_rplacd
#endif
   },
   {"MKT"    ,0,0, 60,0  ,1
#ifdef VM_CODE
   ,cmd_mkt
#endif
   },
   {"ADDT"   ,0,0, 61,0  ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"RSZT"   ,0,0, 153,0  ,2
#ifdef VM_CODE
   ,cmd_resizet
#endif
   },
   {"CPYT"   ,0,0, 154,0  ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"SETT"   ,0,0, 62,0   ,3
#ifdef VM_CODE
   ,cmd_sett
#endif
   },
   {"GETT"    ,0,0, 63,0  ,2
#ifdef VM_CODE
   ,cmd_gett
#endif
   },
   {"DELT"   ,0,0, 64,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"LENT"   ,0,0, 65,0   ,1
#ifdef VM_CODE
   ,cmd_lent
#endif
   },
   {"MKF"    ,0,0,101,0   ,0      // ???????
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"COMPF"  ,0,0,102,0   ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELF"   ,0,0,103,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"MKFR"   ,0,0,104,0   ,1
#ifdef VM_CODE
   ,cmd_mkfr
#endif
   },
   {"CPFR"   ,0,0,105,0   ,2
#ifdef VM_CODE
   ,cmd_cpfr
#endif
   },
   {"ADDFRF" ,0,0,106,0   ,2
#ifdef VM_CODE
   ,cmd_addfrf
#endif
   },
   {"DELFRF" ,0,0,107,0   ,1
#ifdef VM_CODE
   ,cmd_delfrf
#endif
   },
   {"SETDFR" ,0,0,108,0   ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELDFR" ,0,0,109,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"SETDFRS",0,0,110,0   ,3
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELDFRS",0,0,111,0   ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"SETDFRL",0,0,112,0   ,3
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELDFRL",0,0,113,0   ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"SETDFRC",0,0,114,0   ,3
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELDFRC",0,0,115,0   ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"SAVEFRF",0,0,116,0   ,3
#ifdef VM_CODE
   ,cmd_savefrf
#endif
   },
   {"LOADFRF",0,0,117,0   ,2
#ifdef VM_CODE
   ,cmd_loadfrf
#endif
   },
   {"CALLFRF",0,0,118,0   ,2
#ifdef VM_CODE
   ,cmd_callfrf
#endif
   },
   {"MKST"   ,0,0, 70,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"STST"   ,0,0, 71,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELST"  ,0,0, 72,0   ,0
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"MKENV"  ,0,0, 73,0   ,0
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"STENV"  ,0,0, 74,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"DELENV" ,0,0, 75,0   ,0
#ifdef VM_CODE
   ,NULL
#endif
   },

   {"JE"     ,1,3, 80,4   ,2
#ifdef VM_CODE
   ,NULL
#endif
   },

   {"JLNG"   ,1,3,132,4   ,1
#ifdef VM_CODE
   ,cmd_jlng
#endif
   },
   {"JFLT"   ,1,3,133,4   ,1
#ifdef VM_CODE
#if USE_FLOAT
   ,cmd_jfloat
#else
   ,NULL
#endif
#endif
   },
   {"JSTR"   ,1,3, 134,4  ,1
#ifdef VM_CODE
   ,cmd_jstring
#endif
   },
   {"JSYM"   ,1,3, 135,4  ,1
#ifdef VM_CODE
   ,cmd_jsymbol
#endif
   },
   {"JLST"   ,1,3, 136,4  ,1
#ifdef VM_CODE
   ,cmd_jlist
#endif
   },
   {"JTPL"   ,1,3, 137,4  ,1
#ifdef VM_CODE
   ,cmd_jtuple
#endif
   },


   {"JF"     ,1,3, 81,4   ,1
#ifdef VM_CODE
   ,cmd_jfalse
#endif
   },
   {"JNIL"   ,1,3, 82,4   ,1
#ifdef VM_CODE
   ,cmd_jnil
#endif
   },
   {"JZ"     ,1,3, 83,4   ,1
#ifdef VM_CODE
   ,cmd_jz
#endif
   },
   {"JNF"    ,1,3, 138,4  ,1
#ifdef VM_CODE
   ,cmd_jnfalse
#endif
   },
   {"JNNIL"  ,1,3, 139,4  ,1
#ifdef VM_CODE
   ,cmd_jnnil
#endif
   },
   {"JNZ"    ,1,3, 140,4  ,1
#ifdef VM_CODE
   ,cmd_jnz
#endif
   },
   {"JMP"    ,1,3,123,4   ,0
#ifdef VM_CODE
   ,cmd_jmp
#endif
   },
   {"ENTER"  ,0,0,120,0   ,0
#ifdef VM_CODE
   ,cmd_enter
#endif
   },
   {"CALLV"  ,0,0, 84,0   ,1
#ifdef VM_CODE
   ,cmd_callv
#endif
   },
   {"CALL"   ,1,5,121,4   ,0
#ifdef VM_CODE
   ,cmd_call
#endif
   },
   {"RET"    ,0,0, 85,0   ,0
#ifdef VM_CODE
   ,cmd_ret
#endif
   },
   {"PROC"   ,0,0, 86,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"END"    ,0,0, 87,0   ,0
#ifdef VM_CODE
   ,cmd_end
#endif
   },
   {"SETENV" ,0,0, 89,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"LOAD"   ,0,0, 90,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"SAVE"   ,0,0, 91,0   ,2
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"EXEC"   ,0,0, 88,0   ,1
#ifdef VM_CODE
   ,NULL
#endif
   },
   {"EQUAL"  ,0,0, 125,0  ,2
#ifdef VM_CODE
   ,cmd_equal
#endif
   },
   {"NGT"   ,0,0, 126,0   ,2
#ifdef VM_CODE
   ,cmd_ngt
#endif
   },
   {"NLT"   ,0,0, 127,0   ,2
#ifdef VM_CODE
   ,cmd_nlt
#endif
   },
   {"NEQ"   ,0,0, 128,0   ,2
#ifdef VM_CODE
   ,cmd_neq
#endif
   },
   {"STREQ"   ,0,0, 129,0 ,2
#ifdef VM_CODE
   ,cmd_streq
#endif
   },
   {"SYMEQ"   ,0,0, 141,0 ,2
#ifdef VM_CODE
   ,cmd_symeq
#endif
   },
   {"LSTEQ"   ,0,0, 130,0 ,2
#ifdef VM_CODE
   ,cmd_lsteq
#endif
   },
   {"TPLEQ"   ,0,0, 131,0 ,2
#ifdef VM_CODE
   ,cmd_tpleq
#endif
   },
   {"LNG2FLT" ,0,0, 142,0 ,2
#ifdef VM_CODE
#if USE_FLOAT
   ,cmd_lng2flt
#else
   ,NULL
#endif
#endif
   },
   {"FLT2LNG" ,0,0, 143,0 ,2
#ifdef VM_CODE
#if USE_FLOAT
   ,cmd_flt2lng
#else
   ,NULL
#endif
#endif
   },

   {"LNG2STR" ,0,0, 144,0 ,2
#ifdef VM_CODE
   ,cmd_lng2str
#endif
   },
   {"FLT2STR" ,0,0, 145,0 ,2
#ifdef VM_CODE
#if USE_FLOAT
   ,cmd_flt2str
#else
   ,NULL
#endif
#endif
   },
   {"STR2FLT" ,0,0, 146,0 ,2
#ifdef VM_CODE
#if USE_FLOAT
   ,cmd_str2flt
#else
   ,NULL
#endif
#endif
   },
   {"STR2LNG" ,0,0, 147,0 ,2
#ifdef VM_CODE
   ,cmd_str2lng
#endif
   },
   {"SYM2STR" ,0,0, 148,0 ,2
#ifdef VM_CODE
   ,cmd_sym2str
#endif
   },
   {"STR2SYM" ,0,0, 149,0 ,2  // 150
#ifdef VM_CODE
   ,cmd_str2sym
#endif
   },

   {"LAND"   ,0,0, 150,0  ,2
#ifdef VM_CODE
   ,cmd_land
#endif
   },
   {"LOR"    ,0,0, 151,0 ,2
#ifdef VM_CODE
   ,cmd_lor
#endif
   },
   {"LNOT"    ,0,0, 155,0 ,1
#ifdef VM_CODE
   ,cmd_lnot
#endif
   },

   {"GETF"    ,0,0, 156,0 ,1
#ifdef VM_CODE
   ,cmd_getf
#endif
   },
   {"CALLP"    ,0,0, 157,0 ,1 //158
#ifdef VM_CODE
   ,cmd_callp
#endif
   },

   {"EQU"   ,0,0, 158,0   ,2  //159
#ifdef VM_CODE
   ,cmd_equal
#endif
   },

   {"NEG"   ,0,0, 159,0   ,1  //159
#ifdef VM_CODE
   ,cmd_neg
#endif
   },

   {"SETIF" ,1,6, 160,0   ,1  //159
#ifdef VM_CODE
   ,cmd_setif
#endif
   },
   {"SETIFN" ,0,0, 166,0  ,2  
#ifdef VM_CODE
   ,cmd_setifn
#endif
   },

   {"GETIF" ,1,6, 161,0   ,0  //159
#ifdef VM_CODE
   ,cmd_getif
#endif
   },
   {"CALLI" ,1,6, 162,0   ,0  //163
#ifdef VM_CODE
   ,cmd_calli
#endif
   },

   {"BREAK" ,1,6, 163,0   ,0  //164
#ifdef VM_CODE
   ,cmd_break
#endif
   },

   {"NGE"   ,0,0, 164,0   ,2
#ifdef VM_CODE
   ,cmd_nge
#endif
   },
   {"NLE"   ,0,0, 165,0   ,2  // 167
#ifdef VM_CODE
   ,cmd_nle
#endif
   },
   {"CLEAR" ,1,1, 167,4   ,0   // 168
#ifdef VM_CODE
   ,cmd_clear
#endif
   },
   {"TATOM"   ,0,0, 168,0   ,1  // 169
#ifdef VM_CODE
   ,cmd_tatom
#endif
   },
   {"STATOM"   ,0,0, 169,0   ,1  // 170
#ifdef VM_CODE
   ,cmd_statom
#endif
   },

   {""       ,0,0, 255,0
#ifdef VM_CODE
   ,NULL
#endif
   }
};
