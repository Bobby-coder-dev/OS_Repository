// gcc -Wall copy.c -o copy
// strace ./copy arquivo.txt
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 128

int myopen(const char *filename, int flags, size_t buffersize){
    //return open(filename, flags);
    int fd = 0;

    // r: 64 bits | d: 32 bots
    //movimentação | parâmetros saída | parâmetros entrada

    //return open(filename, flags);

    __asm__("mov %0, %%rdi" : : "r"(filename));
    __asm__("mov %0, %%esi" : : "r"(flags));
    __asm__("mov %0, %%rdx" : : "r"(buffersize));
    __asm__("mov $2, %rax");
    __asm__("syscall");
    __asm__("mov %%eax, %0" : "=r"(fd) :);

    return fd;
}

int myread(int fd, void *buff, size_t count){
    //return read(fd, buff, count);
    __asm__("mov %0, %%edi" : : "r"(fd));
    __asm__("mov %0, %%rsi" : : "r"(buff));
    __asm__("mov %0, %%rdx" : : "r"(count));
    __asm__("mov $0, %rax");
    __asm__("syscall");
    __asm__("mov %%eax, %0" : "=r"(fd) :);
    return fd;
}

int mywrite(int fd, void *buff, size_t count){
    //return write(fd, buff, count);

    __asm__("mov %0, %%edi" : : "r"(fd));
    __asm__("mov %0, %%rsi" : : "r"(buff));
    __asm__("mov %0, %%rdx" : : "r"(count));
    __asm__("mov $1, %rax");
    __asm__("syscall");
    __asm__("mov %%eax, %0" : "=r"(fd) :);
    return fd;
}

int myclose(int fd){
    __asm__("mov %0, %%edi" : : "r"(fd));
    __asm__("mov $3, %rax");
    __asm__("syscall");
    __asm__("mov %%eax, %0" : "=r"(fd) :);
    return fd;
}

int main(int argc, char **argv){
    int fd; // file descriptor
    char buff[SIZE];
    ssize_t readCount; // Posição do arquivo que está sendo lida atualmente
    
    if(argc != 2){
        fprintf(stderr,"Uso correto: %s <nome_do_arquivo>\n", argv[0]);
        return 1;
    }
    
    fd = myopen(argv[1], O_RDONLY, SIZE);

    if (fd < 0){
        perror("file open");
        return 1;
    }

    while((readCount = myread(fd, buff, SIZE)) > 0){
        if((mywrite(STDOUT_FILENO, buff, readCount)) != readCount){
            perror("write");
            return 1;
        }
    }

    myclose(fd);

    return 0;
}
