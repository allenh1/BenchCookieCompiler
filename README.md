Bench Cookie Compiler
=====================
Bench cookie is a simple, intuitive language that compiles into ARM assembly.
The syntax is kinda fun.

Status:
-------
The compiler can now print and read strings and integers! Not much, but... PROGRESS.

```
    $ ./bcc out.s < source.bc
    $ gcc out.s -o out
```

ToDo:
-----
 * ~~Compile automatically.~~
 * Bool variables
 * Arithmetic Expressions
 * If statements
 * Line Comments
 * Block Comments
 * Function objects
 * Double/Float variables

Interesting Ideas:
------------------
The syntax has *mostly* been made precise. We are going to continue with the linear style of code. This allows user input to be rather script-like, as this language is mostly for backend work. We are nowhere close to ready for window management... So, we will continue with this for now.

The functions will ALL be objects. They will be initialized in some order to be called later. All necessary variables and return values reside on the heap. The ENTIRE chain is on the heap. Every separate chain ends up on the stack, where the first call resides at the top.

Pick Statements:
-------------------------------
Example:

We have a notion of function objects.

```
func (arg1, arg2, ...) picker = [
  potential_object ? condition,
  definite_object,
  definite_object,
] --> resultant_object
```

The `potential_object` is going to be a variable in this function if a certain
condition is satisfied. The definite_objects are garaunteed to be there. The resultant
is a pointer to the return list of the function object.

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
<typename> please ? > varname
```

This reads a typename from stdin and puts it, nicely, in varname.


About:
======
That is the question...
