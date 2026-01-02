#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "ipc.h"

void channel_send(channel_t *ch, char c);
char channel_recv(channel_t *ch);

int main() {
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);

    shm_ipc_t *shm = mmap(NULL, sizeof(shm_ipc_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("Process B started\n");

    while (1) {
        char c = channel_recv(&shm->a2b);
        printf("B <- A: %c\n", c);

        char reply = c - 32;  // lowercase -> uppercase
        printf("B -> A: %c\n", reply);
        channel_send(&shm->b2a, reply);
        if (c == 'z') break;
    }
    munmap(shm, sizeof(shm));
    close(fd);
    shm_unlink(SHM_NAME);
}
