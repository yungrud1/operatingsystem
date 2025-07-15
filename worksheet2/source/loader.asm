global loader                   ; the entry symbol for ELF
extern kmain                    ; declare external C function

MAGIC_NUMBER equ 0x1BADB002    ; define the magic number constant
FLAGS        equ 0x0           ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER ; calculate the checksum
                               ; (magic number + checksum + flags should equal 0)

KERNEL_STACK_SIZE equ 4096     ; size of stack in bytes

section .bss
align 4                        ; align stack on 4-byte boundary
kernel_stack:
    resb KERNEL_STACK_SIZE     ; reserve stack space

section .text:                 ; start of the text (code) section
    align 4                    ; the code must be 4 byte aligned
    dd MAGIC_NUMBER            ; write the magic number to the machine code,
    dd FLAGS                   ; the flags,
    dd CHECKSUM                ; and the checksum

loader:                        ; the loader label (defined as entry point in linker script)
    mov eax, 0xCAFEBABE       ; place the number 0xCAFEBABE in the register eax
    
    ; Set up the stack for C function calls
    mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point stack to end of stack area
    
    ; Call the C main function
    call kmain
    
    ; After returning from C, the result should be in EAX
    ; Continue with infinite loop
.loop:
    jmp .loop                  ; loop forever
