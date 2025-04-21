#include <windows.h>
#include <stdio.h>

typedef void (_stdcall AnotherFunc)(char*, int, char*);


int main(){
    HMODULE lib = LoadLibrary("string_var19.dll");
    if(!lib){
        print("Не удалось загрузить библиотеку!\n");
        return 1;
    }

    AnotherFunc f = (AnotherFunc)GetProcAdress("StringVar19");
    if(!f){
        print("Не удалось найти функцию\n");
        return 1;
    }
}