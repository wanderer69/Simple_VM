void cmd_nop();
void cmd_pushc();
void cmd_drop();
void cmd_dup();
void cmd_cpa();
void cmd_savevl();
void cmd_loadvl();
void cmd_saveal();
void cmd_loadal();
void cmd_savev();
void cmd_loadv();
void cmd_add();
void cmd_sub();
void cmd_mul();
void cmd_div();
void cmd_neg();

void cmd_jz();
void cmd_jnz();
void cmd_jnil();
void cmd_jnnil();
void cmd_jfalse();
void cmd_jnfalse();
void cmd_jlng();
void cmd_jfloat();
void cmd_jstring();
void cmd_jsymbol();
void cmd_jtuple();
void cmd_jlist();
void cmd_jmp();

void cmd_enter();
void cmd_call();
void cmd_callv();
void cmd_ret();

void cmd_neq();
void cmd_ngt();
void cmd_nlt();
void cmd_nge();
void cmd_nle();
void cmd_symeq();
void cmd_lsteq();
void cmd_streq();
void cmd_tpleq();
void cmd_equal();

void cmd_land();
void cmd_lor();
void cmd_lnot();

void cmd_addstr();
void cmd_findstr();
void cmd_midstr();
void cmd_lenstr();

void cmd_lng2flt();
void cmd_flt2lng();
void cmd_lng2str();
void cmd_flt2str();
void cmd_str2lng();
void cmd_str2flt();
void cmd_str2sym();
void cmd_sym2str();

void cmd_cons();
void cmd_car();
void cmd_cdr();
void cmd_rplaca();
void cmd_rplacd();

void cmd_mkt();
void cmd_sett();
void cmd_gett();
void cmd_resizet();
void cmd_lent();

void cmd_mkfr();
void cmd_cpfr();
void cmd_addfrf();
void cmd_delfrf();
void cmd_savefrf();
void cmd_loadfrf();
void cmd_callfrf();

void cmd_end();

void cmd_getf();
void cmd_callp();

void cmd_break();

void cmd_calli();
void cmd_setif();
void cmd_setifn();
void cmd_getif();
void cmd_clear();

void cmd_statom();
void cmd_tatom();
