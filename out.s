section .text
	global _start

_start:
	call    main
	mov     rdi, rax
	mov     rax, 60
	syscall

main:
	push    rbp
	mov     rbp, rsp
	sub     rsp, 8
	lea     r9, [rbp - 8]
	push    r9
	push    7
	pop     r9
	pop     r8
	mov     qword [r8], r9
	push    r8
	pop     r8
	mov     r8, qword [r8]
	push    r8
	add     rsp, 8
	mov     r9, qword [rbp - 8]
	push    r9
	mov     rax, [rsp]
	mov     rsp, rbp
	pop     rbp
	ret
	mov     rsp, rbp
	pop     rbp
	ret
