int main (void) {
    register int a = 0;
    register int b = 2;
    register volatile int c;

     
     c = a + b;
     return c;
}
