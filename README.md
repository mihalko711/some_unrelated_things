# some_unrelated_things

# Динамическое и статическое подключение библиотек, написанных на `MASM32`

Данная задача состоит в написании некоторого кода на MASM32 и последющем подключении его в виде библиотек к коду `C\C++`
Весь код в папке `masm_string_library`

## Инструкции по сборке
### `MASM32`

Сначала сборка в объектнй файл `COFF`(почему-то без этого `link` не работает, то же самое с одноименной опцией в `ml`):

```sh
ml /c /coff prog_name.asm
```

Потом работа с `link`:

```sh
link /dll /subsystem:windows /export:procedure_name prog_name.obj
```


или все вместе:

```sh
ml /c /coff prog_name.asm && link /subsystem:console prog_name.obj
```
### Library loading(static)

Тут нужно закинуть `lib_name.lib` в папку с программой

и собирать все командой 

```sh
your_compilator_name main_c_prog.cpp lib_name.lib
```


```cpp
#include <stdio.h>

extern int __stdcall AnotherFunc(char*,int, char*);

int main(){
}
```

### Library loading(dynamic)

```cpp
#include <windows.h> //для LoadLibrary, FreeLibrary
...
typedef void (__stdcall *AnotherFunc)(char*,int, char*); //тип нашей загружаемой функции
...
int main(){
    HMODULE lib  = LoadLibrary("some_dll_name.dll");
    if(!lib){
        print("Failed to load DLL\n");
        return 1;
    }

    AnotherFunc f = (AnotherFunc)GetProcAdress("ExpFuncName");
    if(!f){
        printf("Failed to get function\n");
        return 1;
    }
}
```