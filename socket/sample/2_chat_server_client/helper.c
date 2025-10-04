#include "helper.h"

int get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

int count_wrapped_lines(const char* str1, const char* str2, int terminal_width) {
    int len2 = strlen(str2);
    int len1 = strlen(str1);
    return (len1 + len2) / terminal_width;
}

void clear_previous_lines(int lines) {
    for (int i = 0; i < lines; i++) {
        printf("\033[F");    // Move cursor up 1 line
        printf("\033[2K");   // Clear entire line
    }
}


struct termios oldt;

void handle_sigint(int sig) {
    // Reset terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n[!] Reset terminal sucessfully and exit.\n");
    exit(1);
}

int terminal_setup() {
    signal(SIGINT, handle_sigint);
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt); // Save origin terminal mode
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Turn off canonical & echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply
    // setting terminal: turn off canonical + echo

}

char* get_message_and_jump_start_line(char* _speaker, char* catch_buffer, size_t buffer_size) {
    //char* orgin_ptr = catch_buffer;
    static char input[256];
    int index = 0;
    char c;
    int terminal_width = get_terminal_width();

    char seq[3];

    terminal_setup();
    printf(" Input ---> ");
    fflush(stdout);

    // Get per char until get Enter
    while ((c = getchar()) != '\n' && index < sizeof(input) - 1) {
        if (c == 127 || c == 8) {  // 127 = DEL, 8 = BS
            if (index > 0) {
                index--;
                input[index] = '\0';
                catch_buffer[index] = '\0';

                // backspace and back fd before.
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }
        if (c == 27) { // ESC
            seq[0] = c;
            seq[1] = getchar(); //  '['
            seq[2] = getchar(); // 'A', 'B', 'C', 'D'

            if (seq[1] == '[') {
                switch (seq[2]) {
                    case 'A': // ↑
                    case 'B': // ↓
                    case 'C': // →
                    case 'D': // ←
                        // printf("[Filtered arrow key: %c]\n", seq[2]);
                        continue;
                }
            }
        }
        input[index] = c;
        catch_buffer[index] = c;
        index++;
        catch_buffer[index] = '\0';

        putchar(c); // echo
        fflush(stdout);
    }

    input[index] = '\0'; // End of string
    
    // Reset origin terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    int lines_used = count_wrapped_lines(" Input ---> ", catch_buffer, terminal_width);
    // Remove line before
    clear_previous_lines(lines_used);
    // Overidde present line.
    printf("\r                                              %s %s \n", _speaker, input);
    // catch_buffer -= index;
    memset(catch_buffer, '\0', buffer_size);
    return &input[0];
}