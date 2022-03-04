/* Hexdump main function, assembly language version */
	.section .rodata

sColon: .string ": "
sSingleSpace: .string " "
sDoubleSpace: .string "   "
sNewLine: .string "\n"

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	subq $8, %rsp			  //Adjust the stack pointer
	subq $128, %rsp 		//initialize 16 bytes of storage for char array to store user input
	movq %rsp, %rbp 		//%rbp is the array pointer of the char array
	movl $16, %r12d 		//initialize num_elements for loop condition
	movl $0, %r13d 			//initialize count for tracking the position

.LprintLoop:
	movq %rbp, %rdi 		//move userinput[16] into parameter
	call hex_read 			//call hex read
	movl %eax, %r12d		//putting the result return from read to num_elements

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
	call hex_write_string 	//Write ": "

	//Write the hex number of each char to output
	movl $0, %r15d			//%r15d as the index tracker, i = 0
	jmp .LcheckWriteStop	//Jump to check condition

.LwriteHexNum:
	subq $24, %rsp			//Create a char array of size 3 bytes
	movq %rsp, %rbx			//use %rbx to store the char array
	movb (%rbp, %r15, 1), %dil//Move the char into as the fist parameter
	movq %rbx, %rsi			//pass in %rbx - cur[3] as the second parameter
	call hex_format_byte_as_hex//Call hex_format_byte_as_hex
	movq %rbx, %rdi			//pass in %rbx - cur[3] as the first parameter
	call hex_write_string 	//Write current char
	movq $sSingleSpace, %rdi//Pass in " " as the first parameter
	call hex_write_string	//Write the space " "
	addq $24, %rsp 			//realign the stack pointer for the char array
	incl %r15d
	
.LcheckWriteStop:
	cmpl %r12d, %r15d		//If index i < num_elements, continue loop
	jl	.LwriteHexNum		//Jump to loop body

	movl %r12d, %r15d		//New loop index for writing the double space, i = num_elements
	jmp .LcheckWriteDoubleSpaceStop//Start a new loop

.LwriteDoubleSpace:
	movq $sDoubleSpace, %rdi//Pass double space "   " into argument
	call hex_write_string	//Write the doble space "  "
	incl %r15d				//Increase i, the loop index

.LcheckWriteDoubleSpaceStop:
	cmpl $16, %r15d			//If index i < 16, continue loop
	jl	.LwriteDoubleSpace	//Jump to the loop body

//Write each char of the sentence to output
	movq $sSingleSpace, %rdi//Move " " as the first paramter
	call hex_write_string	//Wtite " " to the output
	movl %r12d, %ebx		//move num_elements into %ebx, ebx is the number of bytes needed
	incl %ebx				//increment %ebx for assigning space for array
	imul $8, %ebx			//multiply by 8, %ebx is now the number of bits needed
	subq %rbx, %rsp			//Assign the stack memory for the array sentence
	movq %rsp, %r14 		//Use %r14 to store sentence[num_elements + 1]
	movl $0, %r15d			//Update index i to 0 as loop control

	jmp .LcheckWriteSentence//Start a new loop

.LwriteSentence:
	movb (%rbp, %r15, 1), %al//Move userinput[i] to %al
	movb %al, %dil			//move userinput[i] as the first parameter
	call hex_to_printable	//Call hex_to_printable
	movb %al, (%r14, %r15, 1)//Move the return result form hex_to_printable to sentence[i]
	incl %r15d

.LcheckWriteSentence:
	cmpl %r12d, %r15d		//Compare loop control, if i < num_elements, continue loop
	jl .LwriteSentence		//Jump to loop body to write the sentence

	movb $0, (%r14, %r12, 1)//Move '\0' to sentence[num_elements]
	movq %r14, %rdi			//Move sentence as the first parameter
	call hex_write_string	//Call hex_write_string
	addq %rbx, %rsp			//Realign stack pointer for semtemce[num_elements + 1]
	movq $sNewLine, %rdi	//Move "\n" as the first parameter
	call hex_write_string	//Write "\n" to the output
	addl $16, %r13d			//Add 16 to count
	addq $72, %rsp			//Reaalign the stack pointer given to position char array
	cmpl $16, %r12d			//Check while loop condition
	je .LprintLoop			//Continue while loop if num_elements == 16

.LloopDone:
	movq $0, %rax			//Move 0 to rax
	addq $128, %rsp			//Realign stack pointer for userinput[16]
	addq $8, %rsp			//Realign Stack pointer for the program
	ret

/* vim:ft=gas:
 */
