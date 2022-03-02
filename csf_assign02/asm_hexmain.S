/* Hexdump main function, assembly language version */

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	subq $8, %rsp
	subq $128, %rsp //Initialize 16 bytes of storage for char array to store user input
	movq %rsp, %rbp //%rbp is the array pointer of the char array
	movl $16, %r12d //initialize num_elements for loop condition
	movl $0, %r13d //Initialize count for tracking the position

	//.LprintLoop:
	movq %rbp, %rdi //move userinput[16] into parameter
	call hex_read //call hex read
	
	//jmp .LprintLoop

	.LloopDone:
	addq $8, %rsp
	ret

/* vim:ft=gas:
 */
