#ifndef COMMON_DATA
#define COMMON_DATA

#define PROJ_ID 101

#define STR_LEN 128
#define S_EMPTY 10
#define S_CAN_READ 0
#define S_CAN_WRITE 1

#define BUFF_SIZE STR_LEN

#include <sys/sem.h>

//структуры сигналов семфаоров

struct sembuf wait_read = {0, -1, 0};
struct sembuf wait_write = {1, -1, 0};
struct sembuf signal_write = {1, 1 ,0};
struct sembuf signal_read = {0, 1, 0};

#endif