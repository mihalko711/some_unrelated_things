#include <windows.h>
#include <stdio.h>

typedef void (*AnotherFunc)(char*, int, char*); // объявление прототипа функции (dynamic)
extern "C" __declspec(dllimport) void StringVar19(char*, int, char*); // импорт функции для (static)

int main(){
    HINSTANCE lib = LoadLibrary("string_var19.dll"); // Загружаем библиотеку
    if(!lib){ 
        // Ну а вдруг что не так?
        printf("Failed to load a library!\n");
        DWORD err = GetLastError();
        printf("Ошибка загрузки DLL: %lu\n", err);  
        return 1;
    }

    AnotherFunc f = (AnotherFunc)GetProcAddress(lib,"StringVar19"); // Загружаем функцию
    if(!f){
        // Ну а вдруг что не так?
        printf("Function not found!\n");
        return 1;
    }

    char input[256]; //входная строка
    char buffer[10]; //для чтения числа
    int length;

    printf("Input string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; //перенос строки портит обработку последнего слова
    printf("Input word length: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &length);
    char output1[256] = {0}; // выхходная строка
    char output2[256] = {0}; // выхходная строка
    

    StringVar19(input, length, output1);
    f(input, length, output2);

    printf("Result of static library: %s\n", output1);
    printf("Result of dynamic library: %s\n", output1);

    return 0;
 }