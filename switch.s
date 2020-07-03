.section .text
.global switch_to_next
switch_to_next:
	push %ebp
	mov %esp,%ebp
	push %edi
	push %esi
	push %ebx
	push %edx
	push %ecx
	push %eax
	pushfl

	mov current,%eax
	mov %esp,8(%eax)
	mov 8(%ebp),%eax
	mov %eax,current
	mov 8(%eax),%esp

	popfl
	popl %eax
	popl %edx
	popl %ecx
	popl %ebx
	popl %esi
	popl %edi

	popl %ebp
	ret
