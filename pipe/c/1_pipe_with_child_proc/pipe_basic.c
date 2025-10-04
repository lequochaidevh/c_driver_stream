#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
    int fd[2];                // fd[0]: read end, fd[1]: write end
    pid_t pid;
    char message[] = "Hello child";
    char buffer[100];

    // Create an anonymous pipe
    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // Fork a new process
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // --- Child process ---
        close(fd[1]);  // Close write end (child only reads)

        // Read message from parent
        read(fd[0], buffer, sizeof(buffer));

        printf("Child received: %s\n", buffer);

        close(fd[0]);  // Close read end
    } else {
        // --- Parent process ---
        close(fd[0]);  // Close read end (parent only writes)

        // Send message to child
        write(fd[1], message, strlen(message) + 1);

        close(fd[1]);  // Close write end
    }

    return 0;
}
