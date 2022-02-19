/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */
	sMap: .string "0123456789abcdef"

	.section .text

	/* TODO: add functions here */
	
	.globl hex_read
hex_read:
		ret
	
	.globl hex_write_string
hex_write_string:
	ret

	.globl hex_format_offset
hex_format_offset:
	ret


	.globl hex_format_byte_as_hex
hex_format_byte_as_hex: 
	// byteval in %rdi, sbuf[] in %rsi, sMap in %rdx, hexchar in %rcx
	subq $8,%rsp //adjust stack pointer

	movq $sMap,%rdx //store string map in %rdx
	movq $0, 2(%rsi) //store null terminator in sbuf[2]

	andq $0xf,%rdi //and operation
	movq (%rdx, %rdi, 1), %rcx //store correct hexchar in %rcx
	movq %rcx, 1(%rsi) //put hex in sbuf[1]

	shrq $4,%rdi //shift byteval 4 places to the right
	andq $0xf,%rdi //and operation
	movq (%rdx, %rdi, 1),%rsi //put hex in sbuf[0]

	addq $8,%rsp //adjust stack pointer



 



	.globl hex_to_printable
hex_to_printable:
	subq $8,%rsp //adjust stack pointer
	cmpq $31,%rdi //%rdi stores byteval
	jg .Lvalid //when byteval is printable
	movq $46,%rax 
	addq $8,%rsp //adjust stack pointer
	ret

.Lvalid:  //when byteval is printable
	movq %rdi,%rax //move byteval to %rax
	addq $8,%rsp //adjust stack pointer
	ret

	
	
/* vim:ft=gas:
 */
