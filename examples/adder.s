/**
 *adder.s
 *
 * Generated by Bench Cookie Compiler!
 * Bench Cookie is... Experimental. Don't be mad.
 */
	.bss

	.lcomm locals, 400

	.data

S0:	.ascii "Enter a number: \0"
S1:	.ascii "Enter another number: \0"
S2:	.ascii "-------------------------------\n\0"
S3:	.ascii " + \0"
S4:	.ascii " = \0"
S5:	.ascii "\n\0"
I0:	.word 0
I1:	.word 0

string_fmt:	.ascii "%s\0"
num_fmt:	.ascii "%d\0"
TRUE_STORY:	.ascii "true\0"
FALSE:	.ascii "false\0"


	.text
	.global main
	.global printf
	.global scanf
	.global malloc
main:
	ldr %r9, =locals
	@ Make space for locals
	ldr %r0, =S0
	bl printf

	ldr %r0, =num_fmt
	ldr %r1, =I0
	bl scanf

	ldr %r0, =S1
	bl printf

	ldr %r0, =num_fmt
	ldr %r1, =I1
	bl scanf

	ldr %r0, =S2
	bl printf

	ldr %r0, =num_fmt
	ldr %r1, =I0
	ldr %r1, [%r1]
	bl printf
	ldr %r0, =S3
	bl printf

	ldr %r0, =num_fmt
	ldr %r1, =I1
	ldr %r1, [%r1]
	bl printf
	ldr %r0, =S4
	bl printf

	ldr %r1, =I0
	ldr %r1, [%r1]
	push {%r1}
	ldr %r1, =I1
	ldr %r1, [%r1]
	push {%r1}
	pop {%r1, %r2}
	add %r0, %r1, %r2
	push {%r0}
	pop {%r0}
	ldr %r3, =I0
	str %r0, [%r3]
	@ Yayy! Stack has nothing left.
	@ You should be happy. I am.

	ldr %r0, =num_fmt
	ldr %r1, =I0
	ldr %r1, [%r1]
	bl printf
	ldr %r0, =S5
	bl printf

	@ Free local vars
	@ locals are free


	mov %r0, $0
	bl fflush
	mov %r7, $1
	swi $0
	.end
