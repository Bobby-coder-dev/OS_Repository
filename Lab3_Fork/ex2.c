#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int main(void){
    int pid, status;
    int resp = 1;
    int i = 5;

    while(i>0) {
        pid = fork();
        if(pid == -1){
            perror("Fork falhou!");
        } else if (pid == 0){
            resp *= i;
            return resp;
        } else {
            wait(&status);
            if (WIFEXITED(status)){
                i--;
                resp = WEXITSTATUS(status);
            }
        }
    }
    printf("%d! = %d\n", 5, resp);
    exit(0);
}