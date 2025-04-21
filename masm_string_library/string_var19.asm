.686
.model flat, c
option casemap: none

public StringVar19
.code
StringVar19 proc
    xor esi,esi ;индекс обхода строки 1
    xor edi,edi ;индекс обхода строки 2
    xor ecx,ecx

    push ebp ; сохраним значение esp
    mov ebp,esp ; save stack
    pushad ; сохранение E-регистров
    pushfd ; сохранение значений флагов
    sub esp,4 ;позиция записи в выходно строке
    mov dword ptr [esp],0 ;позиция записи в выходно строке

    mov eax, [ebp + 8] ;input string pointer 
    ;[ebp + 12]length of word
    mov edx, [ebp + 16] ;output buffer pointer
    
    ; string_cycle - базовй проход по строке
    ; при встрече разделителя переход на write_word
    string_cycle:
        mov cl, [eax + esi] ;read sign
        inc esi
        cmp cl, ' ' ;space matching
        je write_word ; переход на проверку длины
    string_cycle_continue: ; инкрементирем главный счетчик
        cmp cl,0 ;EOF
        je after_cycle_
        jmp string_cycle

    ; write_word - поиск длинs слова и 
    ; при "хорошей" длине, переход на запись write_comp
    ; иначе - возвращение в цикл
    write_word:
        mov ecx, esi 
        sub ecx, edi ;сравнение главного счетчика с позицией начла слова 
        dec ecx
        cmp ecx, [ebp + 12] ; сравнение длины слова с требуемой величиной
        jne write_comp ; в случае неравенства идем на запись  слова
        mov edi,esi ;сброс счетчика с начала слова до его конца
        jmp string_cycle_continue

    ; запись слова и возвращение в string_cycle на нужную позицию
    write_comp:
        cmp esi,edi ; пока не дойдем до конца слова
        je string_cycle_continue 
        mov cl, [eax + edi] ; возьмем нужнй символ
        mov ebx, [esp] 
        mov [edx + ebx], cl  ; запишем символ слова в новую строку 
        inc edi 
        inc dword ptr [esp] ; инкрементируем проход по входному и выходному слову
        cmp cl, 0
        je final ; случай обработки последнего слова строки
        jmp write_comp

    after_cycle_:
        mov ecx, esi 
        sub ecx, edi ;сравнение главного счетчика с позицией начла слова
        dec ecx
        cmp ecx, [ebp + 12] ; сравнение длины слова с требуемой величиной
        jne write_comp
    
    final:
        add esp,4 ; нужно сдвинуть стек обратно, чтоб вернуть флаги и регистры
        popfd ; озвращаем флаги и регистры
        popad
        mov esp,ebp ; stack reincarnation
        pop ebp ; base pointer reincarnation
        ret
StringVar19 endp

DllMain proc hInstDLL:WORD, fdwReason:DWORD, lpReserved:DWORD
    mov eax, -1
    ret
DllMain endp

end DllMain


