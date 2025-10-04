#ifndef __SOCKET_HELPER__
#define __SOCKET_HELPER__

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h> // override signal interupt
#include <stdlib.h>

#include <sys/ioctl.h>
extern struct termios oldt;
char* get_message_and_jump_start_line(char *_speaker, char* catch_buffer, size_t buffer_size);

#endif // __SOCKET_HELPER__