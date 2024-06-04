extern void abort(void);
extern void __assert_fail(const char *, const char *, unsigned int, const char *) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));
void reach_error() { __assert_fail("0", "vnew2.c", 3, "reach_error"); }
extern void abort(void);
void assume_abort_if_not(int cond) {
  if(!cond) {abort();}
}
void __VERIFIER_assert(int cond) {
  if (!(cond)) {
    ERROR: {reach_error();abort();}
  }
  return;
}

int __VERIFIER_nondet_int();

int max(int x, int y){
  if(x > y) return x;
  return y;
}

int main()
{
  int i, j;
  int sum = 0;
  Int INT_MAX = 2147483648;
  //__invariant(sum + (45000-i * 45000-j) < INT_MAX)
  for(i = 0; i < 45000; i++){
    //__invariant(sum + (45000-i * 45000-j) < INT_MAX)
    //__invariant(i < 45000)
    for(j = 0; j < 45000; j++){
      sum ++;
    }
  }
  __VERIFIER_assert(sum < INT_MAX );

}
