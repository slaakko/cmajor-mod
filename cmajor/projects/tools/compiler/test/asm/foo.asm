.CODE

Foo PROC
    mov r8, [rax]
    mov r8, [rbx]
    mov r8, [rcx]
    mov r8, [rdx]
    mov r8, [rsi]
    mov r8, [rdi]
    mov r8, [rsp]
    mov r8, [rbp]
    mov r8, [r8]
    mov r8, [r9]
    mov r8, [r10]
    mov r8, [r11]
    mov r8, [r12]
    mov r8, [r13]
    mov r8, [r14]
    mov r8, [r15]
Foo ENDP

Bar PROC
    mov rax, [r13]
    mov rbx, [r13]
    mov rcx, [r13]
    mov rdx, [r13]
    mov rsi, [r13]
    mov rdi, [r13]
    mov rsp, [r13]
    mov rbp, [r13]
    mov r8, [r13]
    mov r9, [r13]
    mov r10, [r13]
    mov r11, [r13]
    mov r12, [r13]
    mov r13, [r13]
    mov r14, [r13]
    mov r15, [r13]
Bar ENDP

Baz PROC
    mov byte ptr [rax],al
    mov byte ptr [rbx],al
    mov byte ptr [rcx],al
    mov byte ptr [rdx],al
    mov byte ptr [rsi],al
    mov byte ptr [rdi],al
    mov byte ptr [rsp],al
    mov byte ptr [rbp],al
    mov byte ptr [r8],al
    mov byte ptr [r9],al
    mov byte ptr [r10],al
    mov byte ptr [r11],al
    mov byte ptr [r12],al
    mov byte ptr [r13],al
    mov byte ptr [r14],al
    mov byte ptr [r15],al
Baz ENDP

Xyzzy PROC
    mov r8b, byte ptr [rax]
    mov r8b, byte ptr [rbx]
    mov r8b, byte ptr [rcx]
    mov r8b, byte ptr [rdx]
    mov r8b, byte ptr [rsi]
    mov r8b, byte ptr [rdi]
    mov r8b, byte ptr [rbp]
    mov r8b, byte ptr [rsp]
    mov r8b, byte ptr [r8]
    mov r8b, byte ptr [r9]
    mov r8b, byte ptr [r10]
    mov r8b, byte ptr [r11]
    mov r8b, byte ptr [r12]
    mov r8b, byte ptr [r13]
    mov r8b, byte ptr [r14]
    mov r8b, byte ptr [r15]
Xyzzy ENDP

Qwerty PROC
    mov [rax], r8
    mov [rbx], r8
    mov [rcx], r8
    mov [rdx], r8
    mov [rsi], r8
    mov [rdi], r8
    mov [rsp], r8
    mov [rbp], r8
    mov [r8], r8
    mov [r9], r8
    mov [r10], r8
    mov [r11], r8
    mov [r12], r8
    mov [r13], r8
    mov [r14], r8
    mov [r15], r8
Qwerty ENDP

Compare PROC
    cmp sil,r13b
Compare ENDP

END
