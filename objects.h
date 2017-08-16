#ifdef	__cplusplus
extern "C" {
#endif
typedef char *PNODE;

extern PNODE global_node_root;
extern PNODE global_const_nil;
extern PNODE global_const_false;
extern PNODE global_const_true;

extern PNODE global_const_slabel;
extern PNODE current_ret;

#define ROOT global_node_root
#define NIL global_const_nil
#define TRUE global_const_true
#define FALSE global_const_false
#define SLABEL global_const_slabel

PNODE set_n_value(PNODE v, PNODE *n);
void clear_n_value(PNODE *n);

//#define set_n_type(p, a)       ((get_p_node(p))->n_type)=((((get_p_node(p))->n_type) & MASK_NOT_TYPE) | (a & MASK_TYPE))
//#define set_n_stype(p, a)      ((get_p_node(p))->n_type)=((((get_p_node(p))->n_type)  & MASK_NOT_SUBTYPE) | ((a << SHIFT_SUBTYPE) & MASK_SUBTYPE))

#define get_n_nodetype(p) (*(p))
#define set_n_nodetype(p, a) (*(p)=a)

#define N_HEADER_SIZE 1

#define N_BACK_SIZE 4

#define N_LONG 1
#if USE_FLOAT
#define N_FLOAT 2
#endif
#define N_STRING 10
#define N_SYMBOL 11
#define N_CONS 20
#define N_TUPLE 30
#define N_FUNC 31
#define N_CLOS 32
#define N_FRAME  33
#define N_STACK 40
#define N_STACKP 41
#define N_ENVEXT 50

#define MASK_TYPE        0x0f
// 0x07
#define MASK_NOT_TYPE    0xf0
// 0xf8
#define SHIFT_TYPE       0
// 0
#define MASK_SUBTYPE     0x30
// 0x18        0b00110000
#define MASK_NOT_SUBTYPE 0xcf
// 0xe7        0b11001111
#define SHIFT_SUBTYPE    4
// 3
#define MASK_PTYPE       0xc0
// 0xe0
#define MASK_NOT_PTYPE   0x3f
// 0x1f
#define SHIFT_PTYPE      6
// 5

#define STACK_SIZE 50000

/*
#define NFREE         (((0 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((0 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // узел свободен
#define N_SYMBOL      (((1 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((0 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // символ
#define N_LONG        (((2 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((0 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // целое
#if USE_FLOAT
#define N_FLOAT       (((3 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((0 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // плавучее
#endif
#define N_STRING      (((4 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((0 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // строка
#define N_STACKP      (((7 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((0 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // массив элементов типа long
// списки
#define N_CONS        (((5 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((0 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // списочная ячейка
//#define N_ENV         (((5 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((1 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // окружение - список из парных елементов
// кортежи
#define N_TUPLE       (((6 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((0 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // кортеж
#define N_STACK       (((6 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((1 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // стек
#define N_FRAME       (((6 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((2 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // фрейм
// функции
#define N_FUNC        (((6 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((3 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // функция - фактически кортеж
#define N_CLOS        (((6 & MASK_TYPE) & MASK_NOT_SUBTYPE) | ((4 << SHIFT_SUBTYPE) & MASK_SUBTYPE))    // замыкание
*/

#if ARCHITECT8
   #define N_LONG_SIZE_2 1
   #define N_LONG_SIZE 2
   #define N_CONS_SIZE_2 1
#else
#if ARCHITECT16
   #define N_LONG_SIZE_4 1
   #define N_LONG_SIZE 4
#if USE_FLOAT
   #define N_FLOAT_SIZE 8
#endif
   #define N_CONS_SIZE_4 1
#endif
#endif

#if N_LONG_SIZE_4
//typedef unsigned long s_int;
#ifdef USE_FLOAT
typedef double s_flt;
#endif
#elif N_LONG_SIZE_2
//typedef short int s_int;
#endif

extern s_int global_stream_closure; // номер исполняемого глобального потока
extern s_int global_end_exec;

#define get_n_type(p) (*((char *)p))
//#define get_n_type(p) ((*((char *)p))& MASK_TYPE)
//#define get_n_type(p) ((*((char *)p)) & 0xf0)
#define get_n_stype(p) (((*((char *)p)) & MASK_SUBTYPE) >> SHIFT_SUBTYPE)

#define set_n_stype(p, a)     (get_n_type(p))=( ( (get_n_type(p))  & MASK_NOT_SUBTYPE) | ((a << SHIFT_SUBTYPE) & MASK_SUBTYPE))

//#define get_n_type(p)          (((get_p_node(p))->n_type) & MASK_TYPE)
//#define get_n_stype(p)         ((((get_p_node(p))->n_type) & MASK_SUBTYPE) >> SHIFT_SUBTYPE)

#define is_n_atom(p) ((get_n_type(p)<N_CONS)? 1 : 0)

PNODE make_n_long(s_int l);
void set_n_longvalue(PNODE p, s_int l);
s_int get_n_longvalue(PNODE p);

#define is_n_long(p) ((get_n_type(p) == N_LONG)? 1 : 0)

#if ARCHITECT16
PNODE make_n_float(s_int l);
s_int get_n_floatvalue(PNODE p);
void set_n_floatvalue(PNODE p, s_int l);
#define is_n_float(p) ((get_n_type(p) == N_FLOAT)? 1 : 0)
#endif

#if USE_FLOAT
#define is_n_numeric(p) (((get_n_type(p) == N_LONG) || (get_n_type(p) == N_FLOAT))? 1 : 0)
#else
#define is_n_numeric(p) (((get_n_type(p) == N_LONG))? 1 : 0)
#endif

void set_n_stringvalue(PNODE p, char *s);
char *get_n_stringvalue(PNODE p);
PNODE make_n_string(char *s);
#define get_n_stringlen(p) (get_pool_len((char *)p)-N_HEADER_SIZE)
#define is_n_string(p) (((get_n_type(p) >= N_STRING) && (get_n_type(p) < N_CONS))? 1 : 0)

PNODE cont_n_string(PNODE p1, PNODE p2);
long find_n_string(PNODE p1, PNODE p2);
PNODE mid_n_string(PNODE p, s_int b, s_int l);

#define set_n_symbolvalue set_n_stringvalue
#define get_n_symbolvalue get_n_stringvalue
PNODE make_n_symbol(char *s);
//#define make_n_symbol make_n_string
#define get_n_symbollen get_n_stringlen
#define is_n_symbol(p) ((get_n_type(p) == N_SYMBOL)? 1 : 0)

PNODE make_n_list(PNODE a, PNODE d);
PNODE get_n_listcar(PNODE p);
PNODE get_n_listcdr(PNODE p);
void set_n_listcdr(PNODE p, PNODE l);
void set_n_listcar(PNODE p, PNODE l);

#define is_n_list(p) ((get_n_type(p)==N_CONS)? 1 : 0)

PNODE make_n_tuple(s_int l);
void set_n_tuplevalue(PNODE p, s_int l, PNODE a);
PNODE get_n_tuplevalue(PNODE p, s_int l);
s_int get_n_tuplelen(PNODE p);
void fill_n_tuple(PNODE p, PNODE v);
PNODE copy_n_tuple(PNODE p);
PNODE resize_n_tuple(PNODE p, s_int l);

#define is_n_tuple(p) (((get_n_type(p) >= N_TUPLE) && (get_n_type(p) < N_STACK))? 1 : 0)
#define is_n_func(p) (((get_n_type(p)==N_FUNC))? 1 : 0)
#define is_n_closure(p) (((get_n_type(p)==N_CLOS))? 1 : 0)

PNODE make_n_stack(s_int l);
PNODE get_n_stackvalue(PNODE p, s_int l);
void set_n_stackvalue(PNODE p, s_int l, PNODE a);
s_int get_n_stackptr(PNODE p);
void set_n_stackptr(PNODE p, s_int l);
s_int get_n_stacklen(PNODE p);
PNODE pop_n_stack(PNODE p);
void push_n_stack(PNODE p, PNODE v);
PNODE get_n_stacksvalue(PNODE p, s_int l);

#define make_n_array(l) make_n_tuple(l)
#define set_n_arrayvalue(p, l, a) set_n_tuplevalue(p, l, (PNODE)a)
#define get_n_arrayvalue(p, l) ((s_int)get_n_tuplevalue(p, l))

#define make_n_stackp(l) make_n_stack(l)
#define set_n_stackpvalue(p, l, a) set_n_stackvalue(p, l, (PNODE)a)
#define get_n_stackpvalue(p, l) ((s_int)get_n_stackvalue(p, l))
#define set_n_stackpptr(p, l) set_n_stackptr(p, l)
#define get_n_stackpptr(p) get_n_stackptr(p)

#define is_n_frame(p) (((get_n_type(p)==N_FRAME))? 1 : 0)
PNODE make_n_frame(PNODE n);
PNODE copy_n_frame(PNODE n, PNODE k);
void add_n_frame_slot(PNODE p1, PNODE n);
void del_n_frame_slot(PNODE p1, PNODE n);
void set_n_frame_slot_value(PNODE p1, PNODE n, PNODE v);
PNODE get_n_frame_slot_value(PNODE p1, PNODE n);
PNODE get_n_frame_slot_type(PNODE p1, PNODE n);
PNODE find_n_frame_slot(PNODE p1, PNODE n);


// in vm.c
PNODE make_n_envext(void);
PNODE add_n_envext(PNODE p, PNODE k, PNODE v);
PNODE get_n_envexts(PNODE p, char *k);
PNODE get_n_envext(PNODE p, PNODE k);
PNODE gettuple_n_envexts(PNODE p, char *k);
PNODE gettuple_n_envext(PNODE p, PNODE k);
PNODE set_n_envext(PNODE p, PNODE k, PNODE v);
PNODE make_global_symbol(char *f);
PNODE make_n_func(void);
PNODE make_n_closure(void);
PNODE make_n_closurefunc(PNODE f, PNODE curr_closure);
PNODE make_n_closurefuncn(PNODE f);
void make_gtree_root(void);


PNODE find_func_name(PNODE fname);
PNODE make_n_lists(char * s, s_int len);
PNODE make_n_tuples(char * s, s_int len);


int get_long_value(char *p);
void set_long_value(char *p, int r);
PNODE make_n_stringn(char * s, s_int len);

extern PNODE CRIT_REG;

extern s_int global_end_exec;
extern s_int global_debug_code;
extern s_int global_reg_error;
extern s_int global_interrupt_ext;

extern PNODE global_current_command_string; // текущая строка кода

extern int global_arg_point; // точка от которой отсчитываются аргументы
extern int global_var_point; // точка от которой отсчитываются переменные
extern int global_type_vm_func; // тип выполняемой фенкции
extern int global_pc_counter; // положение
extern int global_const_point; // точка от которой отсчитываются константы
extern int global_func_point; // точка от которой отсчитывают список функций

extern int global_prev_point; // точка в которой лежит предыдушая функция если 0 то значит это стартовая функция
extern PNODE global_prev_func; // предыдушая функция

void copy_func_to_stack(PNODE f);

void print_n_stackvalue(PNODE p);

extern PNODE global_prev_func_p; // предыдушая функция
extern PNODE global_current_func; // текущая функция

typedef struct {
  unsigned int score;
  unsigned int time;
} COMMAND_TIMING, *PCOMMAND_TIMING;

extern PCOMMAND_TIMING global_timing; // указатель на стек возвратов

#ifdef	__cplusplus
}
#endif


