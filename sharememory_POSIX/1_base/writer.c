// writer.c
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include "shared_memory.h"

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(shared_data_t));

    shared_data_t *data = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    for (int i = 0; i < 10; i++) {
        sem_wait(sem);

        data->counter = i;
        snprintf(data->message, sizeof(data->message), "Hello from writer %d", i);

        sem_post(sem);
        sleep(1);
    }

    munmap(data, sizeof(shared_data_t));
    close(shm_fd);
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);
}
