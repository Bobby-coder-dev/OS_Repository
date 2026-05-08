#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // Necessário para usleep

int saldo = 100;
int turn = 1;

void* depositar(void* arg) {
    int id = *(int*)arg; //Atribui ao respectivo ID
    int valor = (id == 1) ? 50 : 30;
    
    while(turn != id); //Espera por vez
    
    // Seção critica
    int temp = saldo;        // Passo 1: Leitura
    // Estou forçando a troca de contexto aqui:
    // A thread pausa por alguns microssegundos, 
    // dando chance para a outra thread ler o mesmo saldo original.
    usleep(100); 
    temp += valor;           // Passo 2: Soma
    saldo = temp;            // Passo 3: Escrita
    
    if(id == 1) turn++; // Quando a primeira thread terminar passa para a proxima
    else turn = 0; // Encerra as vezes
    
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;
    
    pthread_create(&t1, NULL, depositar, &id1);
    pthread_create(&t2, NULL, depositar, &id2);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Saldo final esperado: 180\n");
    printf("Saldo final obtido: %d\n", saldo);
    return 0;
}
