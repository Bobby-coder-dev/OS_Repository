package org.example;
import java.util.concurrent.Semaphore; 

public class ExemploSemaforo {
    static int NUM_THREADS = 100;
    static int NUM_STEPS = 100000;
    static int sum = 0;
    static Semaphore semaphore = new Semaphore(1); // Semaphore(i); i = threads per critical section

    static class ThreadBody extends Thread {    
        int id;

        ThreadBody(int id) {
            this.id = id;
        }

        //Seção crítica 

        public void run() {
            for (int i = 0; i < NUM_STEPS; i++) {
                boolean acquired = false;
                try {
                    semaphore.acquire();s
                    acquired = true;
                    sum += 1;
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    if (acquired) {
                        semaphore.release();
                    }
                }
            }
        }
    }
    public static void main(String[] args) {
        ThreadBody[] threads = new ThreadBody[NUM_THREADS];

        for (int i = 0; i < NUM_THREADS; i++) {
            threads[i] = new ThreadBody(i);
            threads[i].start();
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("Valor final: " + sum);
        System.out.println("Valor esperado: " + NUM_THREADS * NUM_STEPS);
        System.out.println("Diferença: " + (NUM_THREADS * NUM_STEPS - sum));

        
    }
}
