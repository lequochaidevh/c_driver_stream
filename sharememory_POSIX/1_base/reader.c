// reader.c
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include "shared_memory.h"

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);

    shared_data_t *data = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t *sem = sem_open(SEM_NAME, 0);

    for (int i = 0; i < 10; i++) {
        sem_wait(sem);

        printf("Read: counter=%d, msg=%s\n", data->counter, data->message);

        sem_post(sem);
        sleep(1);
    }

    munmap(data, sizeof(shared_data_t));
    close(shm_fd);
}
