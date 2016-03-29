Bench Cookie Compiler
=====================
Bench cookie is a simple, intuitive language.
The syntax is kinda fun.

Status:
-------
This language has a LOT of work done. Note: as of right now,
we are only doing things with integers. So bad things will happen if you try and
do floating point arithmetic. Please don't file bug reports about floats not
doing what the should, because I'm telling you they won't.


Usage:
------
To compile a program, run the following. Recall: until the mips support and
x86/x86_64 architectures have been ported (Drew and Brian respectively), this
will cause errors when bcc forks gcc. Sorry.

```
    $ ./bcc out.s < source.bc
```

ToDo:
-----
 * ~~Compile automatically.~~
 * ~~Bool variables~~                         <----------- Assigned to Hunter
 * ~~Arithmetic Expressions~~                 <----------- Assigned to Hunter
 * ~~If statements~~                          <----------- Assigned to Hunter
 * ~~For loops~~			      <----------- Assigned to Hunter
 * ~~Creation of C subroutine~~		      <----------- Assigned to Hunter
 * ~~Line Comments~~                          <----------- Assigned to Drew
 * ~~Block Comments~~                         <----------- Assigned to Drew
 * Float variables, Double variables          <----------- Assigned to Hunter
 * ~~x86 / x86_64 support~~                   <----------- Assigned to Brian
 * MIPS support                               <----------- Assigned to Drew
 * Lex/Yacc style interpreter in Bench cookie <----------- Assigned to Hunter
 * Bench Cookie in Bench Cookie
 * Function Objects
 * Machine-Code Optimization
 * Network Communication Library
 * Thread library
 * Graphical User Interface Library

Interesting Ideas:
------------------
The syntax has *mostly* been made precise. We are going to continue with the linear style of code. This allows user input to be rather script-like, as this language is mostly for backend work. We are nowhere close to ready for window management... So, we will continue with this for now.

The functions will ALL be objects. They will be initialized in some order to be called later. All necessary variables and return values reside on the heap. The ENTIRE chain is on the heap. Every separate chain ends up on the stack, where the first call resides at the top.

Local Objects:
--------------
Objects are allocated on the heap. As you will read below, all functions are objects. A local object
is simply a function that extracts the value from the object! Rest assured, this is not done in
an obnoxious way (or so I think).

```
int x ?
```

So foar this only works for ints, but is supported on both ARM and x86_64.

Function Objects:
-----------------
We have a notion of function objects that allows us to direct flow from function to function,
still holding values within this function. This is done by placing all the functions on the heap.
I see it. I know your face right now. But, before you sit there liek "bruh... wut..." I can explain!
There are still function calls on the stack; HOWEVER, the actual functions take place on the heap.
Yes, I mean the actual heap. Why? Well, this makes the captures a lot easier to manage and adds
the security of randomization to the entire program.

```
func (arg1, arg2, ...) picker = [
  ; your great code here

  captured int x ?
  int y ?
] --> resultant_object
```

SO, you make the bench cookie version of a struct like this:
func bc_structure (arg1, arg2, ...) [
  captured int x ? x <-- arg1
  captured int y ? y <-- arg2
  captured int z ? ...

  ; This is a constructor!
]

Comments:
---------
```
 @;
  ; Block comments look like this
  ; and just kinda keep going.
  ; As block comments usually do.
 @;

@ This is a line comment
print "Hallo, there!\n"?
; This is also a line comment.
```

If Statements:
--------------
In the following, we test if num, an int, is greater than zero. If so, we print
"Yup." Otherwise, we print "Nope."

```
if num > 0 ? then
  print "Yup.\n" ?
endif

if num <= 0 ? then
  print "Nope.\n" ?
endif
```

Yes, I know that not having else statements is annoying.
I will probably have them... soon-ish. And, if
we are lucky, we will have else-if's, too.

Again, if you are disgusted by this syntax, please let me
know (and hopefully suggest something else).

Output Statements:
-----------------
There are two types of print statements.

**Print literals:**

```
 print "literally what you want to say\n" ?
```

**Print values:**

```
 print_<typename> varname ?
```

Input Statments:
----------------
There is a general syntax for input.

```
<typename> please? > varname
```

This reads a typename from stdin and puts it, nicely, in varname. Also, notice that lack of space on the word please. That's very important.

