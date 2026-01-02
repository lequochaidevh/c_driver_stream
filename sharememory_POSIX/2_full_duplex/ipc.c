#include "ipc.h"

void channel_init(channel_t *ch) {
    ch->w = 0;
    ch->r = 0;

    sem_init(&ch->empty, 1, BUF_SIZE);
    sem_init(&ch->full, 1, 0);
    sem_init(&ch->mutex, 1, 1);
}

void channel_send(channel_t *ch, char c) {
    sem_wait(&ch->empty);
    sem_wait(&ch->mutex);

    ch->buf[ch->w % BUF_SIZE] = c;
    ch->w++;

    sem_post(&ch->mutex);
    sem_post(&ch->full);
}

char channel_recv(channel_t *ch) {
    char c;

    sem_wait(&ch->full);
    sem_wait(&ch->mutex);

    c = ch->buf[ch->r % BUF_SIZE];
    ch->r++;

    sem_post(&ch->mutex);
    sem_post(&ch->empty);

    return c;
}
