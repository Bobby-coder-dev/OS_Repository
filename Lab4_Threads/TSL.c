#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int saldo = 100;
int lock = 0; 

int TSL(int *lock_ptr) {
    // old <- x
    // x <- 1
    // return old
    return __sync_lock_test_and_set(lock_ptr, 1); // Função atomica
}

void* depositar(void* arg) {
    int valor = *(int*)arg;
    
    while (TSL(&lock)); //Endereço de lock 
    
    // Seção Crítica
    int temp = saldo;
    usleep(100); 
    temp += valor;
    saldo = temp;
    
    lock = 0; // Destranca
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int v1 = 50, v2 = 30;
    
    pthread_create(&t1, NULL, depositar, &v1);
    pthread_create(&t2, NULL, depositar, &v2);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Saldo final esperado: 180\n");
    printf("Saldo final obtido: %d\n", saldo);
    return 0;
}
