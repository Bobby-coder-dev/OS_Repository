#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 256

extern char **environ;

int main() {
    char input[SIZE];
    pid_t pid;

    while (1) {
        printf("> ");
        if (fgets(input, SIZE, stdin) == NULL) break;
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "sair") == 0) break;

        // verificar se deve rodar em background 
        int background = 0;
        int len = strlen(input);
        if (len > 0 && input[len - 1] == '&') {
            background = 1;
            input[len - 1] = '\0'; // remove o &
        }

        pid = fork();

        if (pid < 0) {
            perror("Erro no fork");
        } 
        else if (pid == 0) {
            //Processo filho
            char *args[] = {input, NULL}; 

            printf("[Filho] Executando: %s\n", input);
            
            // Substitui o código atual pelo programa do caminho informado
            if (execve(input, args, environ) == -1) {
                perror("Erro ao executar");
                exit(EXIT_FAILURE);
            }
        } 
        else {
            //Processo pai
            if (background) {
                printf("[Pai] Rodando em background (PID: %d)\n", pid);
            } else {
                wait(NULL); // Espera o filho terminar
            }
        }
    }
    return 0;
}