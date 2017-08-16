PNODE find_n_frame_slot_by_num(PNODE p1, int l)
{
 long i;
 PNODE p, ret;
   ret=FALSE;
   p=get_n_tuplevalue(p1,2);
//   win_printf(">>> %d\r\n",l);
   if((l>=0) && (l<get_n_tuplelen(p))){
      ret=get_n_tuplevalue(p,l);
//      print(ret);
   };
   return(ret);
};

int get_n_slot_num(PNODE p1)
{
 int l;
 PNODE p;
   p=get_n_tuplevalue(p1,2);
   l=get_n_tuplelen(p);
   return(l);
};

