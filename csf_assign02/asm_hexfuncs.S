/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */
	sMap: .string "0123456789abcdef"

	.section .text

	/* TODO: add functions here */
	
	.globl hex_read
hex_read:
	addq $8, %rsp //adjust stack pointer
	movq %rdi, %r10 //move data_buf into resgiter
	movq $0, %rdi //pass in 0 to read function call
	movq %r10, %rsi //pass in data_buf to read function call
	movq $16, %rdx //paass in 16 to read function call
	call read
	subq $8, %rsp //restore stack pointer
		ret

	.globl hex_write_string
hex_write_string:
	addq $8, %rsp //adjust stack pointer
	movq $16, %r10 //length = 16
	movq $0, %r11 // %r11 stores the value of i, int i = 0
	jmp .LcheckLoop //Go to loop
	
.LupdateLength:
	movq %r11, %r10 //put the value of loop index into length
	jmp .Lwrite //break from loop body

.LfindNull:
	movq (%rdi, %r11, 1), %rcx //Access char stored in s[i], store it into %rcx
	addq $1, %r11 //update the loop index, i++
	cmpq $0, %rcx  //Compare char with 0
	je .LupdateLength //Jump to update length if char == null terminator

.LcheckLoop:
	cmpq $16, %r11 //Compare if i < 16
	jl .LfindNull //Jump to loop body if it does

//%r10 now stores the value of length
.Lwrite:
	movq %rdi, %rsi //pass char s[] as the second element into write
	movq $1, %rdi //pass 1 into as the first element into write
	movq %r10, %rdx //pass length as the third element of write
	call write
	subq $8, %rsp //restore stack pointer

	.globl hex_format_offset
hex_format_offset:
	ret


	.globl hex_format_byte_as_hex
hex_format_byte_as_hex: 
	// byteval in %rdi, sbuf[] in %rsi, sMap in %rdx, hexchar in %rcx, copy of byteval in %r8
	subq $8,%rsp //adjust stack pointer

	movq $sMap,%rdx //store string map in %rdx
	movq $0, 2(%rsi) //store null terminator in sbuf[2]

	movq %rdi,%r8 //copy byteval into %r8
	andq $0xf,%r8 //and operation
	movq (%rdx, %r8, 1), %rcx //store correct hexchar in %rcx
	movb %cl, 1(%rsi) //put hex in sbuf[1]

	shrq $4,%rdi //shift byteval 4 places to the right
	andq $0xf,%rdi //and operation

	movq (%rdx, %rdi, 1), %rcx //store correct hexchar in %rcx
	movb %cl,(%rsi) //put hex in sbuf[0]

	addq $8,%rsp //adjust stack pointer



 



	.globl hex_to_printable
hex_to_printable:
	subq $8,%rsp //adjust stack pointer
	cmpq $31,%rdi //%rdi stores byteval
	jg .Lvalid1 //when byteval > 31
	movq $46,%rax //move "." to %rax
	addq $8,%rsp //adjust stack pointer
	ret

.Lvalid1:  //check if byteval > 31
	cmpq $127,%rdi //check if byteval < 127
  jl .Lvalid2 //jump to check
  movq $46,%rax //move "." to %rax
	addq $8,%rsp //adjust stack pointer
	ret

.Lvalid2: //if 31< byteval < 127 
  movq %rdi,%rax //move byteval to %rax
	addq $8,%rsp //adjust stack pointer
	ret




	
/* vim:ft=gas:
 */
