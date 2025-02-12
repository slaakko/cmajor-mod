PUBLIC PushInst
PUBLIC PopInst
PUBLIC CallInst
EXTRN Foo:PROC

.CODE

PushInst PROC
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push rsp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    ret 00H
PushInst ENDP

PopInst PROC
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rbp
    pop rsp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    ret 00H
PopInst ENDP

CallInst PROC
    call Foo
    call rax
    call rbx
    call rcx
    call rdx
    call rsi
    call rdi
    call rbp
    call rsp
    call r8
    call r9
    call r10
    call r11
    call r12
    call r13
    call r14
    call r15
CallInst ENDP

AddInst PROC
    add al, al
    add al, bl
    add al, cl
    add al, dl
    add al, sil
    add al, dil
    add al, bpl
    add al, spl
    add al, r8b
    add al, r9b
    add al, r10b
    add al, r11b
    add al, r12b
    add al, r13b
    add al, r14b
    add al, r15b
    add bl, al
    add bl, bl
    add bl, cl
    add bl, dl
    add bl, sil
    add bl, dil
    add bl, bpl
    add bl, spl
    add bl, r8b
    add bl, r9b
    add bl, r10b
    add bl, r11b
    add bl, r12b
    add bl, r13b
    add bl, r14b
    add bl, r15b
    add cl, al
    add dl, al
    add sil, al
    add dil, al
    add bpl, al
    add spl, al
    add r8b, al
    add r9b, al
    add r10b, al
    add r11b, al
    add r12b, al
    add r13b, al
    add r14b, al
    add r15b, al
    add ax, ax
    add ax, bx
    add ax, cx
    add ax, dx
    add ax, si
    add ax, di
    add ax, bp
    add ax, sp
    add ax, r8w
    add ax, r9w
    add ax, r10w
    add ax, r11w
    add ax, r12w
    add ax, r13w
    add ax, r14w
    add ax, r15w
    add bx, ax
    add bx, bx
    add bx, cx
    add bx, dx
    add bx, si
    add bx, di
    add bx, bp
    add bx, sp
    add bx, r8w
    add bx, r9w
    add bx, r10w
    add bx, r11w
    add bx, r12w
    add bx, r13w
    add bx, r14w
    add bx, r15w
    add eax, eax
    add eax, ebx
    add eax, ecx
    add eax, edx
    add eax, esi
    add eax, edi
    add eax, ebp
    add eax, esp
    add eax, r8d
    add eax, r9d
    add eax, r10d
    add eax, r11d
    add eax, r12d
    add eax, r13d
    add eax, r14d
    add eax, r15d
    add ebx, eax
    add ebx, ebx
    add ebx, ecx
    add ebx, edx
    add ebx, esi
    add ebx, edi
    add ebx, ebp
    add ebx, esp
    add ebx, r8d
    add ebx, r9d
    add ebx, r10d
    add ebx, r11d
    add ebx, r12d
    add ebx, r13d
    add ebx, r14d
    add ebx, r15d
    add ecx, eax
    add edx, eax
    add esi, eax
    add edi, eax
    add ebp, eax
    add esp, eax
    add r8d, eax
    add r9d, eax
    add r10d, eax
    add r11d, eax
    add r12d, eax
    add r13d, eax
    add r14d, eax
    add r15d, eax
    add rax, rax
    add rax, rbx
    add rax, rcx
    add rax, rdx
    add rax, rsi
    add rax, rdi
    add rax, rbp
    add rax, rsp
    add rax, r8
    add rax, r9
    add rax, r10
    add rax, r11
    add rax, r12
    add rax, r13
    add rax, r14
    add rax, r15
    add rbx, rax
    add rbx, rbx
    add rbx, rcx
    add rbx, rdx
    add rbx, rsi
    add rbx, rdi
    add rbx, rbp
    add rbx, rsp
    add rbx, r8
    add rbx, r9
    add rbx, r10
    add rbx, r11
    add rbx, r12
    add rbx, r13
    add rbx, r14
    add rbx, r15
    add rcx, rax
    add rdx, rax
    add rsi, rax
    add rdi, rax
    add rbp, rax
    add rsp, rax
    add r8, rax
    add r9, rax
    add r10, rax
    add r11, rax
    add r12, rax
    add r13, rax
    add r14, rax
    add r15, rax
AddInst ENDP

END

