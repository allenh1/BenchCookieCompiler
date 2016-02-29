	.bss
	.data
	.balign 4
msg:	.ascii "Hello, world!\n\0"

	.text

	.global main
	.global printf

msg_address:	.word msg

main:
	ldr %r0, msg_address
	bl printf

	mov %r7, $1
	swi $0
	.end
