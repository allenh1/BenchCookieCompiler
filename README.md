Bench Cookie Compiler
=====================
Bench cookie is a simple, intuitive language that compiles into ARM assembly.
The syntax is kinda fun.

Status:
-------
It prints stuff that is one word to the screen. To use, though, one does this.

'''
    $ ./bcc out.s < source.bc
    $ gcc out.s -o out
'''

ToDo:
-----
 * Compile automatically.
 * Save assembly if you get a "-s" flag.
 * Add the rest of the syntax, starting with `func main() { }`.

Interesting Ideas:
------------------
We are still coming up with some interesting ideas for syntax. If you have some,
feel free to fork this repo and modify this section of the readme. If we like it,
we'll merge it and try to make it work.

Multi-Action Switch statements:
-------------------------------
We have your basic switch statment, but also allow multiple results.
Example:

We have a data type for the labels, called a pick-list. The pick list is initialized as seen below, where
the function that decides what's chosen is a parameter for the pick-list.
 
'''
func [arg1, arg2, ... ] picker;

pick-list list(picker);

pick from list {
  one: 
    ....
  two:
    ....
  three:
    ....
  .
  .
  .
}
'''

Comments:
---------
'''
 @;
  ; Block comments look like this
  ; and just kinda keep going.
  ; As block comments usually do.
 @;

@ This is a line comment
print "Hallo, there!\n"?
; This is also a line comment.
'''

Output Statements:
-----------------
There are two types of print statements.

*Print literals:*

'''
 print "literally what you want to say\n" ?
'''

*Print values:*

'''
 print_<typename> varname ?
'''

Input Statments:
----------------
There is a general syntax for input.

'''
<typename> please ? > varname
'''

This reads a typename from stdin and puts it, nicely, in varname.


About:
======
That is the question...
