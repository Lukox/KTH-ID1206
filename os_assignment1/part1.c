#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int f_dir[2];
    pipe(f_dir);

    pid_t pid = fork();

    if (pid == -1) {
        exit(1);
    }
    else if (pid == 0) {
        dup2(f_dir[1], 1);
        close(f_dir[0]);
        execlp("ls", "ls", "/", NULL);
    }
    else {
        dup2(f_dir[0], 0);
        close(f_dir[1]);
        execlp("wc", "wc", "-l", NULL);
    }

    return 0;
}