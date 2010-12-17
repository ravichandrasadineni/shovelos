
.code16gcc

################################################################
# memcpy(dst,src,size)
#   1) dst ( 20bit address ) ( 0x00000 -> 0xfffff )
#   2) src ( 20bit address ) ( 0x00000 -> 0xfffff )
#   3) size in bytes
######
.global memcpy
		memcpy:
		pushl   %edi
		pushl   %esi
		pushl   %ds
		pushl   %es
									 # setup destination (es:edi)
		movl 20(%esp),    %edi       # edi = dst
		andl $0x0ffff,    %edi       # edi &= 0xffff
		andl $0xf0000,    20(%esp)   # dst  &= 0xf0000
		shrl $4,          20(%esp)   # dst  >>= 4;
		movl 20(%esp),    %ecx       # es = dst
		movl %ecx,        %es        #

			                         # setup source (ds::esi)
		movl 24(%esp),    %esi       # esi = src
		andl $0x0ffff,    %esi       # esi &= 0xffff
		andl $0xf0000,    24(%esp)   # src  &= 0xf0000
		shrl $4,          24(%esp)   # src  >>= 4;
		movl 24(%esp),    %ecx       # ds = src
		movl %ecx,        %ds

		xorl %ecx,         %ecx
    .loop:
		cmpl   %ecx,    28(%esp)     	# increment counter and test for exit
		je    .eloop
		incl   %ecx

	    ds movb (%esi),     %al      	# copy byte
		es movb    %al,   (%edi)

										# INCREMENT DST seg:offset
		incl %edi
		cmpl $0x10000, %edi
		jne .done_edi
		movl %es, %edx
		addl $0x1000,   %edx
		movl %edx,     %es
		xorl %edi,     %edi
	.done_edi:

										# INCREMENT SRC seg:offset
		incl %esi
		cmpl $0x10000, %esi
		jne .done_esi
		movl %ds, %edx
	    addl $0x1000,   %edx
		movl %edx,     %ds
		xorl %esi,     %esi
	.done_esi:

		jmp .loop
	.eloop:

		popl    %es
		popl    %ds
		popl    %esi
		popl    %edi
		ret


################################################################
# memcpy(dst,fill,size)
#   1) dst ( 20bit address ) ( 0x00000 -> 0xfffff )
#   2) fill byte
#   3) size in bytes
######
	.global memset
			memset:
		pushl   %edi
		pushl   %es
									 # setup destination (es:edi)
		movl 12(%esp),    %edi       # edi = dst
		andl $0x0ffff,    %edi       # edi &= 0xffff
		andl $0xf0000,    12(%esp)   # dst  &= 0xf0000
		shrl $4,          12(%esp)   # dst  >>= 4;
		movl 12(%esp),    %ecx       # es = dst
		movl %ecx,        %es

		xorl %ecx,         %ecx
    .loop2:
	    cmpl   %ecx,    20(%esp)     	# increment counter and test for exit
		je    .eloop2
		incl   %ecx

		   movb 16(%esp),   %al      	# copy byte
		es movb    %al,   (%edi)

								        # INCREMENT DST seg:offset
		incl %edi
		cmpl $0x10000, %edi
		jne .loop2
		movl %es, %edx
		addl $0x1000,   %edx
		movl %edx,     %es
		xorl %edi,     %edi
		jmp .loop2
	.eloop2:
		popl    %es
		popl    %edi
		ret



	.global strcmp
			strcmp:

			xorl %ecx, %ecx
			xorl %eax, %eax

._286_strcmp_loop:

			movb 4(%esp,%ecx,1), %ah
			movb 8(%esp,%ecx,1), %al

			cmpb %al,%ah                  # different? return non-zero
			jnz  ._286_strcmp_ret

			cmpw $0, %ax                  # both zero? return zero
			jz   ._286_strcmp_ret

			inc %ecx                      # same, not terminal, continue
			jmp  ._286_strcmp_loop

._286_strcmp_ret:
			ret



