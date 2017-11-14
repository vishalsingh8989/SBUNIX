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
.globl _isr129
.globl _isr130
.globl _isr131
.globl _isr132
.globl _isr133
.globl _isr134
.globl _isr135
.globl _isr136
.globl _isr137
.globl _isr138
.globl _isr139
.globl _isr140
.globl _isr141
.globl _isr142
.globl _isr143
.globl _isr144
.globl _isr145
.globl _isr146
.globl _isr147
.globl _isr148
.globl _isr149
.globl _isr150
.globl _isr151
.globl _isr152
.globl _isr153
.globl _isr154
.globl _isr155
.globl _isr156
.globl _isr157
.globl _isr158
.globl _isr159
.globl _isr160
.globl _isr161
.globl _isr162
.globl _isr163
.globl _isr164
.globl _isr165
.globl _isr166
.globl _isr167
.globl _isr168
.globl _isr169
.globl _isr170
.globl _isr171
.globl _isr172
.globl _isr173
.globl _isr174
.globl _isr175
.globl _isr176
.globl _isr177
.globl _isr178
.globl _isr179
.globl _isr180
.globl _isr181
.globl _isr182
.globl _isr183
.globl _isr184
.globl _isr185
.globl _isr186
.globl _isr187
.globl _isr188
.globl _isr189
.globl _isr190
.globl _isr191
.globl _isr192
.globl _isr193
.globl _isr194
.globl _isr195
.globl _isr196
.globl _isr197
.globl _isr198
.globl _isr199

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
	iretq

_isr128:
	cli
	pushq $128
	pushad
	movq %rsp, %rdi
	callq syscall_handler 
	popad
	addq $16, %rsp
	iretq

_isr129:
  cli
  pushq $129
  pushad
  movq %rsp, %rdi
  callq int_handler_129
  popad
  addq $16, %rsp
  iretq

_isr130:
  cli
  pushq $130
  pushad
  movq %rsp, %rdi
  callq int_handler_130
  popad
  addq $16, %rsp
  iretq

_isr131:
  cli
  pushq $131
  pushad
  movq %rsp, %rdi
  callq int_handler_131
  popad
  addq $16, %rsp
  iretq

_isr132:
  cli
  pushq $132
  pushad
  movq %rsp, %rdi
  callq int_handler_132
  popad
  addq $16, %rsp
  iretq

_isr133:
  cli
  pushq $133
  pushad
  movq %rsp, %rdi
  callq int_handler_133
  popad
  addq $16, %rsp
  iretq

_isr134:
  cli
  pushq $134
  pushad
  movq %rsp, %rdi
  callq int_handler_134
  popad
  addq $16, %rsp
  iretq

_isr135:
  cli
  pushq $135
  pushad
  movq %rsp, %rdi
  callq int_handler_135
  popad
  addq $16, %rsp
  iretq

_isr136:
  cli
  pushq $136
  pushad
  movq %rsp, %rdi
  callq int_handler_136
  popad
  addq $16, %rsp
  iretq

_isr137:
  cli
  pushq $137
  pushad
  movq %rsp, %rdi
  callq int_handler_137
  popad
  addq $16, %rsp
  iretq

_isr138:
  cli
  pushq $138
  pushad
  movq %rsp, %rdi
  callq int_handler_138
  popad
  addq $16, %rsp
  iretq

_isr139:
  cli
  pushq $139
  pushad
  movq %rsp, %rdi
  callq int_handler_139
  popad
  addq $16, %rsp
  iretq

_isr140:
  cli
  pushq $140
  pushad
  movq %rsp, %rdi
  callq int_handler_140
  popad
  addq $16, %rsp
  iretq

_isr141:
  cli
  pushq $141
  pushad
  movq %rsp, %rdi
  callq int_handler_141
  popad
  addq $16, %rsp
  iretq

_isr142:
  cli
  pushq $142
  pushad
  movq %rsp, %rdi
  callq int_handler_142
  popad
  addq $16, %rsp
  iretq

_isr143:
  cli
  pushq $143
  pushad
  movq %rsp, %rdi
  callq int_handler_143
  popad
  addq $16, %rsp
  iretq

_isr144:
  cli
  pushq $144
  pushad
  movq %rsp, %rdi
  callq int_handler_144
  popad
  addq $16, %rsp
  iretq

_isr145:
  cli
  pushq $145
  pushad
  movq %rsp, %rdi
  callq int_handler_145
  popad
  addq $16, %rsp
  iretq

_isr146:
  cli
  pushq $146
  pushad
  movq %rsp, %rdi
  callq int_handler_146
  popad
  addq $16, %rsp
  iretq

_isr147:
  cli
  pushq $147
  pushad
  movq %rsp, %rdi
  callq int_handler_147
  popad
  addq $16, %rsp
  iretq

_isr148:
  cli
  pushq $148
  pushad
  movq %rsp, %rdi
  callq int_handler_148
  popad
  addq $16, %rsp
  iretq

_isr149:
  cli
  pushq $149
  pushad
  movq %rsp, %rdi
  callq int_handler_149
  popad
  addq $16, %rsp
  iretq

_isr150:
  cli
  pushq $150
  pushad
  movq %rsp, %rdi
  callq int_handler_150
  popad
  addq $16, %rsp
  iretq

_isr151:
  cli
  pushq $151
  pushad
  movq %rsp, %rdi
  callq int_handler_151
  popad
  addq $16, %rsp
  iretq

_isr152:
  cli
  pushq $152
  pushad
  movq %rsp, %rdi
  callq int_handler_152
  popad
  addq $16, %rsp
  iretq

_isr153:
  cli
  pushq $153
  pushad
  movq %rsp, %rdi
  callq int_handler_153
  popad
  addq $16, %rsp
  iretq

_isr154:
  cli
  pushq $154
  pushad
  movq %rsp, %rdi
  callq int_handler_154
  popad
  addq $16, %rsp
  iretq

_isr155:
  cli
  pushq $155
  pushad
  movq %rsp, %rdi
  callq int_handler_155
  popad
  addq $16, %rsp
  iretq

_isr156:
  cli
  pushq $156
  pushad
  movq %rsp, %rdi
  callq int_handler_156
  popad
  addq $16, %rsp
  iretq

_isr157:
  cli
  pushq $157
  pushad
  movq %rsp, %rdi
  callq int_handler_157
  popad
  addq $16, %rsp
  iretq

_isr158:
  cli
  pushq $158
  pushad
  movq %rsp, %rdi
  callq int_handler_158
  popad
  addq $16, %rsp
  iretq

_isr159:
  cli
  pushq $159
  pushad
  movq %rsp, %rdi
  callq int_handler_159
  popad
  addq $16, %rsp
  iretq

_isr160:
  cli
  pushq $160
  pushad
  movq %rsp, %rdi
  callq int_handler_160
  popad
  addq $16, %rsp
  iretq

_isr161:
  cli
  pushq $161
  pushad
  movq %rsp, %rdi
  callq int_handler_161
  popad
  addq $16, %rsp
  iretq

_isr162:
  cli
  pushq $162
  pushad
  movq %rsp, %rdi
  callq int_handler_162
  popad
  addq $16, %rsp
  iretq

_isr163:
  cli
  pushq $163
  pushad
  movq %rsp, %rdi
  callq int_handler_163
  popad
  addq $16, %rsp
  iretq

_isr164:
  cli
  pushq $164
  pushad
  movq %rsp, %rdi
  callq int_handler_164
  popad
  addq $16, %rsp
  iretq

_isr165:
  cli
  pushq $165
  pushad
  movq %rsp, %rdi
  callq int_handler_165
  popad
  addq $16, %rsp
  iretq

_isr166:
  cli
  pushq $166
  pushad
  movq %rsp, %rdi
  callq int_handler_166
  popad
  addq $16, %rsp
  iretq

_isr167:
  cli
  pushq $167
  pushad
  movq %rsp, %rdi
  callq int_handler_167
  popad
  addq $16, %rsp
  iretq

_isr168:
  cli
  pushq $168
  pushad
  movq %rsp, %rdi
  callq int_handler_168
  popad
  addq $16, %rsp
  iretq

_isr169:
  cli
  pushq $169
  pushad
  movq %rsp, %rdi
  callq int_handler_169
  popad
  addq $16, %rsp
  iretq

_isr170:
  cli
  pushq $170
  pushad
  movq %rsp, %rdi
  callq int_handler_170
  popad
  addq $16, %rsp
  iretq

_isr171:
  cli
  pushq $171
  pushad
  movq %rsp, %rdi
  callq int_handler_171
  popad
  addq $16, %rsp
  iretq

_isr172:
  cli
  pushq $172
  pushad
  movq %rsp, %rdi
  callq int_handler_172
  popad
  addq $16, %rsp
  iretq

_isr173:
  cli
  pushq $173
  pushad
  movq %rsp, %rdi
  callq int_handler_173
  popad
  addq $16, %rsp
  iretq

_isr174:
  cli
  pushq $174
  pushad
  movq %rsp, %rdi
  callq int_handler_174
  popad
  addq $16, %rsp
  iretq

_isr175:
  cli
  pushq $175
  pushad
  movq %rsp, %rdi
  callq int_handler_175
  popad
  addq $16, %rsp
  iretq

_isr176:
  cli
  pushq $176
  pushad
  movq %rsp, %rdi
  callq int_handler_176
  popad
  addq $16, %rsp
  iretq

_isr177:
  cli
  pushq $177
  pushad
  movq %rsp, %rdi
  callq int_handler_177
  popad
  addq $16, %rsp
  iretq

_isr178:
  cli
  pushq $178
  pushad
  movq %rsp, %rdi
  callq int_handler_178
  popad
  addq $16, %rsp
  iretq

_isr179:
  cli
  pushq $179
  pushad
  movq %rsp, %rdi
  callq int_handler_179
  popad
  addq $16, %rsp
  iretq

_isr180:
  cli
  pushq $180
  pushad
  movq %rsp, %rdi
  callq int_handler_180
  popad
  addq $16, %rsp
  iretq

_isr181:
  cli
  pushq $181
  pushad
  movq %rsp, %rdi
  callq int_handler_181
  popad
  addq $16, %rsp
  iretq

_isr182:
  cli
  pushq $182
  pushad
  movq %rsp, %rdi
  callq int_handler_182
  popad
  addq $16, %rsp
  iretq

_isr183:
  cli
  pushq $183
  pushad
  movq %rsp, %rdi
  callq int_handler_183
  popad
  addq $16, %rsp
  iretq

_isr184:
  cli
  pushq $184
  pushad
  movq %rsp, %rdi
  callq int_handler_184
  popad
  addq $16, %rsp
  iretq

_isr185:
  cli
  pushq $185
  pushad
  movq %rsp, %rdi
  callq int_handler_185
  popad
  addq $16, %rsp
  iretq

_isr186:
  cli
  pushq $186
  pushad
  movq %rsp, %rdi
  callq int_handler_186
  popad
  addq $16, %rsp
  iretq

_isr187:
  cli
  pushq $187
  pushad
  movq %rsp, %rdi
  callq int_handler_187
  popad
  addq $16, %rsp
  iretq

_isr188:
  cli
  pushq $188
  pushad
  movq %rsp, %rdi
  callq int_handler_188
  popad
  addq $16, %rsp
  iretq

_isr189:
  cli
  pushq $189
  pushad
  movq %rsp, %rdi
  callq int_handler_189
  popad
  addq $16, %rsp
  iretq

_isr190:
  cli
  pushq $190
  pushad
  movq %rsp, %rdi
  callq int_handler_190
  popad
  addq $16, %rsp
  iretq

_isr191:
  cli
  pushq $191
  pushad
  movq %rsp, %rdi
  callq int_handler_191
  popad
  addq $16, %rsp
  iretq

_isr192:
  cli
  pushq $192
  pushad
  movq %rsp, %rdi
  callq int_handler_192
  popad
  addq $16, %rsp
  iretq

_isr193:
  cli
  pushq $193
  pushad
  movq %rsp, %rdi
  callq int_handler_193
  popad
  addq $16, %rsp
  iretq

_isr194:
  cli
  pushq $194
  pushad
  movq %rsp, %rdi
  callq int_handler_194
  popad
  addq $16, %rsp
  iretq

_isr195:
  cli
  pushq $195
  pushad
  movq %rsp, %rdi
  callq int_handler_195
  popad
  addq $16, %rsp
  iretq

_isr196:
  cli
  pushq $196
  pushad
  movq %rsp, %rdi
  callq int_handler_196
  popad
  addq $16, %rsp
  iretq

_isr197:
  cli
  pushq $197
  pushad
  movq %rsp, %rdi
  callq int_handler_197
  popad
  addq $16, %rsp
  iretq

_isr198:
  cli
  pushq $198
  pushad
  movq %rsp, %rdi
  callq int_handler_198
  popad
  addq $16, %rsp
  iretq

_isr199:
  cli
  pushq $199
  pushad
  movq %rsp, %rdi
  callq int_handler_199
  popad
  addq $16, %rsp
  iretq



_isrxx:
	cli
	pushad
	callq default_int_handler
	popad
	sti
	iretq
