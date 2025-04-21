.686
.model flat, stdcall
option casemap: none

public var19
.code
var19 proc
    pushad ; сохранение E-регистров
    pushfd ; сохранение значений флагов
    xor esi,esi ;индекс обхода строки 1
    xor edi,edi ;индекс обхода строки 2

    push ebp ; сохраним значение esp
    mov ebp,esp ; save stack
    sub esp,4 ;позиция записи в выходно строке
    mov dword ptr [esp],0 ;позиция записи в выходно строке

    mov eax, [ebp + 8] ;input string pointer 
    ;[ebp + 12]length of word
    mov edx, [ebp + 16] ;output buffer pointer
    
    ; string_cycle - базовй проход по строке
    ; при встрече разделителя переход на write_word
    string_cycle:
        mov cl, [eax + esi] ;read sign
        cmp cl, ' ' ;space matching
        je write_word
        cmp cl,0 ;EOF
        je after_cycle_
        inc esi
        jmp string_cycle

    ; write_word - поиск длинs слова и 
    ; при "хорошей" длине, переход на запись write_comp
    ; иначе - возвращение в цикл
    write_word:
        mov ecx, esi
        sub ecx, edi
        cmp ecx, [ebp + 12]
        jne write_comp
        mov edi,esi
        jmp string_cycle

    ; запись слова и возвращение в string_cycle
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
    popfd
    popad
    ret 4
var19 endp

DllMain proc hInstDLL:WORD, fdwReason:DWORD, lpReserved:DWORD
    mov eax, 1
    ret
DllMain

end DllMain
