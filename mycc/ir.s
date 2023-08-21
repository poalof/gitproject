	.text
	.file	"main"
	.p2align	4, 0x90         # -- Begin function main
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movl	$1, i(%rip)
	jmp	.LBB0_1
	.p2align	4, 0x90
.LBB0_2:                                # %loop
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	$.str, %edi
	movl	$105, %esi
	xorl	%eax, %eax
	callq	printf
	incl	%ebx
	movl	%ebx, i(%rip)
.LBB0_1:                                # %cond
                                        # =>This Inner Loop Header: Depth=1
	movl	i(%rip), %ebx
	cmpl	$5, %ebx
	jle	.LBB0_2
# %bb.3:                                # %afterLoop
	movl	$4, i(%rip)
	movl	$1, i(%rip)
	cmpl	$0, i(%rip)
	jle	.LBB0_6
	.p2align	4, 0x90
.LBB0_5:                                # %loop2
                                        # =>This Inner Loop Header: Depth=1
	movl	i(%rip), %esi
	movl	$.str.1, %edi
	xorl	%eax, %eax
	callq	printf
	addl	$-2, i(%rip)
	cmpl	$0, i(%rip)
	jg	.LBB0_5
	.p2align	4, 0x90
.LBB0_6:                                # %loop8
                                        # =>This Inner Loop Header: Depth=1
	movl	$.str.2, %edi
	movl	$114, %esi
	xorl	%eax, %eax
	callq	printf
	cmpl	$0, i(%rip)
	jg	.LBB0_6
# %bb.7:                                # %afterLoop10
	popq	%rbx
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	i,@object               # @i
	.bss
	.globl	i
	.p2align	2
i:
	.long	0                       # 0x0
	.size	i, 4

	.type	.str,@object            # @.str
	.section	.rodata,"a",@progbits
	.globl	.str
.str:
	.asciz	"%c\n"
	.size	.str, 4

	.type	.str.1,@object          # @.str.1
	.globl	.str.1
.str.1:
	.asciz	"%d\n"
	.size	.str.1, 4

	.type	.str.2,@object          # @.str.2
	.globl	.str.2
.str.2:
	.asciz	"%c\n"
	.size	.str.2, 4


	.section	".note.GNU-stack","",@progbits
