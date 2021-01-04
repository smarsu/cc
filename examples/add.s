	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15, 6
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3               ## -- Begin function add
LCPI0_0:
	.quad	4599075939470750515     ## double 0.29999999999999999
LCPI0_1:
	.quad	4604480259023595110     ## double 0.69999999999999996
LCPI0_2:
	.quad	4602678819172646912     ## double 0.5
LCPI0_3:
	.quad	4607182418800017408     ## double 1
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_add
	.p2align	4, 0x90
_add:                                   ## @add
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movsd	LCPI0_0(%rip), %xmm2    ## xmm2 = mem[0],zero
	movsd	%xmm0, -16(%rbp)
	movsd	%xmm1, -24(%rbp)
	movsd	-16(%rbp), %xmm0        ## xmm0 = mem[0],zero
	mulsd	-24(%rbp), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	LBB0_2
## %bb.1:
	movsd	LCPI0_2(%rip), %xmm0    ## xmm0 = mem[0],zero
	movsd	%xmm0, -8(%rbp)
	jmp	LBB0_5
LBB0_2:
	movsd	LCPI0_1(%rip), %xmm0    ## xmm0 = mem[0],zero
	movsd	-16(%rbp), %xmm1        ## xmm1 = mem[0],zero
	mulsd	-24(%rbp), %xmm0
	ucomisd	%xmm1, %xmm0
	jbe	LBB0_4
## %bb.3:
	movsd	LCPI0_3(%rip), %xmm0    ## xmm0 = mem[0],zero
	movsd	%xmm0, -8(%rbp)
	jmp	LBB0_5
LBB0_4:
	movsd	LCPI0_2(%rip), %xmm0    ## xmm0 = mem[0],zero
	movsd	%xmm0, -8(%rbp)
LBB0_5:
	movsd	-8(%rbp), %xmm0         ## xmm0 = mem[0],zero
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
