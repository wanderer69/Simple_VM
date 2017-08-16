typedef struct {
    char * name;
    char narg;
    char targ;
    unsigned char code;
    char nbyte;
    char sarg;
#ifdef VM_CODE
    void (*command)(void);
#endif
} AWORD;

#ifdef VM_CODE
extern AWORD aword[]; 
#else

#endif
