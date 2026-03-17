#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int pid, pid2, status;

    pid = fork();

    if (pid == -1) {
        perror("Fork falhou!!");
        exit(-1);
    } 
    else if (pid == 0) {
        pid2 = fork(); 

        if (pid2 == -1) {
            perror("Fork falhou!!");
            exit(-1);
        } 
        else if (pid2 == 0) {
            sleep(1);
            printf("Eu sou o processo %d (C), filho de %d\n", getpid(), getppid());
            exit(0);
        } 
        else {
            
            wait(&status); 
            sleep(1);
            printf("Eu sou o processo %d (B), filho de %d\n", getpid(), getppid());
            exit(0);
        }
    } 
    else {
        wait(&status);
        sleep(1);
        printf("Eu sou o processo %d (A), filho de %d\n", getpid(), getppid());
        exit(0);
    }
}