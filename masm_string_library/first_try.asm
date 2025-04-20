.686
.model flat, stdcall
option casemap: none

include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
includelib \masm32\lib\kernel32.lib
include \masm32\include\masm32rt.inc

.data
    msg db "pokA",0
    buf db 26 dup(?)
    num dw 4
    cntrl db "pipyava",10,0


.code
first_try:
    xor ebx,ebx
    push offset buf
    push offset num
    push offset msg
    call var19
    push offset cntrl
    call StdOut


    var19 proc
        pushad
        xor esi,esi ;индекс обхода строки 1
        xor edi,edi ;индекс обхода строки 2

        push ebp
        mov ebp,esp ; save stack
        sub esp,4 ;позиция записи в выходно строке
        mov dword ptr [esp],0 ;позиция записи в выходно строке

        mov eax, [ebp + 8] ;input string pointer 
        ;[ebp + 12]length of word
        mov edx, [ebp + 16] ;output buffer pointer
        

        string_cycle:
            mov cl, [eax + esi] ;read sign
            cmp cl, ' ' ;space matching
            je write_word
            cmp cl,0 ;EOF
            je after_cycle_
            inc esi
            jmp string_cycle

        write_word:
            mov ecx, esi
            sub ecx, edi
            cmp ecx, [ebp + 12]
            jne write_comp
            mov edi,esi
            jmp string_cycle

        write_comp:
            cmp esi,edi
            je string_cycle
            mov cl, [eax + edi]
            mov ebx, [esp]
            mov [edx + ebx], cl 
            inc edi
            inc dword ptr [esp]
            jmp write_comp
            
        after_cycle_:
            cmp esi,edi
            jne write_word

        mov esp,ebp ; stack reincarnation
        pop ebp ; base pointer reincarnation
        popad
        ret 4
    var19 endp


end first_try
