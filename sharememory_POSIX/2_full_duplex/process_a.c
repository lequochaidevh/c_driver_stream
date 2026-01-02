#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include "ipc.h"

void channel_init(channel_t *ch);
void channel_send(channel_t *ch, char c);
char channel_recv(channel_t *ch);

int main() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(shm_ipc_t));

    shm_ipc_t *shm = mmap(NULL, sizeof(shm_ipc_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Init ONLY ONCE
    channel_init(&shm->a2b);
    channel_init(&shm->b2a);

    printf("Process A started\n");

    for (char c = 'a'; c <= 'z'; c++) {
        printf("A -> B: %c\n", c);
        channel_send(&shm->a2b, c);

        char reply = channel_recv(&shm->b2a);
        printf("A <- B: %c\n", reply);

        sleep(1);
    }

    munmap(shm, sizeof(shm));
    close(fd);
}
