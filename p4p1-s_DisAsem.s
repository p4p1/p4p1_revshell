(__TEXT,__text) section

_main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$0x110, %rsp            ## imm = 0x110
	leaq	0xae8(%rip), %rax
	leaq	0xaf8(%rip), %rcx
	movq	0xde0(%rip), %rdx
	movq	(%rdx), %rdx
	movq	%rdx, -0x8(%rbp)
	movl	$0x0, -0x64(%rbp)
	movl	%edi, -0x68(%rbp)
	movq	%rsi, -0x70(%rbp)
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	0x100001bd2
	leaq	0xacc(%rip), %rcx
	movq	%rax, -0x78(%rbp)
	movq	%rcx, -0xa0(%rbp)
	cmpq	$0x0, -0x78(%rbp)
	jne	0x10000127f
	movl	$0x1159, -0x94(%rbp)    ## imm = 0x1159
	jmp	0x1000012f9
	movl	$0x0, -0xb0(%rbp)
	movq	-0x78(%rbp), %rdi
	callq	0x100001bc6
	movb	%al, %cl
	movb	%cl, -0xa9(%rbp)
	movsbl	%cl, %eax
	cmpl	$0xffffffff, %eax       ## imm = 0xFFFFFFFF
	je	0x1000012d0
	movb	-0xa9(%rbp), %al
	movslq	-0xb0(%rbp), %rcx
	movb	%al, -0x60(%rbp,%rcx)
	movl	-0xb0(%rbp), %edx
	addl	$0x1, %edx
	movl	%edx, -0xb0(%rbp)
	jmp	0x100001289
	leaq	-0x60(%rbp), %rdi
	callq	0x100001bba
	leaq	0xa45(%rip), %rdi
	movl	%eax -0x94(%rbp)
	movl	-0x94(%rbp), %esi
	movb	$0x0, %al
	callq	0x100001bfc
	movl	%eax, -0xbc(%rbp)
	movb	$0x2, -0x17(%rbp)
	movl	$0x0, -0x14(%rbp)
	movl	-0x94(%rbp), %eax
	movw	%ax, %cx
	movzwl	%cx, %edi
	callq	0x1000015f0
	leaq	0xa1d(%rip), %rdi
	movzwl	%ax, %edx
	movw	%dx, %ax
	movw	%ax, -0x16(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	leaq	0xa20(%rip), %rdi
	movl	%eax, -0xc0(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	movl	-0x14(%rbp), %edi
	movl	%eax, -0xc4(%rbp)
	callq	0x100001bde
	leaq	0xa13(%rip), %rdi
	movl	-0x94(%rbp), %edx
	movq	%rax, %rsi
	movb	$0x0, %al
	callq	0x100001bfc
	movl	$0x2, %edi
	movl	$0x1, %esi
	xorl	%edx, %edx
	movl	%eax, -0xc8(%rbp)
	callq	0x100001c14
	movl	%eax, -0x7c(%rbp)
	cmpl	$0xffffffff, -0x7c(%rbp) ## imm = 0xFFFFFFFF
	jne	0x1000013a1
	leaq	0x9ed(%rip), %rdi
	movb	$0x0, %al
	callq	0x100001bfc
	movl	%eax, -0xcc(%rbp)
	movl	$0x10, %edx
	leaq	-0x18(%rbp), %rax
	movl	-0x7c(%rbp), %edi
	movq	%rax, %rsi
	callq	0x100001bc0
	cmpl	$0x0, %eax
	jge	0x1000013de
	leaq	0x9d4(%rip), %rdi
	callq	0x100001c08
	movl	$0x1, -0x64(%rbp)
	movl	%eax, -0xd0(%rbp)
	jmp	0x1000015b5
	movl	$0x3, %esi
	movl	-0x7c(%rbp), %edi
	callq	0x100001be4
	leaq	0x9b6(%rip), %rdi
	movl	-0x94(%rbp), %esi
	movl	%eax, -0xd4(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	movl	$0x10, -0x84(%rbp)
	movl	%eax, -0xd8(%rbp)
	leaq	-0x84(%rbp), %rdx
	leaq	-0x28(%rbp), %rax
	movl	-0x7c(%rbp), %edi
	movq	%rax, %rsi
	callq	0x100001bb4
	movl	%eax, -0x80(%rbp)
	cmpl	$0x0, %eax
	je	0x100001589
	movl	-0x24(%rbp), %edi
	callq	0x100001bde
	movq	%rax, -0xa8(%rbp)
	cmpq	$0x0, -0xa8(%rbp)
	jne	0x100001465
	leaq	0x979(%rip), %rdi
	callq	0x100001bf6
	leaq	0x982(%rip), %rdi
	movq	-0xa8(%rbp), %rsi
	movl	-0x94(%rbp), %edx
	movb	$0x0, %al
	callq	0x100001bfc
	leaq	0x98d(%rip), %rdi
	movl	%eax, -0xdc(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	movl	-0x80(%rbp), %edi
	movq	-0xa0(%rbp), %rsi
	movq	-0xa0(%rbp), %rcx
	movl	%edi, -0xe0(%rbp)
	movq	%rcx, %rdi
	movl	%eax, -0xe4(%rbp)
	movq	%rsi, -0xf0(%rbp)
	callq	0x100001c1a
	movl	-0xe0(%rbp), %edi
	movq	-0xf0(%rbp), %rsi
	movq	%rax, %rdx
	callq	0x100001c20
	leaq	0x950(%rip), %rdi
	movq	-0xa0(%rbp), %rsi
	movq	%rax, -0xf8(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	movl	$0x1, %r8d
	movl	%r8d, %edi
	movl	%eax, -0xfc(%rbp)
	callq	0x100001bea
	leaq	-0xb8(%rbp), %rdi
	xorl	%r8d, %r8d
	movl	%r8d, %esi
	leaq	0x107(%rip), %rdx
	movq	%rax, -0x90(%rbp)
	movl	-0x80(%rbp), %r8d
	movq	-0x90(%rbp), %rax
	movl	%r8d, (%rax)
	movq	-0x90(%rbp), %rax
	movq	%rax, %rcx
	callq	0x100001c02
	cmpl	$0x0, %eax
	jge	0x100001560
	leaq	0x8fd(%rip), %rdi
	callq	0x100001bf6
	movl	$0x1, -0x64(%rbp)
	jmp	0x1000015b5
	leaq	0x8fe(%rip), %rdi
	movq	-0xa8(%rbp), %rsi
	movl	-0x94(%rbp), %edx
	movl	-0x80(%rbp), %ecx
	movb	$0x0, %al
	callq	0x100001bfc
	movl	%eax, -0x100(%rbp)
	jmp	0x100001415
	cmpl	$0x0, -0x80(%rbp)
	jge	0x1000015ae
	leaq	0x8f6(%rip), %rdi
	callq	0x100001bf6
	movl	$0x1, -0x64(%rbp)
	jmp	0x1000015b5
	movl	$0x0, -0x64(%rbp)
	movq	0xa54(%rip), %rax
	movl	-0x64(%rbp), %ecx
	movq	(%rax), %rax
	cmpq	-0x8(%rbp), %rax
	movl	%ecx, -0x104(%rbp)
	jne	0x1000015e1
	movl	-0x104(%rbp), %eax
	addq	$0x110, %rsp            ## imm = 0x110
	popq	%rbp
	retq
	callq	0x100001ba8
	nopw	%cs:(%rax,%rax)

__OSSwapInt16:
	pushq	%rbp
	movq	%rsp, %rbp
	movw	%di, %ax
	movw	%ax, -0x2(%rbp)
	movzwl	-0x2(%rbp), %edi
	shll	$0x8, %edi
	movzwl	-0x2(%rbp), %ecx
	sarl	$0x8, %ecx
	orl	%ecx, %edi
	movw	%di, %ax
	movzwl	%ax, %eax
	popq	%rbp
	retq
	nopw	%cs:(%rax,%rax)

_connection_handler:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$0x27f0, %rsp           ## imm = 0x27F0
	leaq	0x86f(%rip), %rax
	movq	0x9d7(%rip), %rcx
	movq	(%rcx), %rcx
	movq	%rcx, -0x8(%rbp)
	movq	%rdi, -0x2728(%rbp)
	movq	%rax, %rdi
	movb	$0x0, %al
	callq	0x100001bfc
	leaq	0x864(%rip), %rdi
	movl	%eax, -0x273c(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	leaq	0x864(%rip), %rdi
	movl	%eax, -0x2740(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	leaq	0x865(%rip), %rdi
	movl	%eax, -0x2744(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	leaq	0x866(%rip), %rdi
	movl	%eax, -0x2748(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	leaq	0x867(%rip), %rdi
	movl	%eax, -0x274c(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	leaq	0x863(%rip), %rsi
	movl	$0x270f, %edx           ## imm = 0x270F
	leaq	-0x2720(%rbp), %rdi
	movq	-0x2728(%rbp), %rcx
	movl	(%rcx), %r8d
	movl	%r8d, -0x272c(%rbp)
	movl	%eax, -0x2750(%rbp)
	callq	0x100001bae
	leaq	0x835(%rip), %rdi
	movl	-0x272c(%rbp), %esi
	movq	%rax, -0x2758(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	movl	%eax, -0x275c(%rbp)
	leaq	0x827(%rip), %rdi
	movl	-0x272c(%rbp), %esi
	movb	$0x0, %al
	callq	0x100001bfc
	movl	$0x270f, %esi           ## imm = 0x270F
	movq	0x8f3(%rip), %rdi
	leaq	-0x2720(%rbp), %rcx
	movq	(%rdi), %rdx
	movq	%rcx, %rdi
	movl	%eax, -0x2760(%rbp)
	callq	0x100001bcc
	movsbl	-0x2720(%rbp), %esi
	cmpl	$0x21, %esi
	movq	%rax, -0x2768(%rbp)
	jne	0x100001890
	leaq	0x7e4(%rip), %rdi
	movb	$0x0, %al
	callq	0x100001bfc
	movl	$0x5, %esi
	movq	0x8a7(%rip), %rdi
	leaq	-0x2731(%rbp), %rcx
	movq	(%rdi), %rdx
	movq	%rcx, %rdi
	movl	%eax, -0x276c(%rbp)
	callq	0x100001bcc
	leaq	-0x2731(%rbp), %rdi
	movq	%rax, -0x2778(%rbp)
	callq	0x100001bba
	leaq	0x7c2(%rip), %rsi
	movl	$0x270f, %r8d           ## imm = 0x270F
	movl	%r8d, %edx
	leaq	-0x2720(%rbp), %rdi
	movl	%eax, -0x2738(%rbp)
	callq	0x100001bae
	movl	$0x270f, %r8d           ## imm = 0x270F
	movl	%r8d, %edx
	leaq	-0x2720(%rbp), %rsi
	movl	-0x2738(%rbp), %edi
	movq	%rax, -0x2780(%rbp)
	callq	0x100001c20
	movl	$0x270f, %edi           ## imm = 0x270F
	movl	%edi, %ecx
	leaq	-0x2720(%rbp), %rdx
	xorl	%esi, %esi
	movq	%rdx, %r9
	movq	%r9, %rdi
	movq	%rdx, -0x2788(%rbp)
	movq	%rcx, %rdx
	movq	%rax, -0x2790(%rbp)
	movq	%rcx, -0x2798(%rbp)
	callq	0x100001bf0
	movl	-0x2738(%rbp), %edi
	movq	-0x2788(%rbp), %rsi
	movq	-0x2798(%rbp), %rdx
	callq	0x100001c0e
	leaq	0x734(%rip), %rdi
	leaq	-0x2720(%rbp), %rsi
	movq	%rax, -0x27a0(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	leaq	-0x2720(%rbp), %rdi
	movl	%eax, -0x27a4(%rbp)
	callq	0x100001c1a
	cmpq	$0x0, %rax
	jbe	0x100001877
	movl	-0x2738(%rbp), %eax
	movl	%eax, -0x272c(%rbp)
	jmp	0x10000188b
	leaq	0x6fa(%rip), %rdi
	movb	$0x0, %al
	callq	0x100001bfc
	movl	%eax, -0x27a8(%rbp)
	jmp	0x10000199d
	movsbl	-0x2720(%rbp), %eax
	cmpl	$0x5f, %eax
	jne	0x1000018a7
	jmp	0x100001998
	movsbl	-0x2720(%rbp), %eax
	cmpl	$0x26, %eax
	jne	0x1000018ff
	leaq	0x6d0(%rip), %rsi
	movl	$0x270f, %eax           ## imm = 0x270F
	movl	%eax, %edx
	leaq	-0x2720(%rbp), %rdi
	callq	0x100001bae
	movl	$0x270f, %ecx           ## imm = 0x270F
	movl	%ecx, %edx
	leaq	-0x2720(%rbp), %rsi
	movl	-0x272c(%rbp), %edi
	movq	%rax, -0x27b0(%rbp)
	callq	0x100001c20
	movq	%rax, -0x27b8(%rbp)
	jmp	0x1000019a2
	movl	$0x270f, %eax           ## imm = 0x270F
	movl	%eax, %edx
	leaq	-0x2720(%rbp), %rsi
	movl	-0x272c(%rbp), %edi
	callq	0x100001c20
	movl	$0x270f, %edi           ## imm = 0x270F
	movl	%edi, %edx
	leaq	-0x2720(%rbp), %rsi
	xorl	%edi, %edi
	movq	%rsi, %rcx
	movl	%edi, -0x27bc(%rbp)
	movq	%rcx, %rdi
	movl	-0x27bc(%rbp), %r8d
	movq	%rsi, -0x27c8(%rbp)
	movl	%r8d, %esi
	movq	%rdx, -0x27d0(%rbp)
	movq	%rax, -0x27d8(%rbp)
	callq	0x100001bf0
	movl	-0x272c(%rbp), %edi
	movq	-0x27c8(%rbp), %rsi
	movq	-0x27d0(%rbp), %rdx
	callq	0x100001c0e
	leaq	0x61b(%rip), %rdi
	leaq	-0x2720(%rbp), %rsi
	movq	%rax, -0x27e0(%rbp)
	movb	$0x0, %al
	callq	0x100001bfc
	movl	%eax, -0x27e4(%rbp)
	jmp	0x100001998
	jmp	0x10000199d
	jmp	0x100001705
	movq	-0x2728(%rbp), %rdi
	callq	0x100001bd8
	movq	0x65b(%rip), %rdi
	movq	(%rdi), %rdi
	cmpq	-0x8(%rbp), %rdi
	jne	0x1000019cd
	xorl	%eax, %eax
	addq	$0x27f0, %rsp           ## imm = 0x27F0
	popq	%rbp
	retq
	callq	0x100001ba8
	nopw	%cs:(%rax,%rax)
	
_int_to_str:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$0x60, %rsp
	movq	0x621(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -0x8(%rbp)
	movl	%edi, -0x28(%rbp)
	movq	%rsi, -0x30(%rbp)
	movl	%edx, -0x34(%rbp)
	movl	$0x0, -0x38(%rbp)
	cmpq	$0x0, -0x30(%rbp)
	jne	0x100001a21
	movl	$0xffffffff, -0x24(%rbp) ## imm = 0xFFFFFFFF
	jmp	0x100001b7f
	movl	$0x0, -0x3c(%rbp)
	cmpl	$0x0, -0x28(%rbp)
	jge	0x100001a6f
	movl	-0x38(%rbp), %eax
	movl	-0x34(%rbp), %ecx
	subl	$0x1, %ecx
	cmpl	%ecx, %eax
	jae	0x100001a6a
	movq	-0x30(%rbp), %rax
	movq	%rax, %rcx
	addq	$0x1, %rcx
	movq	%rcx, -0x30(%rbp)
	movb	$0x2d, (%rax)
	movl	-0x38(%rbp), %edx
	addl	$0x1, %edx
	movl	%edx, -0x38(%rbp)
	jmp	0x100001a6f
	leaq	-0x20(%rbp), %rax
	addq	$0x10, %rax
	addq	$-0x1, %rax
	movq	%rax, -0x48(%rbp)
	movl	$0xa, %eax
	movl	-0x28(%rbp), %ecx
	movl	%eax, -0x50(%rbp)
	movl	%ecx, %eax
	cltd
	movl	-0x50(%rbp), %ecx
	idivl	%ecx
	movl	%edx, -0x4c(%rbp)
	cmpl	$0x0, -0x4c(%rbp)
	jge	0x100001ab3
	xorl	%eax, %eax
	subl	-0x4c(%rbp), %eax
	movl	%eax, -0x54(%rbp)
	jmp	0x100001ab9
	movl	-0x4c(%rbp), %eax
	movl	%eax, -0x54(%rbp)
	movl	-0x54(%rbp), %eax
	movl	$0xa, %ecx
	movl	%eax, -0x4c(%rbp)
	movl	-0x4c(%rbp), %eax
	addl	$0x30, %eax
	movb	%al, %dl
	movq	-0x48(%rbp), %rsi
	movq	%rsi, %rdi
	addq	$-0x1, %rdi
	movq	%rdi, -0x48(%rbp)
	movb	%dl, (%rsi)
	movl	-0x28(%rbp), %eax
	cltd
	idivl	%ecx
	movl	%eax, -0x28(%rbp)
	movl	-0x3c(%rbp), %eax
	addl	$0x1, %eax
	movl	%eax, -0x3c(%rbp)
	cmpl	$0x0, -0x28(%rbp)
	jne	0x100001a83
	jmp	0x100001b08
	cmpl	$0x0, -0x3c(%rbp)
	jle	0x100001b71
	movl	-0x38(%rbp), %eax
	movl	-0x34(%rbp), %ecx
	subl	$0x1, %ecx
	cmpl	%ecx, %eax
	jae	0x100001b5c
	movl	$0x10, %eax
	movl	%eax, %ecx
	movslq	-0x3c(%rbp), %rdx
	subq	%rdx, %rcx
	movb	-0x20(%rbp,%rcx), %sil
	movq	-0x30(%rbp), %rcx
	movq	%rcx, %rdx
	addq	$0x1, %rdx
	movq	%rdx, -0x30(%rbp)
	movb	%sil, (%rcx)
	movl	-0x38(%rbp), %eax
	addl	$0x1, %eax
	movl	%eax, -0x38(%rbp)
	jmp	0x100001b61
	movl	-0x3c(%rbp), %eax
	addl	$0xffffffff, %eax       ## imm = 0xFFFFFFFF
	movl	%eax, -0x3c(%rbp)
	jmp	0x100001b08
	movq	-0x30(%rbp), %rax
	movb	$0x0, (%rax)
	movl	$0x0, -0x24(%rbp)
	movq	0x48a(%rip), %rax
	movl	-0x24(%rbp), %ecx
	movq	(%rax), %rax
	cmpq	-0x8(%rbp), %rax
	movl	%ecx, -0x58(%rbp)
	jne	0x100001ba2
	movl	-0x58(%rbp), %eax
	addq	$0x60, %rsp
	popq	%rbp
	retq
	callq	0x100001ba8
