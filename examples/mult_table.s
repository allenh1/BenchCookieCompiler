/**
 *mult_table.s
 *
 * Generated by Bench Cookie Compiler!
 * Bench Cookie is... Experimental. Don't be mad.
 */
	.bss

	.lcomm locals, 400

	.data

S0:	.ascii "Let's make a multiplication table!\n\0"
S1:	.ascii "Who's table did you forget? \0"
S2:	.ascii " * \0"
S3:	.ascii " = \0"
S4:	.ascii "\n\0"
I0:	.word 0

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

	ldr %r0, =S1
	bl printf

	ldr %r0, =num_fmt
	ldr %r1, =I0
	bl scanf

	mov %r0, $4
	bl malloc
	str %r0, [%r9, #0]

	mov %r0, $4
	bl malloc
	str %r0, [%r9, #4]

	mov %r0, $4
	bl malloc
	str %r0, [%r9, #8]

	mov %r1, $0
	push {%r1}
	pop {%r0}
	mov %r3, %r9
	ldr %r3, [%r3, #0]
	str %r0, [%r3]
	@ Yayy! Stack has nothing left.
	@ You should be happy. I am.

	mov %r1, %r9
	ldr %r1, [%r9, #0]
	ldr %r1, [%r1]
	push {%r1}
	mov %r1, $12
	push {%r1}
	pop {%r1, %r2}
	mov %r0, $1
	cmp %r2, %r1
	ble NOSETZERO0
	mov %r0, $0
NOSETZERO0: @label used to set as true
	push {%r0}
	pop {%r0}
	mov %r3, %r9
	ldr %r3, [%r3, #4]
	str %r0, [%r3]
	@ Yayy! Stack has nothing left.
	@ You should be happy. I am.

FOR0:	mov %r8, %r9
	ldr %r8, [%r8, #4]
	ldr %r8, [%r8]
	cmp %r8, $0
	beq END_FOR0
	ldr %r1, =I0
	ldr %r1, [%r1]
	push {%r1}
	mov %r1, %r9
	ldr %r1, [%r9, #0]
	ldr %r1, [%r1]
	push {%r1}
	pop {%r1, %r2}
	mul %r0, %r1, %r2
	push {%r0}
	pop {%r0}
	mov %r3, %r9
	ldr %r3, [%r3, #8]
	str %r0, [%r3]
	@ Yayy! Stack has nothing left.
	@ You should be happy. I am.

	ldr %r0, =num_fmt
	ldr %r1, =I0
	ldr %r1, [%r1]
	bl printf
	ldr %r0, =S2
	bl printf

	ldr %r0, =num_fmt
	ldr %r1, =locals
	ldr %r1, [%r1, #0]
	ldr %r1, [%r1]
	bl printf
	ldr %r0, =S3
	bl printf

	ldr %r0, =num_fmt
	ldr %r1, =locals
	ldr %r1, [%r1, #8]
	ldr %r1, [%r1]
	bl printf
	ldr %r0, =S4
	bl printf

	mov %r1, %r9
	ldr %r1, [%r9, #0]
	ldr %r1, [%r1]
	push {%r1}
	mov %r1, $1
	push {%r1}
	pop {%r1, %r2}
	add %r0, %r1, %r2
	push {%r0}
	pop {%r0}
	mov %r3, %r9
	ldr %r3, [%r3, #0]
	str %r0, [%r3]
	@ Yayy! Stack has nothing left.
	@ You should be happy. I am.

	mov %r1, %r9
	ldr %r1, [%r9, #0]
	ldr %r1, [%r1]
	push {%r1}
	mov %r1, $12
	push {%r1}
	pop {%r1, %r2}
	mov %r0, $1
	cmp %r2, %r1
	ble NOSETZERO1
	mov %r0, $0
NOSETZERO1: @label used to set as true
	push {%r0}
	pop {%r0}
	mov %r3, %r9
	ldr %r3, [%r3, #4]
	str %r0, [%r3]
	@ Yayy! Stack has nothing left.
	@ You should be happy. I am.

b FOR0
END_FOR0:
	@ Free local vars
	ldr %r0, =locals
	ldr %r0, [%r0, #0]
	bl free

	ldr %r0, =locals
	ldr %r0, [%r0, #4]
	bl free

	ldr %r0, =locals
	ldr %r0, [%r0, #8]
	bl free

	@ locals are free


	mov %r0, $0
	bl fflush
	mov %r7, $1
	swi $0
	.end
