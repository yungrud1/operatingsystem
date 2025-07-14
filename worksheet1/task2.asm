%include "asm_io.inc"

segment .data
    ; Program 1: Name and welcome message
    prompt_name     db "Enter your name: ", 0
    prompt_times    db "Enter number of times to print welcome (51-99): ", 0
    welcome_msg     db "Welcome, ", 0
    exclamation     db "!", 10, 0  ; newline included
    error_too_small db "Error: Number too small (must be >= 51)", 10, 0
    error_too_large db "Error: Number too large (must be <= 99)", 10, 0
    
    ; Program 2: Array sum
    array_sum_msg   db "Sum of array (1-100): ", 0
    
    ; Program 3: Range sum
    prompt_start    db "Enter start of range (1-100): ", 0
    prompt_end      db "Enter end of range (1-100): ", 0
    range_sum_msg   db "Sum of range: ", 0
    error_invalid   db "Error: Invalid range", 10, 0

segment .bss
    user_name       resb 100    ; buffer for user name
    times_count     resd 1      ; number of times to print
    array           resd 100    ; array of 100 integers
    array_sum       resd 1      ; sum of array
    range_start     resd 1      ; start of range
    range_end       resd 1      ; end of range
    range_sum       resd 1      ; sum of range

segment .text
    global asm_main

asm_main:
    pusha
    
    ; Program 1: Name and welcome message
    call program1_name_welcome
    call print_nl
    
    ; Program 2: Array initialization and sum
    call program2_array_sum
    call print_nl
    
    ; Program 3: Range sum
    call program3_range_sum
    
    popa
    mov eax, 0
    ret

; Program 1: Ask for name and number of times, print welcome message
program1_name_welcome:
    pusha
    
    ; Get user's name
    mov eax, prompt_name
    call print_string
    
    ; Read the entire name string
    mov ebx, user_name      ; pointer to name buffer
    mov ecx, 0              ; character counter
    
.read_name_loop:
    call read_char
    cmp al, 10              ; check for newline (ASCII 10)
    je .name_done           ; if newline, we're done
    cmp ecx, 99             ; check buffer limit (leave space for null terminator)
    jge .read_name_loop     ; if buffer full, ignore extra characters
    
    mov [ebx + ecx], al     ; store character in buffer
    inc ecx                 ; increment counter
    jmp .read_name_loop
    
.name_done:
    mov byte [ebx + ecx], 0 ; null-terminate the string
    
    ; Get number of times
    mov eax, prompt_times
    call print_string
    call read_int
    mov [times_count], eax
    
    ; Check if number is in valid range (51-99)
    cmp eax, 51
    jl .error_small
    cmp eax, 99
    jg .error_large
    
    ; Print welcome message the specified number of times
    mov ecx, [times_count]
.print_loop:
    push ecx            ; save counter
    
    mov eax, welcome_msg
    call print_string
    mov eax, user_name      ; print the entire name string
    call print_string
    mov eax, exclamation
    call print_string
    
    pop ecx             ; restore counter
    loop .print_loop
    jmp .done
    
.error_small:
    mov eax, error_too_small
    call print_string
    jmp .done
    
.error_large:
    mov eax, error_too_large
    call print_string
    
.done:
    popa
    ret

; Program 2: Initialize array 1-100 and sum it
program2_array_sum:
    pusha
    
    ; Initialize array with values 1-100
    mov ecx, 100        ; counter
    mov eax, 1          ; current value
    mov ebx, array      ; array pointer
    
.init_loop:
    mov [ebx], eax      ; store current value
    add ebx, 4          ; move to next element (4 bytes per int)
    inc eax             ; increment value
    loop .init_loop
    
    ; Sum the array
    mov ecx, 100        ; counter
    mov eax, 0          ; sum accumulator
    mov ebx, array      ; array pointer
    
.sum_loop:
    add eax, [ebx]      ; add current element to sum
    add ebx, 4          ; move to next element
    loop .sum_loop
    
    mov [array_sum], eax ; store sum
    
    ; Print result
    mov eax, array_sum_msg
    call print_string
    mov eax, [array_sum]
    call print_int
    call print_nl
    
    popa
    ret

; Program 3: Sum a user-specified range of the array
program3_range_sum:
    pusha
    
    ; Get start of range
    mov eax, prompt_start
    call print_string
    call read_int
    mov [range_start], eax
    
    ; Get end of range
    mov eax, prompt_end
    call print_string
    call read_int
    mov [range_end], eax
    
    ; Validate range
    mov eax, [range_start]
    cmp eax, 1
    jl .invalid_range
    cmp eax, 100
    jg .invalid_range
    
    mov eax, [range_end]
    cmp eax, 1
    jl .invalid_range
    cmp eax, 100
    jg .invalid_range
    
    mov eax, [range_start]
    cmp eax, [range_end]
    jg .invalid_range
    
    ; Calculate sum of range
    mov eax, 0              ; sum accumulator
    mov ecx, [range_start]  ; current index
    
.range_sum_loop:
    ; Calculate address: array + (index-1)*4
    mov ebx, ecx
    dec ebx                 ; convert to 0-based index
    shl ebx, 2              ; multiply by 4 (shift left 2 bits)
    add ebx, array          ; add base address
    
    add eax, [ebx]          ; add current element to sum
    inc ecx                 ; increment index
    cmp ecx, [range_end]
    jle .range_sum_loop
    
    mov [range_sum], eax    ; store sum
    
    ; Print result
    mov eax, range_sum_msg
    call print_string
    mov eax, [range_sum]
    call print_int
    call print_nl
    jmp .done
    
.invalid_range:
    mov eax, error_invalid
    call print_string
    
.done:
    popa
    ret
