// Короче говоря, получилась следующая структура
// Размеры указателей

#if ARCHITECT8
   #define N_PTR_SIZE_2 1
   #define N_PTR_SIZE 2
#else
#if ARCHITECT16
   #define N_PTR_SIZE_4 1
   #define N_PTR_SIZE 4
#endif
#endif
#if N_PTR_SIZE_4
typedef unsigned int s_int;
#elif N_PTR_SIZE_2
typedef short int s_int;
#endif

typedef struct {
//  unsigned char header;
  s_int len1;

//  unsigned short int header;
//  unsigned short int count1;

  unsigned int header;
  unsigned int count1;

//  unsigned int count;
} MEM_HEADER_REC, *TMEM_HEADER_REC;

#define BLOCK_LENGTH_MASK 0x03
#define BLOCK_LENGTH_NOT_MASK 0xFC
#define BLOCK_LENGTH_BYTE 0x01
#define BLOCK_LENGTH_WORD 0x02
#define BLOCK_LENGTH_DWORD 0x03

#define BLOCK_FUNC_MASK 0x04
#define BLOCK_FUNC_NOT_MASK 0xFB
#define BLOCK_FUNC 0x04

#define BLOCK_SWAP_MASK 0x08
#define BLOCK_SWAP_NOT_MASK 0xF7
#define BLOCK_SWAP 0x08

#define BLOCK_USE_MASK 0x80
#define BLOCK_USE_NOT_MASK 0x7F
#define BLOCK_USE 0x80

#define BLOCK_MARK_MASK 0x8000
#define BLOCK_MARK_NOT_MASK 0x7FFF
#define BLOCK_MARK 0x8000

#define BLOCK_PTR_MASK 0x4000
#define BLOCK_PTR_NOT_MASK 0xBFFF
#define BLOCK_PTR 0x4000

#if N_PTR_SIZE_4
   #define BLOCK_VALUE_LENGTH_MASK 0x00FFFFFF
   #define BLOCK_VALUE_LENGTH_NOT_MASK 0xFF000000
   #define BLOCK_VALUE_LENGTH
#elif N_PTR_SIZE_2
   #define BLOCK_VALUE_LENGTH_MASK 0xFFFF
   #define BLOCK_VALUE_LENGTH_NOT_MASK 0x0000
   #define BLOCK_VALUE_LENGTH
#endif

#define BLOCK_VALUE_COUNT_MASK 0xFFFF

/*
#define get_block_length(p) (((p)->header) & BLOCK_LENGTH_MASK)
#define set_block_length(p,a) ((p)->header=(((p)->header) | (a & BLOCK_LENGTH_MASK)))
#define set_block_length_byte(p) ((p)->header=(((p)->header) | BLOCK_LENGTH_BYTE))
#define set_block_length_word(p) ((p)->header=(((p)->header) | BLOCK_LENGTH_WORD))
#define set_block_length_dword(p) ((p)->header=(((p)->header) | BLOCK_LENGTH_DWORD))

#define is_block_length_byte(p) (get_block_length(p) == BLOCK_LENGTH_BYTE)
#define is_block_length_word(p) (get_block_length(p) == BLOCK_LENGTH_WORD)
#define is_block_length_dword(p) (get_block_length(p) == BLOCK_LENGTH_DWORD)

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

void monitor(char *,s_int);
s_int memfull(void);
int memcreate(s_int mem_size, int debug);
void stat_info(void);
void memdelete(void);

char *pool_memmake(s_int length);
void pool_memfree(char *p);
void pool_count_up(char *n);
void pool_count_down(char *n);
s_int get_pool_len(char *n);


unsigned short int get_block_length(char *p1);
void set_block_length(char *p1,unsigned short int a);
void set_block_length_byte(char *p1);
void set_block_length_word(char *p1);
void set_block_length_dword(char *p1);
unsigned int get_block_value_length(char *p1);
void set_block_value_length(char *p1,unsigned int a);
unsigned short int get_block_value_count(char *p1);
void set_block_value_count(char *p1,unsigned short int a);
int is_block_length_byte(char *p1);
int is_block_length_word(char *p1);
int is_block_length_dword(char *p1);
unsigned short int get_block_func(char *p1);
void set_block_func(char *p1);
void clear_block_func(char *p1);
int is_block_func(char *p1);
unsigned short int get_block_use(char *p1);
void set_block_use(char *p1);
void clear_block_use(char *p1);
int is_block_use(char *p);
void set_block_mark(char *p1);
void clear_block_mark(char *p1);
int is_block_mark(char *p);
void set_block_ptr(char *p1);
void clear_block_ptr(char *p1);
int is_block_ptr(char *p);
unsigned short int get_block_swap(char *p1);

void gc(void);
void pool_count_always(char *n);

extern int command_code_int;
extern int command_code_flag;


