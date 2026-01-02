#pragma once
#include <stdint.h>
#include <semaphore.h>

#define SHM_NAME "/ipc_full_duplex_shm"
#define BUF_SIZE 256

typedef struct {
    uint32_t w;
    uint32_t r;
    char     buf[BUF_SIZE];

    sem_t empty;
    sem_t full;
    sem_t mutex;
} channel_t;

typedef struct {
    channel_t a2b;
    channel_t b2a;
} shm_ipc_t;
