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

    For `clisp`, you can install by the steps below:

    ```
    % wget ftp://ftp.gnu.org/pub/gnu/clisp/release/2.49/clisp-2.49.tar.gz 
    % tar xvfz clisp-2.49.tar.gz
    % cd clisp-2.49 
    % ./configure --ignore-absence-of-libsigsegv build-dir
    % make
    ```

    After the installation is success, you can add `clisp` to `PATH` for convenience.
    ```
    % export PATH="$HOME/clisp-2.49/build-dir:$PATH"
    ```

2. Test Common Lisp in ``sc/src/``
    ```
    % cd sc/src
    % alisp     # or "clisp"
    ```

3. Install SC language system

    1.
     > cd src/c2scpp

    2.
     > ./configure && make

    3. Add ``(%cinclude "<stdio.h>" (:macro))`` into your SC code to include declarations in ``<stdio.h>``.

    4. To include C macros, set the ``:macro`` keyword argument as
    > (%cinclude "<stdio.h>" (:macro NULL stdin stdout)).

    5. To include multiple C header files, write as
    > (%cinclude "<stdio.h>" "<math.h>" (:macro NULL stdin stdout))

4. Run Common Lisp in ``sc/src/``
    ```
    % cd sc/src
    % alisp     # or "clisp"
    ```

5. Load "init.lsp":
> (load "init.lsp")

6. To translate an SC program into a C program, call the sc2c function. For example:
> (sc2c "sample/fib.sc")

7. You can compile the resulting C program using a C compiler (e.g., gcc) and run it.
    ```
    % cd sample
    % gcc fib.c -o fib
    % ./fib 30
    ```

8. If you would like to compile an SC file from shell command line directly:

      1. Make a copy of ``bin/sc-setenv.sh``
      
      2. Modify the value of ``SC_PREFIX`` to your SC directory.

      3. Execute ``"source <modified sc-setenv.sh>"``.

      4. You can translate an SC program by ``"sc2c <sc-file> <options>"`` 

          (``<options>`` are defined in ``src/sc-cmdline.lsp``. Details to be written.)

      5. If necessary, copy the contents into a startup file of your login shell.
      
## Executing Tascell Programs in Shared Memory Environments
Sample programs of Tascell are located in
```
sc/src/sample/tcell/*.tcell
```
in the archive.

After the installation of SC, you can compile these Tascell programs by
```
% make xxx-gcc    # xxx is a basename of .tcell files.
```
Then, execute the generated execution program.
```
% ./fib-gcc -n 8 -i "1 44"   
```
Here, 8 is the nubmer of workers and "1 44" is the input for the fib program. 
See the comment in each .tcell source for the format of the input.

## Executing Tascell Programs in Distributed Memory Environments
To execute Tascell programs in distributed memory environments, you need to execute a Tascell Server first and then execute worker program (e.g., fib-gcc) connecting to the server process.

You can execute a Tascell Server by
```
% alisp    # or "sbcl"
> (load "LOAD.lsp")
> (ms :local-host "hhhh" :n-wait-children 4 :auto-initial-task "1 44")
   # hhhh is the hostname accessible from worker ndoes
```
Here, 4 is the number of worker nodes and "1 44" is the input for the worker program.

Then, execute the worker program on each worker node.
```
% ./fib-gcc -n 8 -s hhhh   
   # hhhh is the hostname of the server
```
The server uses the TCP/9865 connection for communications among the server and the workers. 
If you want to change the port number, specify the number by the ``:chidren-port`` keyword argument (server) 
and the ``-p`` command line argument (worker).

