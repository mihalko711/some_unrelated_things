#include <stdio.h>
#include <string.h> //для strcspn

extern "C" __declspec(dllimport) void StringVar19(char*, int, char*); // импорт функции


 int main(){
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
    

    StringVar19(input, length, output);

    printf("Result: %s\n", output);

    return 0;
 }