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

int main()
{
    int x = 0;
    int y = __VERIFIER_nondet_int();
    int z = __VERIFIER_nondet_int();

    // Modified invariant that is a tautology that always doesn't hold
    // __invariant(x == 0 && z < y);  // This invariant always evaluates to false

    __invariant(z >= y); 
    for (; x < 500; x++) {
        if (z <= y) {
            y = z;
        }
    }

    __VERIFIER_assert(z >= y);
}

