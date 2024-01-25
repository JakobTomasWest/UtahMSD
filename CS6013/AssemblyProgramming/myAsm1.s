section .rodata  ; this is the read only data (hello is a constant) 
helloString: db "hello",0 ; hellostring is the name of our symbol
; db is the directive to put data in our object file
; the 0 is the null terminator that puts is expecting.  
; nasm does NOT null terminate our string automatically

extern puts    ; telling nasm that puts is an external provided function from c
section .text  ; this says that we're about to write code (as opposed to data)

global _sayHello ; this says that "sayHello" is a symbol that the linker needs to know about
global _myPuts
global _myGTOD

_sayHello:           ; and here we go... 
                    ; the function's code goes here
 mov rdi, helloString ; move address of helloSting to 0 arguement regiser RDI
 call puts
  ret                 ; just return (for now)

_myPuts:

 mov rax, 1		; putting system call for write into the correct register:1
 mov rdx, rsi              ; move 2nd parm - count into  rdx
 mov rsi, rdi              ; move 1st parm - buffer into  rsi
 mov rdi, 1		; fd uses 1 for standard output to terminal
 syscall		; tigger kernel
 ret
 

_myGTOD:

 sub rsp, 16; number of bytes/space I want to allocate on the struct for our time val struct
 mov rdi, rsp; Pointer to time value struct, first parm:  struct timeval *tv
 mov rsi, 0; Put null pointer in this register on our stack for time sonze
 mov rax, 96; Execute a system call instruction for GTOD 
 syscall
 mov rax, [rsp] ; put timeval struct first val- seconds into rax
 mov rdx, [rsp +8] ; put time  val struct second par - ms into rdx
 add rsp, 16 ; give back the stack space by adding to rsp
 ret
