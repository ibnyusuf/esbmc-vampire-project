# esbmc-vampire-project
This repo contains benchmarks and other data relating to the ongoing ESBMC - Vampire integration

## Description of project

ESBMC works by bounded model checking [^1]. As part of this process, it converts the input C program into GOTO format, then carries out symbolic execution on this GOTO program to create Single Static Assignment (SSA) form. During the symbolic execution, loops are unwound to some depth k. In this integration, we  treat loops differently. To understand the approach we take, consider the following program:

```
int main() {
  // variable declarations
  int n = __VERIFIER_nondet_int();
  int v1 = __VERIFIER_nondet_int();
  int v2 = __VERIFIER_nondet_int();
  int v3 = __VERIFIER_nondet_int();
  int x;
  // pre-conditions
  (x = 0);
  // loop body

  __invariant(x <= n || n < 0);  
  while ((x < n)) {
    {
    (x  = (x + 1));
    }

  }

  // post-condition
if ( (x != n) )
__VERIFIER_assert( (n < 0) );

}
```

Note that we use a special function `__invariant(...)` to mark candidate invariants. These statements must appear on the line(s) before a loop.

When symbolic execution reaches the loop statement, it attempts to prove that the candidate invariant holds (base case). The loop variables are havoced (set to non-deterministic values) and the loop is then symbolically executed a single time. Taking the resulting loop SSA and assuming the inductive hypothesis, we attempt to prove the inductive conclusion. If this succeeds we have proven the candidate invariant to be a true invariant. The loop is replaced with the candidate invariant and symbolic execution continues as normal. 

Note, that that we support having multiple candidate invariants for a loop. FOr example, the following is valid:

```
int main() {
  __invariant(cand1);
  __invariant(cand2);
  __invariant(cand3); 
  while ((x < n)) {
    ...
  }
}
```

In such a case, the order in which we attempt to prove the candidates to be invariants may affect what can be proved. To deal with this we make multiple proof passes through the invariants and stop when either all candidates have been proven, or we do not prove any new invariants on a particular pass.

[^1]
