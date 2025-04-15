#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child - rectangle  
        execl("./child.out", NULL);
    }
    else {
        // Parent triangle
        int length = 9;
        int height = 4;

        /*printf("input triangle params\n");*/
        /*scanf("triangle length = %d\n", &length);*/
        /*scanf("triangle height = %d\n", &height);*/

        int S = length * height / 2;
        printf("S of triangle: %d\n", S);
        wait(NULL); // Wait for child process to finish
    }

    return 0;
}