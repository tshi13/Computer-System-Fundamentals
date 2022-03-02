/* Hexdump main function, assembly language version */
	.section .rodata

	sColon: .string ": "
	sSingleSpace: .string " "
	sDoubleSpace: .string " "
	sNewLine: .string "\n"

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	subq $8, %rsp
	subq $128, %rsp 		//initialize 16 bytes of storage for char array to store user input
	movq %rsp, %rbp 		//%rbp is the array pointer of the char array
	movl $16, %r12d 		//initialize num_elements for loop condition
	movl $0, %r13d 			//initialize count for tracking the position

	.LprintLoop:
	movq %rbp, %rdi 		//move userinput[16] into parameter
	call hex_read 			//call hex read
	movq %rbp, %rdi 		//move userinput[16] user just input into parameter
	call hex_write_string	//Call hex_write_string

	cmpl $0, %eax			//if hex_read returns 0
	je .LloopDone			//ends loop
	cmpl $-1, %eax			//If hex_read returns -1
	je .LloopDone			//Also ends loop

	//Write the position to output
	subq $72, %rsp			//creating a position char array to print position
	movq %rsp, %r14			//use %r14 to store position char array 			
	movl %r13d, %edi		//pass count as first parameter
	movq %r14, %rsi			//pass position char array as second paramter
	call hex_format_offset	//Call hex_format_offset
	movq %r14, %rdi			//move position char array as first parameter
	call hex_write_string	//Write position
	movq $sColon, %rdi		//Move ": " as first parameter
	call hex_write_string 	//Write "": "

	//Write the hex number of each char to output
	movl $0, %r15d			//%r15d as the index tracker, i = 0
	jmp .LcheckWriteStop	//Jump to check condition
.LwriteHexNum:
	subq $24, %rsp			//Create a char array of size 3 bytes
	movq %rsp, %rbx			//use %rbx to store the char array
	movb (%rbp, %r15, 1), %dil	//TODO: NEED TO CHECK!
	movq %rbx, %rsi			//pass in %rbx - cur[3] as the second parameter
	call hex_format_byte_as_hex//Call hex_format_byte_as_hex
	movq %rbx, %rdi			//pass in %rbx - cur[3] as the first parameter
	call hex_write_string 	//Write current char
	movq $sSingleSpace, %rdi//Pass in " " as the first parameter
	call hex_write_string	//Write the space " "
	incl %r15d
	
.LcheckWriteStop:
	cmpl %r12d, %r15d		//If index i < num_elements, continue loop
	jl	.LwriteHexNum		//Jump to loop body

	movl %r12d, %r15d		//New loop index for writing the double space, i = num_elements

.LwriteDoubleSpace:
	movq $sDoubleSpace, %rdi//Pass double space "  " into argument
	call hex_write_string	//Write the doble space "  "
	incl %r15d

.LcheckWriteDoubleSpaceStop:
	cmpl $16, %r15d			//If index i < 16, continue loop
	jl	.LwriteDoubleSpace

//Write each char of the sentence to output
	movl %r12d, %eax
	incl %eax
	imul $8, %eax
	subq %eax, %rsp
	





	jmp .LprintLoop

	.LloopDone:
	addq $8, %rsp
	ret

/* vim:ft=gas:
 */
