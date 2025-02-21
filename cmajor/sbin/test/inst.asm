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
    add al, 12H
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
    add bl, 12H
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
    add ax, 1234H;
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
    add bx, 1234H
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
    add eax, 12345678H
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
    add ebx, 12345678H
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
    add rax, 12345678H
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
    add rbx, 12345678H
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

SubReg64Imm32Inst PROC
    sub rax, 12345678H
    sub rbx, 12345678H
    sub rcx, 12345678H
    sub rdx, 12345678H
    sub rsi, 12345678H
    sub rdi, 12345678H
    sub rbp, 12345678H
    sub rsp, 12345678H
    sub r8, 12345678H
    sub r9, 12345678H
    sub r10, 12345678H
    sub r11, 12345678H
    sub r12, 12345678H
    sub r13, 12345678H
    sub r14, 12345678H
SubReg64Imm32Inst ENDP

LeaInst PROC
    lea rax, qword ptr [rax+0000000000000012H]
    lea rbx, qword ptr [rax+0000000000000012H]
    lea rcx, qword ptr [rax+0000000000000012H]
    lea rdx, qword ptr [rax+0000000000000012H]
    lea rsi, qword ptr [rax+0000000000000012H]
    lea rdi, qword ptr [rax+0000000000000012H]
    lea rbp, qword ptr [rax+0000000000000012H]
    lea rsp, qword ptr [rax+0000000000000012H]
    lea r8, qword ptr [rax+0000000000000012H]
    lea r9, qword ptr [rax+0000000000000012H]
    lea r10, qword ptr [rax+0000000000000012H]
    lea r11, qword ptr [rax+0000000000000012H]
    lea r12, qword ptr [rax+0000000000000012H]
    lea r13, qword ptr [rax+0000000000000012H]
    lea r14, qword ptr [rax+0000000000000012H]
    lea r15, qword ptr [rax+0000000000000012H]
LeaInst ENDP

SubInst PROC
    sub al, al
    sub al, bl
    sub al, cl
    sub al, dl
    sub al, sil
    sub al, dil
    sub al, bpl
    sub al, spl
    sub al, r8b
    sub al, r9b
    sub al, r10b
    sub al, r11b
    sub al, r12b
    sub al, r13b
    sub al, r14b
    sub al, r15b
    sub al, 12H
    sub bl, al
    sub bl, bl
    sub bl, cl
    sub bl, dl
    sub bl, sil
    sub bl, dil
    sub bl, bpl
    sub bl, spl
    sub bl, r8b
    sub bl, r9b
    sub bl, r10b
    sub bl, r11b
    sub bl, r12b
    sub bl, r13b
    sub bl, r14b
    sub bl, r15b
    sub bl, 12H
    sub cl, al
    sub dl, al
    sub sil, al
    sub dil, al
    sub bpl, al
    sub spl, al
    sub r8b, al
    sub r9b, al
    sub r10b, al
    sub r11b, al
    sub r12b, al
    sub r13b, al
    sub r14b, al
    sub r15b, al
    sub ax, ax
    sub ax, bx
    sub ax, cx
    sub ax, dx
    sub ax, si
    sub ax, di
    sub ax, bp
    sub ax, sp
    sub ax, r8w
    sub ax, r9w
    sub ax, r10w
    sub ax, r11w
    sub ax, r12w
    sub ax, r13w
    sub ax, r14w
    sub ax, r15w
    sub ax, 1234H;
    sub bx, ax
    sub bx, bx
    sub bx, cx
    sub bx, dx
    sub bx, si
    sub bx, di
    sub bx, bp
    sub bx, sp
    sub bx, r8w
    sub bx, r9w
    sub bx, r10w
    sub bx, r11w
    sub bx, r12w
    sub bx, r13w
    sub bx, r14w
    sub bx, r15w
    sub bx, 1234H
    sub eax, eax
    sub eax, ebx
    sub eax, ecx
    sub eax, edx
    sub eax, esi
    sub eax, edi
    sub eax, ebp
    sub eax, esp
    sub eax, r8d
    sub eax, r9d
    sub eax, r10d
    sub eax, r11d
    sub eax, r12d
    sub eax, r13d
    sub eax, r14d
    sub eax, r15d
    sub eax, 12345678H
    sub ebx, eax
    sub ebx, ebx
    sub ebx, ecx
    sub ebx, edx
    sub ebx, esi
    sub ebx, edi
    sub ebx, ebp
    sub ebx, esp
    sub ebx, r8d
    sub ebx, r9d
    sub ebx, r10d
    sub ebx, r11d
    sub ebx, r12d
    sub ebx, r13d
    sub ebx, r14d
    sub ebx, r15d
    sub ebx, 12345678H
    sub ecx, eax
    sub edx, eax
    sub esi, eax
    sub edi, eax
    sub ebp, eax
    sub esp, eax
    sub r8d, eax
    sub r9d, eax
    sub r10d, eax
    sub r11d, eax
    sub r12d, eax
    sub r13d, eax
    sub r14d, eax
    sub r15d, eax
    sub rax, rax
    sub rax, rbx
    sub rax, rcx
    sub rax, rdx
    sub rax, rsi
    sub rax, rdi
    sub rax, rbp
    sub rax, rsp
    sub rax, r8
    sub rax, r9
    sub rax, r10
    sub rax, r11
    sub rax, r12
    sub rax, r13
    sub rax, r14
    sub rax, r15
    sub rax, 12345678H
    sub rbx, rax
    sub rbx, rbx
    sub rbx, rcx
    sub rbx, rdx
    sub rbx, rsi
    sub rbx, rdi
    sub rbx, rbp
    sub rbx, rsp
    sub rbx, r8
    sub rbx, r9
    sub rbx, r10
    sub rbx, r11
    sub rbx, r12
    sub rbx, r13
    sub rbx, r14
    sub rbx, r15
    sub rbx, 12345678H
    sub rcx, rax
    sub rdx, rax
    sub rsi, rax
    sub rdi, rax
    sub rbp, rax
    sub rsp, rax
    sub r8, rax
    sub r9, rax
    sub r10, rax
    sub r11, rax
    sub r12, rax
    sub r13, rax
    sub r14, rax
    sub r15, rax
    sub rsp, 12345678H
SubInst ENDP

END

