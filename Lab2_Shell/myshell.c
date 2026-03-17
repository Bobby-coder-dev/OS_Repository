#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 256

void func_cwd(char *buf, size_t buffersize){
    if (getcwd(buf, buffersize) == NULL) {
        perror("cwd erro");
    }
} 

void func_cd(char *parametro){
    if(chdir(parametro) != 0){
        perror("Erro ao mudar de diretório");
    }
}

void func_mkdir(char *parametro){
    if(mkdir(parametro, 0700) != 0){ 
        perror("Erro ao criar diretório");
    }
}

void func_rmdir(char *parametro){
    if(rmdir(parametro) != 0){
        perror("Erro ao excluir diretório");
    }
}

void func_stat(char *parametro){
    struct stat st_local; 
    if (stat(parametro, &st_local) == 0) {
        printf("Tamanho: %ld bytes\n", st_local.st_size);
        printf("ID do Usuário (UID): %d\n", st_local.st_uid);

        printf("Tipo: %s\n", S_ISDIR(st_local.st_mode) ? "Diretório" : "Arquivo comum");
    } else {
        perror("Erro no stat");
    }
}

void func_ls(char *path){
    char *alvo = (path == NULL) ? "." : path; 
    DIR *dir = opendir(alvo);
    struct dirent *entrada_dir;

    if (dir == NULL) {
        perror("Erro ao abrir diretório");
        return;
    }


    while ((entrada_dir = readdir(dir)) != NULL) {
        printf("%s  ", entrada_dir->d_name);
    }
    printf("\n");
    closedir(dir);
}

int main(int argc, char **argv){
    char input[SIZE];
    char *cmd, *arg; 

    while(1){
        if (fgets(input, SIZE, stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = 0; 

        cmd = strtok(input, " "); 
        arg = strtok(NULL, " "); 

        if(cmd == NULL) continue; 

        if(strcmp(cmd, "exit") == 0) break;

        if(strcmp(cmd, "ls") == 0){ 
            func_ls(arg);
            continue;
        }

        if(strcmp(cmd, "cwd") == 0){ 
            char buf[SIZE];
            func_cwd(buf, sizeof(buf));
            printf("%s\n", buf);
            continue;
        }

        if(strcmp(cmd, "cd") == 0){ 
            if(arg != NULL) func_cd(arg);
            else printf("Uso: cd <diretorio>\n");
            continue;
        }

        if(strcmp(cmd, "mkdir") == 0){ 
            if(arg != NULL) func_mkdir(arg);
            else printf("Uso: mkdir <diretorio>\n");
            continue;
        }

        if(strcmp(cmd, "rmdir") == 0){ 
            if(arg != NULL) func_rmdir(arg);
            else printf("Uso: rmdir <diretorio>\n");
            continue;
        }

        if(strcmp(cmd, "stat") == 0){ 
            if (arg != NULL) func_stat(arg);
            else printf("Uso: stat <arquivo/diretorio>\n");
            continue;
        }
        
        printf("Comando desconhecido: %s\n", cmd);
    }
    return 0;
}
