# arch/i386/setjmp.S
#
# setjmp/longjmp for the i386 architecture
#
#
# The jmp_buf is assumed to contain the following, in order:
#       %ebx
#       %esp
#       %ebp
#       %esi
#       %edi
#       <return address>
#
		.text
		.align 4
		.globl _ITM_beginTransaction
		.type _ITM_beginTransaction, @function
_ITM_beginTransaction:
        popl %ecx                       # pop return address and adjust the stack
        movl %ebx,(%eax)
        movl %esp,4(%eax)               # post-return %esp
		pushl %ecx
        movl %ebp,8(%eax)
        movl %esi,12(%eax)
        movl %edi,16(%eax)
		movl %ecx,20(%eax)
		call _inner_beginTransaction
        ret

        .size _ITM_beginTransaction,.-_ITM_beginTransaction


        .text
        .align 4
        .globl jmp_to_begin_transaction
        .type jmp_to_begin_transaction, @function
jmp_to_begin_transaction:
#		movl %eax,%edx
		pushl %eax
		call _inner_jmpReturn
		popl %edx
        movl (%edx),%ebx
        movl 4(%edx),%esp
        movl 8(%edx),%ebp
        movl 12(%edx),%esi
        movl 16(%edx),%edi
        jmp *20(%edx)

        .size jmp_to_begin_transaction,.-jmp_to_begin_transaction
