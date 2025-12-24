#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

pid_t fork(void) {
    pid_t pid = syscall(SYS_fork);
    if (pid < 0) {
        errno = -pid;
        return -1;
    }
    return pid;
}
  
int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        printf("Hello from the child process!\n");
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        wait(NULL); // Wait for child to finish
        printf("Hello from the parent process!\n");
    }
    return 0;
}
