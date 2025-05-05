#include <sys/ipc.h> //ftok(), key_t
#include <sys/shm.h> // работа с shm
#include <sys/sem.h> // работа с sem
#include <sys/types.h> // некоторые типы могут содержаться здесь
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ipc_common.h"

int main(){
    key_t key = ftok("for_key",PROJ_ID);
    printf("Consumers key: %d!\n", key);

    // подключение общего сегмента

    int shmid = shmget(key, BUFF_SIZE, 0666);
    

    if (shmid == -1){
        perror("shmget");
        exit(1);
    }
    else printf("Connected common memory segment: %d!\n", shmid);

    // подключение семафоров

    int semid = semget(key, 2, 0666);
    
    if (semid == -1){
        perror("semget");
        exit(1);
    }
    else printf("Connected semaphore: %d!\n", semid);
    int fl = 1;
    while (fl){
        semop(semid, &wait_read, 1);
        //выводим значения
        char *shm_ptr = shmat(shmid,NULL,0);
        if (shm_ptr == (char *) -1){
            perror("shmat");
            exit(1);
        }

        if (strlen(shm_ptr) > 0){
            printf("Message: \"%s\"\n", shm_ptr);

            shm_ptr[0] == '\0'; // затирание строки
        }
        else{
            fl = 0;
            printf("We've got an empty string...exit..");
        }
        

        // чтение строки
        shmdt(shm_ptr);
        semop(semid, &signal_write,1);
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