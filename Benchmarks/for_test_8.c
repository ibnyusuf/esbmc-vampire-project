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

#define FORALL(Var, Type, Cond)       \
  Type Var;                           \
  __invariant(__forall(Var, Cond));   \

#define EXISTS(Var, Type, Cond)       \
  Type Var;                           \
  __invariant(__exists(Var, Cond));   \

int __VERIFIER_nondet_int();

int main() {
    int x = 0;
    int m = 0;
    int n = __VERIFIER_nondet_int();

    __invariant(n < 0 || x <= n);
    __invariant(m == 0 || m < x);

    for (; x < n; x = x + 1) {
        if (__VERIFIER_nondet_int()) {
            m = x;
        }
    }

    if (n > 0) {
        __VERIFIER_assert(m < n);
        //assert (m >= 0);
    }
}
