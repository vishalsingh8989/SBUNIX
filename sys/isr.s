.globl _isr0
.globl _isr1
.globl _isr4
.globl _isr6
.globl _isr8
.globl _isr12
.globl _isr14
.globl _isr17
.globl _isr32
.globl _isr33
.globl _isr128
.globl _isrxx
.align 4

.macro pushad
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rbp
	pushq %rsi
	pushq %rdi
	pushq %r8
	pushq %r9
	//pushq %r10
	//pushq %r11
	//pushq %r12
	//pushq %r13
	//pushq %r14
	//pushq %r15
	//movq %rsp, %rdi
.endm

.macro popad
	//popq %r15
	//popq %r14
	//popq %r13
	//popq %r12
	//popq %r11
	//popq %r10
	popq %r9
	popq %r8
	popq %rdi
	popq %rsi
	popq %rbp
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	//addq $16, %rsp
.endm

_isr0:
	cli
	pushad
	callq div0_int_handler
	popad
	sti
	iretq

_isr32:
	cli
	pushad
	callq timer_int_handler
	popad
	sti
	iretq

_isr33:
	cli
	pushad
	callq keyboard_int_handler
	popad
	sti
	iretq

_isr1:
	cli
	pushad
	callq debug_excep_handler 
	popad
	sti
	iretq

_isr4:
	cli
	pushad
	callq overflow_handler 
	popad
	sti
	iretq

_isr6:
	cli
	pushad
	callq invalid_opcode_handler 
	popad
	sti
	iretq

_isr8:
	cli
	pushad
	callq double_fault_handler 
	popad
	sti
	iretq

_isr12:
	cli
	pushad
	callq stack_fault_handler 
	popad
	sti
	iretq

_isr17:
	cli
	pushad
	callq alignment_check_handler 
	popad
	sti
	iretq

_isr14:
	cli
	pushq $14
	pushad
	movq %rsp, %rdi
	callq page_fault_handler 
	popad
	addq $16, %rsp
	//sti
	iretq

_isr128:
	cli
	pushq $128
	pushad
	movq %rsp, %rdi
	callq syscall_handler 
	popad
	addq $0x8, %rsp
    sti
	sysretq

_isrxx:
	cli
	pushad
	callq default_int_handler
	popad
	sti
	iretq
