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
ml /c /coff prog_name.asm && link /dll /subsystem:windows /export:FuncName prog_name.obj
```
### Library loading(static)

Тут нужно закинуть `lib_name.lib` в папку с программой

и собирать все командой 

```sh
your_compilator_name main_c_prog.cpp lib_name.lib
```


```cpp
#include <stdio.h>

extern void __stdcall AnotherFunc(char*,int, char*);

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

```sh
your_compilator_name main_c_prog.cpp
```

Я использовал 32-битный `mingw-gcc`, в проблемах с 64-битным предпочел не разбираться. Но предполагаю, что переход на `MASM64` должен помочь.

### Смешанное подключение

Есть `main.cpp` в нем и динамическое подключение и статическое

Как собирать?


```shell
ml /c /coff string_var19.asm && link /dll /subsystem:windows /export:StringVar19 string_var19.obj && gcc main.cpp string_var19.lib -o main.exe
```

## Лан, запишем все команды сборки

### Статика

```shell
ml /c /coff string_var19.asm && link /dll /subsystem:windows /export:StringVar19 string_var19.obj && gcc main_static.cpp string_var19.lib -o main_static.exe
```

### Динамика

```shell
ml /c /coff string_var19.asm && link /dll /subsystem:windows /export:StringVar19 string_var19.obj && gcc main_dynamic.cpp -o main_dynamic.exe
```

### Мешанина

```shell
ml /c /coff string_var19.asm && link /dll /subsystem:windows /export:StringVar19 string_var19.obj && gcc main.cpp string_var19.lib -o main.exe
```


# Как работает программа?

Да и что она вообще делает?

Все просто, программма по входной строке, натуральному числу  и адресу строки для записи запишет в строку для записи из входной строки только те слова, чья длина не равна заданному натуральному числу.

В  рамках каждого из подключений программа предлагает вести строку, потом число и производит вызов 1-2 функций для этих входных данных.


например:

```shell
 ./main_static.exe
Input string: re abc pooop jkfke jfjf
Input word length: 2
Result: abc pooop jkfke jfjf
```