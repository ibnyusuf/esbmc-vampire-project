extern void abort(void);
extern void __assert_fail(const char *, const char *, unsigned int, const char *) __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));
void reach_error() { __assert_fail("0", "vnew2.c", 3, "reach_error"); }
extern void abort(void);
void assume_abort_if_not(int cond) {
    if (!cond) {
        abort();
    }
}
void __VERIFIER_assert(int cond) {
    if (!(cond)) {
        ERROR: { reach_error(); abort(); }
    }
    return;
}

#define FORALL(Var, Type, Cond) \
    Type Var;                     \
    __invariant(__forall(Var, Cond)); \

#define EXISTS(Var, Type, Cond) \
    Type Var;                     \
    __invariant(__exists(Var, Cond)); \

int __VERIFIER_nondet_int();

int main() {
    // variable declarations
    int i;
    int size = __VERIFIER_nondet_int();
    int sn;
    // pre-conditions
    (sn = 0);
    (i = 1);
    // loop body
    __invariant(sn == i - 1);
    __invariant(sn == 0 || size >= 0);
    __invariant(size < 0 || i <= size + 1);
    for (; i <= size; i = (i + 1), sn = (sn + 1)) {
    }
    // post-condition
    if (sn != 0)
        __VERIFIER_assert(sn == size);

    return 0;
}
