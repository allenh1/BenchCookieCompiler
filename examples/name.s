/**
 *name.s
 *
 * Generated by Bench Cookie Compiler!
 * Bench Cookie is... Experimental. Don't be mad.
 */
	.bss

	.lcomm IS0, 100
	.lcomm IS1, 100
	.lcomm locals, 400

	.data

S0:	.ascii "Enter your first name: \0"
S1:	.ascii "Enter your last name: \0"
S2:	.ascii "Hello, \0"
S3:	.ascii " \0"
S4:	.ascii ".\n\0"

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

	ldr %r0, =string_fmt
	ldr %r1, =IS0
	bl scanf

	ldr %r0, =S1
	bl printf

	ldr %r0, =string_fmt
	ldr %r1, =IS1
	bl scanf

	ldr %r0, =S2
	bl printf

	ldr %r0, =string_fmt
	ldr %r1, =IS0
	bl printf
	ldr %r0, =S3
	bl printf

	ldr %r0, =string_fmt
	ldr %r1, =IS1
	bl printf
	ldr %r0, =S4
	bl printf

	@ Free local vars
	@ locals are free


	mov %r0, $0
	bl fflush
	mov %r7, $1
	swi $0
	.end
