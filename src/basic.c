int add( register int a, register int b){
  register int sum ;
  __asm__ __volatile__ (
                        "qadd\t %0, %1, %2\n"
                        : "=r" ( sum )
                        : "r" ( a ) , "r" ( b )
                        );
  return( sum );
}