#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // Necessário para usleep

int saldo = 100;
int turn = 0;
int wants[2] = {0, 0};

void* depositar(void* arg) {
    int task = *(int*)arg;
    int valor = (task == 0) ? 50 : 30; // Verifica a thread com seu valor
    
    // Mostra interesse e cede o turno
    int other = 1 - task;
    wants[task] = 1;
    turn = other;
    while ((turn == other) && wants[other]); // Espera até que o turno e o interesse mudem
    
    // Seção Critica
    int temp = saldo;        // Passo 1: Leitura
    
    // Estou forçando a troca de contexto aqui:
    // A thread pausa por alguns microssegundos, 
    // dando chance para a outra thread ler o mesmo saldo original.
    usleep(100); 
    
    temp += valor;           // Passo 2: Soma
    saldo = temp;            // Passo 3: Escrita
    
    wants[task] = 0; // Desfaz o interesse
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int task0 = 0, task1 = 1;
    
    pthread_create(&t1, NULL, depositar, &task0);
    pthread_create(&t2, NULL, depositar, &task1);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Saldo final esperado: 180\n");
    printf("Saldo final obtido: %d\n", saldo);
    return 0;
}
