#ifdef	__cplusplus
extern "C" {
#endif
//---------------------------------------------------------------------------

#define get_global_symbol     get_n_tuplevalue(global_node_root, 0)
#define get_global_func       get_n_tuplevalue(global_node_root, 1)
#define get_global_inter      get_n_tuplevalue(global_node_root, 2)
#define get_global_extfunc    get_n_tuplevalue(global_node_root, 5)
#define get_global_stack      get_n_tuplevalue(global_node_root, 3)
#define get_global_adjustment get_n_tuplevalue(global_node_root, 7)
#define get_global_streamclosure get_n_tuplevalue(global_node_root, 8)

#define set_global_symbol(v)     set_n_tuplevalue(global_node_root, 0, v)
#define set_global_func(v)       set_n_tuplevalue(global_node_root, 1, v)
#define set_global_inter(v)      set_n_tuplevalue(global_node_root, 2, v)
#define set_global_stack(v)     set_n_tuplevalue(global_node_root, 3, v)
#define set_global_extfunc(v)    set_n_tuplevalue(global_node_root, 5, v)
#define set_global_adjustment(v) set_n_tuplevalue(global_node_root, 7, v)
#define set_global_streamclosure(v) set_n_tuplevalue(global_node_root, 8, v)

#define set_n_funcconst(p, v)   set_n_tuplevalue(p, 0, v)
#define set_n_funcarg(p, v)     set_n_tuplevalue(p, 1, v)
#define set_n_funcloc(p, v)     set_n_tuplevalue(p, 2, v)
#define set_n_funcenv(p, v)     set_n_tuplevalue(p, 3, v)
#define set_n_funcfunc(p, v)    set_n_tuplevalue(p, 4, v)
#define set_n_funccode(p, v)    set_n_tuplevalue(p, 5, v)
#define set_n_funcmname(p, v)   set_n_tuplevalue(p, 6, v)

#define get_n_funcconst(p)   get_n_tuplevalue(p, 0)
#define get_n_funcarg(p)     get_n_tuplevalue(p, 1)
#define get_n_funcloc(p)     get_n_tuplevalue(p, 2)
#define get_n_funcenv(p)     get_n_tuplevalue(p, 3)
#define get_n_funcfunc(p)    get_n_tuplevalue(p, 4)
#define get_n_funccode(p)    get_n_tuplevalue(p, 5)
#define get_n_funcmname(p)   get_n_tuplevalue(p, 6)

#define get_n_closureconst(p)   get_n_tuplevalue(p, 0)
#define get_n_closurearg(p)     get_n_tuplevalue(p, 1)
#define get_n_closureloc(p)     get_n_tuplevalue(p, 2)
#define get_n_closureenv(p)     get_n_tuplevalue(p, 3)
#define get_n_closurefunc(p)    get_n_tuplevalue(p, 4)
#define get_n_closurecode(p)    get_n_tuplevalue(p, 5)

#define get_n_closurestackc(p)  get_n_tuplevalue(p, 6)
#define get_n_closurestackd(p)  get_n_tuplevalue(p, 7)

#define get_n_closureextenv(p)  get_n_tuplevalue(p, 8)
#define get_n_closuremname(p)   get_n_tuplevalue(p, 9)
#define get_n_closureprev(p)    get_n_tuplevalue(p, 10)
#define get_n_closurecounter(p)   get_n_tuplevalue(p, 11)

#define set_n_closureconst(p, v)   set_n_tuplevalue(p, 0, v)
#define set_n_closurearg(p, v)     set_n_tuplevalue(p, 1, v)
#define set_n_closureloc(p, v)     set_n_tuplevalue(p, 2, v)
#define set_n_closureenv(p, v)     set_n_tuplevalue(p, 3, v)
#define set_n_closurefunc(p, v)    set_n_tuplevalue(p, 4, v)
#define set_n_closurecode(p, v)    set_n_tuplevalue(p, 5, v)

#define set_n_closureextenv(p, v)  set_n_tuplevalue(p, 8, v)
#define set_n_closuremname(p, v)   set_n_tuplevalue(p, 9, v)
#define set_n_closureprev(p, v)    set_n_tuplevalue(p, 10, v)
#define set_n_closurecounter(p, v)   set_n_tuplevalue(p, 11, v)

#define get_current_env      get_n_closureenv(get_n_tuplevalue(get_global_streamclosure,global_stream_closure))
#define get_current_const    get_n_closureconst(get_n_tuplevalue(get_global_streamclosure,global_stream_closure))
#define get_current_args     get_n_closurearg(get_n_tuplevalue(get_global_streamclosure,global_stream_closure))
#define get_current_locs     get_n_closureloc(get_n_tuplevalue(get_global_streamclosure,global_stream_closure))
#define get_current_funcs    get_n_closurefunc(get_n_tuplevalue(get_global_streamclosure,global_stream_closure))
#define get_current_extenv   get_n_closureextenv(get_n_tuplevalue(get_global_streamclosure,global_stream_closure))
#define get_current_closureprev   get_n_closureprev(get_n_tuplevalue(get_global_streamclosure,global_stream_closure))
#define get_current_closurecounter   get_n_longvalue(get_n_closurecounter(get_n_tuplevalue(get_global_streamclosure,global_stream_closure)))
#define set_current_closurecounter(v)   set_n_longvalue(get_n_tuplevalue(get_n_tuplevalue(get_global_streamclosure,global_stream_closure), 11), v)

#define get_current_stackd   get_n_tuplevalue(global_node_root, 3)
#define get_current_stackc   global_frame.stackc

#define get_current_mname    get_n_closuremname(get_n_tuplevalue(get_global_streamclosure,global_stream_closure))

#define current_pc           get_current_closurecounter
#define current_code         get_n_stringvalue(get_n_closurecode(get_n_tuplevalue(get_global_streamclosure,global_stream_closure)))

//PNODE make_global_symbol(char *f);
void load_funcarg(void);
//s_int command_loop(s_int debug_code);
void initialize_loop(int size_dstack, int size_bstack);
//PNODE make_global_symbol(char *f);

void push_to_bstack(unsigned int v);
unsigned int pop_from_bstack(void);

int command_loop(int debug_code);

void load_function_string(char *b,  char * file1);
void print_timing(void);


//#define fatal_error printf
//---------------------------------------------------------------------------
#ifdef	__cplusplus
}
#endif

