/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

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
	ret






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
