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

Note, that we support having multiple candidate invariants for a loop. For example, the following is valid:

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

This is a very short-term solution. A better long-term solution would be to integrate a specification language such ACSL [^2] into ESBMC.

## Code 

All modifications to ESBMC relating to this project can be found on the following branch:

https://github.com/esbmc/esbmc/tree/ahmed-vampire-for-loops

To build ESBMC, please follow the build instructions in the ESBMC repo. To run ESBMC with Vampire, the following command should be used:

```
<path to ESBMC> <path to benchmark> --vampire-for-loops --ir --output <output file name> --vampire-path <path to Vampire executable> --no-bounds-check
```

It is necessary to run with option `--ir` since Vampire cannot handle bit vectors currently. It is also important to add option `--no-bounds-check` as other wise ESBMC adds extra assertions potentially within a loop unrolling. Currently, we cannot handle assertions within a loop (please view TODO list). The option `--output` is used to specify the name of a file to which SMT problems are written. At times it can be useful to add an `abort()` statement to ESBMC to stop execution at a particular point and inspect the content of this file. This allows us to analyse the data being passed from ESBMC to Vampire.

So far, we have been using the main branch of Vampire to carry out tests. Vampire is invoked via a system call as it does not have a working API currently. 

## What is in this Repo

Besides for this README, this repo contains a set of benchmarks that I have been using to test the implementation. Some of these are handcrafted and many others come from:

https://github.com/PL-ML/code2inv/tree/master/benchmarks/C_instances/c

## Useful Papers

The following is a list of papers that have been useful so far or may be useful in the future of this project. Please be aware that this selection is very limited and there are of course many other relevant papers. The fields of invariant generation and program verification are huge.

+ [Code2Inv: A Deep Learning Framework for Program Verification](https://link.springer.com/chapter/10.1007/978-3-030-53291-8_9)
+ [LEMUR: Integrating Large Language Models in Automated Program Verification](https://arxiv.org/pdf/2310.04870.pdf) I think that this paper is very important and it gives me a lot of hope that the approach that we are taking could be very competitive.
+ [Scalable and Precise Verification Based on k-induction](https://eprints.soton.ac.uk/433530/1/Final_Thesis.pdf)
+ [Loop Invariants, Analysis, Classification and Examples](https://se.inf.ethz.ch/~meyer/publications/methodology/invariants.pdf)
+ [Learning Shape Analysis](https://link.springer.com/content/pdf/10.1007/978-3-319-66706-5_4.pdf)
+ [From Invariant Checking to Invariant Inference Using Randomized Search](https://theory.stanford.edu/~aiken/publications/papers/cav14.pdf)
+ [Can ChatGPT Support CPA-Checker with ...](https://cpa.sosy-lab.org/2023/slides/full-talk-1.pdf)
+ [Learning Loop Invariants for Program Verification](https://papers.nips.cc/paper/2018/file/65b1e92c585fd4c2159d5f33b5030ff2-Paper.pdf)
+ [Neural Termination Analysis](https://arxiv.org/abs/2102.03824)

## TODO List

The basic implementation now seems to be working reasonably well (albeit tested over a small set of relatively simple benchmarks). However, there are number of tasks outstanding of varying levels of importance:

- [ ] Add support for `for` and `do-while` loops. Currently, we only support `while` loops.
- [ ] Currently we prove *partial correctness*. That is, once we replace the loops with invariants we assume the negation of the loop condition. We could / should also prove termination.
- [ ] Support `break` statements. A `break` statement in a loop provides an alternative loop exit point making the proving of candidate invariants significantly more complex as we need to show that the invariant holds at every loop exit location.
- [ ] Recursive function calls
- [ ] Add support for assertion statements *within* loops. Such statements are essentially candidate invariants as they need to hold on every iteration of a loop. However, unlike the candidate invariants specified using `__invariant(..)` they don't necessarily have to hold on loop entrance or even on loop exit.
- [ ] Support loops with conjunction / disjunction *and* sideefects (the code [here](https://github.com/esbmc/esbmc/blob/7ddb6251fa7c851800b768ed97922af9dbf321df/src/goto-programs/goto_convert.cpp#L1887) needs updating.
- [ ] Currently, it is difficult to refer to the value of a variable or expression at a particular program location. For example, it is common in functional verification that we may wish to prove that the value of some variable `a` at the end of a function is related to its value at the start in some way. To do that right now, we have to manually insert an auxiliary variable at the start of the function which is then unchanged throughout the function. A nicer way, would be to add a command to the specification language that allows us to reference various locations. Of course, if we move to using ACSL or some similar specification language this is solved for us automatically.
- [ ] Update Vampire to support bit vectors. Currently it doesn't forcing us to use a less accurate integer encoding.
- [ ] The big one: introduce automatic candidate inference generation. My vision is to use LLMs for this. Please view paper links above for examples of some steps in this direction. There are of course other invariant generation techniques that we may wish to explore as well such as Daikon [^3].
- [ ] Another big one: add support for reasoning about memory manipulating programs. This could entail reasoning about separation. It will be really interesting to see whether this can be combined with invariant generation sucessfully.

## Helpful Hints

The ESBMC code base is pretty large and it takes some time to become familiar with it. Here are some things that I wish I knew before I started work on the codebase:

+ ESBMC mantains two internal representations [`irep`](https://github.com/esbmc/esbmc/blob/7ddb6251fa7c851800b768ed97922af9dbf321df/src/util/irep.h#L35) and [`irep2`](https://github.com/esbmc/esbmc/blob/7ddb6251fa7c851800b768ed97922af9dbf321df/src/irep2/irep2.h#L176). `irep` is the older of the two, based on strings, and actually the easier to use. `irep2` is heavily based on templates and uses some fancy Boost macros to produce a lot of boiler plate code. To convert between the two there are various utility functions located in [here](https://github.com/esbmc/esbmc/blob/7ddb6251fa7c851800b768ed97922af9dbf321df/src/util/migrate.h#L1). Both can be printed out using a `dump()` method. This produces a detailed AST for the expression. To convert the internal representation back to something that resembles C code, we can use [this function](https://github.com/esbmc/esbmc/blob/7ddb6251fa7c851800b768ed97922af9dbf321df/src/langapi/language_util.h#L18) or the one below it.
+ The `log_debug(...)` function only produces output if verbosity is set high enough. Thus, in some cases it makes sense to use `log_status(...)` or just a plain old `std::cout << ...` instead.
+ If anything in the above is unclear, I can always be contacted by email: ahmed_bhayat at hotmail.com.
 
[^1]: https://ssvlab.github.io/lucasccordeiro/papers/tse2012.pdf
[^2]: https://frama-c.com/html/acsl.html
[^3]: https://people.csail.mit.edu/cpacheco/publications/daikon-tool-scp2006.pdf
