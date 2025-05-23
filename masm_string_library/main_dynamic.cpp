#include <windows.h>
#include <stdio.h>

typedef void (*AnotherFunc)(char*, int, char*); // объявление прототипа функции


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
    char output[256] = {0}; // выхходная строка
    

    f(input, length, output);

    printf("Result: %s\n", output);
    printf("Result: %d\n", length);


    FreeLibrary(lib);
    return 0;
}