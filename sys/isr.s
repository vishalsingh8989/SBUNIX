.globl _isr0
.globl _isr1
.globl _isr2
.globl _isr3
.globl _isr4
.globl _isr5
.globl _isr6
.globl _isr7
.globl _isr8
.globl _isr9
.globl _isr10
.globl _isr11
.globl _isr12
.globl _isr13
.globl _isr14
.globl _isr16
.globl _isr17
.globl _isr18
.globl _isr19
.globl _isr20
.globl _isr32
.globl _isr33
.globl _isr128
.globl _isrxx
.globl kern_stack
.globl user_stack
.globl fork_return
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
.endm

.macro popad
	popq %r9
	popq %r8
	popq %rdi
	popq %rsi
	popq %rbp
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
.endm

_isr0:
	cli
	pushad
	callq div0_int_handler
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

_isr2:
	cli
	pushad
	callq nmi_int_handler
	popad
	sti
	iretq

_isr3:
	cli
	pushad
	callq breakpoint_excep_handler
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

_isr5:
	cli
	pushad
	callq range_exeed_excep_handler
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

_isr7:
	cli
	pushad
	callq no_device_excep_handler
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

_isr9:
	cli
	pushad
	callq coproc_seg_overrun_handler
	popad
	sti
	iretq

_isr10:
	cli
	pushad
	callq invalid_tss_excep_handler
	popad
	sti
	iretq

_isr11:
	cli
	pushad
	callq segment_not_present_handler
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

_isr13:
	cli
	pushad
	callq general_prot_fault_handler
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

_isr16:
	cli
	pushad
	callq fpu_error_handler
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

_isr18:
	cli
	pushad
	callq machine_check_handler
	popad
	sti
	iretq

_isr19:
	cli
	pushad
	callq simd_fpu_excep_handler
	popad
	sti
	iretq

_isr20:
	cli
	pushad
	callq virtualization_excep_handler
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

_isr128:
	//cli
	pushq $128
	movq %rsp, user_stack
	movq (kern_stack), %rsp
	pushq (user_stack)
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rbp
	pushq %rsi
	pushq %rdi
	pushq %r8
	pushq %r9
	movq %rsp, %rdi
	callq syscall_handler
	jmp sysret
fork_return:
	popq %r9
	popq %r8
	popq %rdi
	popq %rsi
	popq %rbp
	popq %rdx
	popq %rcx
	popq %rbx
	xorq %rax, %rax
	popq %r11
	popq %rsp
	addq $0x8, %rsp
  sti
	sysretq
sysret:
	popq %r9
	popq %r8
	popq %rdi
	popq %rsi
	popq %rbp
	popq %rdx
	popq %rcx
	popq %rbx
	popq %r11
	popq %rsp
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
