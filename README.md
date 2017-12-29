Bench Cookie Compiler:
======================
Bench Cookie Compiler is a _pseudo-functional programming language_ (that's
what I'm calling it, at least...), written from scratch in C.

History:
--------
Lol, right. As if. BCC was originally written as an experiment in CS250, the
computer architecture course at Purdue University. It was my way of learning
ARM assembly. The compiler was terrible, but predated me taking the Purdue
compilers course (which is CS352).

Have you heard of Lex/Yacc?
----------------------------
Yes.

Why is it called Bench Cookie?
-------------------------------
Wait... _Why is it_ called Bench Cookie?

Pseudo-Functional:
==================
I've been using this term too long to start saying "functorial",
but you can just deal with it.

The main principle behind this language is the idea that there
is no such thing as evaluation. A _value_ in this language is
a _function_ from that _object_ to itself.

To use terms from category theory, a _member function_ is a _morphism_,
and an _object_ is an _object_.

Syntax:
=======
_todo: add more_

Example (fibonacci):
--------------------
```
func fib [
   capture <-- { self, int fn, int n };

   maybe self ? [
      maybe self.self ? [
	     fn <-- self.fn + self.self.fn;
	  ] or just fn = 1;
   ] or just fn = 0;

   n <-- self.n - 1;
] (n >= 0) ? --> fib;

func main [
   func fib foo;
   foo.n <-- 40;
   int f40 <-- fib;
]
```

As seen above, we can do some strange stuff.
Recursion is handled by the capturing of `self`
(self is like a back pointer to the prior instance).

Then the function is passed to itself if n >= 0.

In main, an instance of our function, foo, is created.
Then we _initialize_ our function with `n <-- 40`. The
value of the function is passed into the int, and the
computation has to begin.

Think of the functions like this:
```
fib (n = 40) --> fib (n = 39) --> ... --> fib (n = 0) --> f40
```
