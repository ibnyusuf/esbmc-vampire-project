# esbmc-vampire-project
This repo contains benchmarks and other data relating to the ongoing ESBMC - Vampire integration

## Description of Project

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

Note, that that we support having multiple candidate invariants for a loop. For example, the following is valid:

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

### Proving Invariants

Currently all attempts to prove an invariant are channeled to the Vampire theorem prover. There is no particular reason to select Vampire, any theorem prover could be used here. However, by using a superposition based theorem prover, we are able to handle *quantified* invariants. To this end, a pair of special functions `__forall(...)` and `__exists(...)` are used. For example:

```
int main() {

  int n =  1000000;
  int a[n];  
  int i = 0;

  int x; 
  __invariant(n == 1000000);
  __invariant(0 <= i && i <= n);
  __invariant(__forall((void*)(&x), (!((x < i)  && (x>= 0)) || a[x] == x) ));
  while(i < n){
    a[i] = i;
    i++;
  }

  __VERIFIER_assert(__exists((void*)(&x), (a[x] == 500)));

}
```

This is a very short term solution. A better long term solution would be to integrate a specification language such ASCL [^2] into ESBMC.

## Code 

Development on ESBMC can be found on the following branch:

https://github.com/esbmc/esbmc/tree/ahmed-vampire-for-loops

To build ESBMC, please follow the build instructions in the ESBMC repo. To run ESBMC with Vampire, the following command should be used:

```
<path to ESBMC> <path to benchmark> --vampire-for-loops --ir --output out3 --vampire-path <path to Vampire executable> --no-bounds-check
```

It is necessary to run with option `--ir` since Vampire cannot handle bit vectors currently. It is also important to add option `--no-bounds-check` as other wise ESBMC adds extra assertions potentially within a loop unrolling. Currently, we cannot handle assertions within a loop (please view TODO list).

So far, we have been using the main branch of Vampire to cary out tests. Calls to Vampire are carried via a system call as Vampire does not have a workign API currently. 

## What is in this Repo

## Useful Papers

## TODO List


[^1]: https://ssvlab.github.io/lucasccordeiro/papers/tse2012.pdf
[^2]: https://frama-c.com/html/acsl.html
