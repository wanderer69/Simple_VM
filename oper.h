PNODE n_add(PNODE p1, PNODE p2);
PNODE n_sub(PNODE p1, PNODE p2);
PNODE n_mul(PNODE p1, PNODE p2);
PNODE n_div(PNODE p1, PNODE p2);
PNODE n_neg(PNODE p1);

PNODE n_arith(PNODE p1, PNODE p2, char d);

PNODE n_numeq(PNODE p1, PNODE p2);
PNODE n_numgt(PNODE p1, PNODE p2);
PNODE n_numlt(PNODE p1, PNODE p2);
PNODE n_stringeq(PNODE p1, PNODE p2);
PNODE n_symboleq(PNODE p1, PNODE p2);
PNODE n_listeq(PNODE p1, PNODE p2);
PNODE n_tupleeq(PNODE p1, PNODE p2);
PNODE n_equal(PNODE p1, PNODE p2);

PNODE n_addstring(PNODE p1, PNODE p2);
PNODE n_findstring(PNODE p1, PNODE p2);
PNODE n_midstring(PNODE p1, PNODE p2, PNODE p3);
PNODE n_lenstring(PNODE p1);

PNODE n_long_to_float(PNODE p);
PNODE n_float_to_long(PNODE p);
PNODE n_long_to_string(PNODE p);
PNODE n_float_to_string(PNODE p);
PNODE n_string_to_long(PNODE p);
PNODE n_string_to_float(PNODE p);
PNODE n_symbol_to_string(PNODE p);
PNODE n_string_to_symbol(PNODE p);

PNODE n_logand(PNODE p1, PNODE p2);
PNODE n_logor(PNODE p1, PNODE p2);
PNODE n_lognot(PNODE p1);

PNODE n_tuple(PNODE p);
void n_settuplevalue(PNODE p1,PNODE p2,PNODE p3);
PNODE n_gettuplevalue(PNODE p1,PNODE p2);
PNODE n_resizetuple(PNODE p1,PNODE p2);
PNODE n_lentuple(PNODE p1);

void n_call_func(PNODE f, long key);

PNODE n_frame(PNODE p);
PNODE n_frame_from(PNODE p, PNODE k);
void n_add_frame_slot(PNODE p1,PNODE p2);
PNODE n_get_framevalue(PNODE p1,PNODE p2);
void n_set_framevalue(PNODE p1,PNODE p2,PNODE p3);
void n_del_frame_slot(PNODE p1,PNODE p2);

void n_break(long n);

