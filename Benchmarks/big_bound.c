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

int main()
{
  int i =0;
  int j = 0;;
  int sum = 0;
  int INT_MAX = 2147483647;
  __invariant(sum + (45000-i * 45000-j) < INT_MAX);
  while(i < 45000){
    j = 0;
    __invariant(sum + (45000-i * 45000-j) < INT_MAX);
    __invariant(i < 45000);
    while(j < 45000){
      sum ++;
      j++;
    }
    i++;
  }
  __VERIFIER_assert(sum < INT_MAX );

}
