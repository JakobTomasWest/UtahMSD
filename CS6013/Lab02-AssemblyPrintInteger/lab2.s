
section .rodata ;read only data section

section .text ; this says that we're about to write code (as opposed to data)
	global print_int
	global main


	;;void print_int( long x);
	;;	
	;1234=> '1' '2' '3' '4'
	;
	;; rax ... rax is our number 1234... rdx is the remainder
	;1234 /10 => 123 remainder 4
	;123 /10 => 12 remainder 3
	;12 /10  => 1 remainder 2
	;1/10 => 0 remainder 0
	; 0   <- stop

	
print_int:
	;; prologue
	;; first: put base pointer on the stack  
	push rbp		;; save the previous ( in our case 'main') bp so we can go back to it
	mov rbp, rsp		;;  Update teh bp to your location ( incase we call someone)

	sub rsp, 20		;; bytes (characters and bytes are same vvalue) on stack for each digit
	
	;;Start the main code
	mov rax, rdi		;; copy the passed in (1st) param (x) into AX reg to do work there.
	mov rbx, 1		;;create an index: BX is the 'iTH' location in digit array (the index)

	mov byte [rsp+20], 10		;; have to dereference to get stack  put CR (ascii 10) into the first location of my digits array
	;; Loop over number until it reaches 0, getting remainder
	mov r10, 20		;; allocate 20 bits for r10 register
dividing:

	cmp rax, 0
	je print		;; if the above thing is equal than we jump to print

	mov rdx, 0		;; if you don't initlialize DX to 0, then div crashes
	mov rcx, 10		;;  
	div  rcx		;; rax <= rax / rcx (integer math) ... remainder is put into DX

	add rdx, 48		;; Turn the number, into the digig '4' (based on the ascii values)
	inc rbx			;; add rbx, 1 == increment our counter/index into our array of digits
	dec r10			;; decrement register to 
	mov byte[rsp+r10], dl	;; store the digit ( the remainder) in 8 bit register 
	
	jmp dividing  

	
print:
	

	mov rax, 1 		;; 1 tells the OS to use the WRITE function
	mov rdi, 1		;; 1st parma: which is file desrcriptor ((1 == std out)
	
	mov rcx, rsp
	add rcx , r10		;; add position  for reverse print
	
	mov rsi, rcx		;; 2nd param: address of the first character in our array of characters
	mov rdx, rbx		;; 3rd prama: rbx holds teh # of digits - put in rdx so Write will see it

	syscall
	;;done... return
	;;EPLIGOUE... fix up stack and base pointers

	
	mov rsp, rbp
	pop rbp
	ret
main:

	mov rdi, 1234
	call print_int
	
	mov rax, 60		;; sys_exit system call
	mov rdi, 0		;; 1st param: errror code
	syscall			;
