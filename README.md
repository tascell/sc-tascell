# sc-tascell
Backtrcking-based load balancing framework

## What is Tascell?
Tascell is a parallel a work-stealing based parallel programming language, which has the following advantages:

- You can efficiently parallelize irregular applications, such as tree/graph search applications.
- Tascell supports distributed memory environments as well as shared memory environments. It also supports heterogeneous environments and wide-area distributed computing environments where multiple clusters are connected in WAN each other.
- You can add new computating nodes during the computation.
- You can write a Tascell program with reasonble programming cost starting with a sequential C program.

Unlike LTC-based multithreaded languges such as Cilk (recently incorporated in Intel Parallel Composer as Cilk Plus),
and X10, Tascell worker does not create any threads without any requests. When the worker receives a request, it performs temporarily backtracking 
(goes back to the past), spawns a new task, returns from the backtracking (restores the time), and it resumes its own task.

## The SC Language System
The Tascell compiler is implemented as a translater to C based on the SC language system. 
The SC language is a C language with S-expression-based (Scheme-like) syntax, implemented in Common Lisp. For example, the following C code:

```
long sum(long *ar, int n){
  long s=0;
  int i=0;
  do{
    if (i >= n) break;
    s += ar[i++];
  } while(1);
  return s;
}
```
is equivalent to
```
(def (sum ar n) (fn long (ptr long) int)
  (def s long 0)
  (def i int 0)
  (do-while 1
    (if (>= i n) (break))
    (+= s (aref ar (inc i))) )
  (return s) )
```
We can implement an extended language to an SC lanugage by wrinting "transformation rules." 
Transformation rules are written in the extended Common Lisp with the pattern-matching facility over S-expressions.
The archive includes transformation rule-sets for realizing several extended SC languages.

## Download
```
git clone git@github.com:tascell/sc-tascell.git
```

## System Requirements
* OS: We support either of

  - Microsoft Windows + Cygwin
  - Linux

* Common Lisp implementation: We support either of

  - Allegro Common Lisp Version 7.x or later
  - GNU CLISP Version 2.xx or later
  
* To execute Tascell programs in distributed memory environments, you need either of

  - Allegro Common Lisp Version 7.x or later
  - Steel Bank Common Lisp Version 1.0.xx or later

The "indent" utility (a C program formatter) is required. 
You can install this using a package system (e.g., yum, apt-get) on most Linux distribution.

## Installation of the SC Language System
1. Install Lisp

    Please refer to the documents of the Lisp you want to use.

2. Test Common Lisp in sc/src/
```
% cd sc/src
% alisp     # or "clisp"
```

3. Install SC language system

    1.
     > cd src/c2scpp

    2.
     > ./configure && make

    3. Add 
    > (%cinclude "<stdio.h>" (:macro))
    into your SC code to include declarations in ``<stdio.h>``.

    4. To include C macros, set the :macro keyword argument as
    > (%cinclude "<stdio.h>" (:macro NULL stdin stdout)).

    5. To include multiple C header files, write as
    > (%cinclude "<stdio.h>" "<math.h>" (:macro NULL stdin stdout))

3.1 Run Common Lisp in ``sc/src/``
```
% cd sc/src
% alisp     # or "clisp"
```

3.2 Load "init.lsp":
> (load "init.lsp")

3.3 To translate an SC program into a C program, call the sc2c function. For example:
> (sc2c "sample/fib.sc")

3.4 You can compile the resulting C program using a C compiler (e.g., gcc) and run it.
```
% cd sample
% gcc fib.c -o fib
% ./fib 30
```

3.5 If you would like to compile an SC file from shell command line directly:

      3.5.1 Make a copy of ``bin/sc-setenv.sh``
      
      3.5.2 Modify the value of ``SC_PREFIX`` to your SC directory.
      
      3.5.3 Execute ``"source <modified sc-setenv.sh>"``.
      
      3.5.4 You can translate an SC program by ``"sc2c <sc-file> <options>"``
      
      (<options> are defined in src/sc-cmdline.lsp. Details to be written.)
      
      3.5.5 If necessary, copy the contents into a startup file of your login shell.
      
