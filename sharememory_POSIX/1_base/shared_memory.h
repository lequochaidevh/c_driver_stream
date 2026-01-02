// shared.h
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_sem"

#define SHM_SIZE 256

typedef struct {
    int  counter;
    char message[128];
} shared_data_t;

#endif