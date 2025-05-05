#include <sys/ipc.h> //ftok(), key_t
#include <sys/shm.h> // работа с shm
#include <sys/sem.h> // работа с sem
#include <sys/types.h> // некоторые типы могут содержаться здесь
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // для обработки Ctrl-C
#include <string.h>
#include "ipc_common.h"


int shmid, semid;

void cleanup(){
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
}

void handle_sigint(){
    cleanup();
    exit(0);
}



int main(){
    atexit(cleanup);
    signal(SIGINT, handle_sigint);
    key_t key = ftok("for_key",PROJ_ID);
    printf("Producers key: %d!\n", key);

    // создание разделяемого сегмента памяти

    shmid = shmget(key, BUFF_SIZE, IPC_CREAT | 0666);
    
    if (shmid == -1){
        perror("shmget");
        exit(1);
    }
    else printf("Generated common memory segment: %d!\n", shmid);

    // создание семафоров

    semid = semget(key, 2, IPC_CREAT | 0666);
    
    if (semid == -1){
        perror("semget");
        exit(1);
    }
    else printf("Generated semaphore: %d!\n", semid);

    // структура для инициализации 

    union semun {
        int val;  //SETVAL
        struct semid_ds *buf; //IPC_STAT, IPC_SET
        unsigned short *array; //GETALL, SETALL
    };

    // массив со значениями для инициализации
    unsigned short values[2] = {S_CAN_READ, S_CAN_WRITE};
    

    union semun arg;


    // инициализация
    arg.array = values;
    semctl(semid,0,SETALL,arg);

    
    int fl = 1;
    while (fl) {
        semop(semid, &wait_write, 1);
        

        char *shm_ptr = shmat(shmid,NULL,0);
        if (shm_ptr == (char *) -1){
            perror("shmat");
            exit(1);
        }

        char input[STR_LEN];
        printf("Input message(non empty): ");
        fgets(input, STR_LEN, stdin);
        input[strcspn(input,"\n")] = '\0';
        strcpy(shm_ptr, input);
        if(strlen(input) == 0){
            fl = 0;
            printf("Got an empty string...exit...");
        }
        shmdt(shm_ptr);
        semop(semid, &signal_read,1);

        //выводим значения

        int semval_can_read = semctl(semid,0,GETVAL);
        int semval_can_write = semctl(semid,1,GETVAL);

        if(semval_can_read == -1){
            perror("semctl GETVAL");
        }
        else printf("Semaphore CAN_READ = %d\n", semval_can_read);

        if(semval_can_write == -1){
            perror("semctl GETVAL");
        }
        else printf("Semaphore CAN_WRITE = %d\n", semval_can_write);
    }
    
}