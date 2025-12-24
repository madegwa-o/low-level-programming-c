#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#define NUM_CHILDREN 5
#define SEM_NAME "/my_semaphore"
sem_t *sem;

void* child_process(void* arg) {
    int id = *((int*)arg);
    free(arg);

    // Simulate some work before trying to enter critical section
    sleep(rand() % 3);

    printf("Child %d: Waiting to enter critical section...\n", id);
    sem_wait(sem);
    printf("Child %d: Entered critical section.\n", id);

    // Simulate work in critical section
    sleep(2);

    printf("Child %d: Exiting critical section.\n", id);
    sem_post(sem);

    return NULL;
}

int main() {
    pthread_t children[NUM_CHILDREN];
    srand(time(NULL));

    // Create a named semaphore with an initial value of 1
    sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Create child threads
    for (int i = 0; i < NUM_CHILDREN; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&children[i], NULL, child_process, id) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all child threads to finish
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pthread_join(children[i], NULL);
    }

    // Clean up semaphore
    sem_close(sem);
    sem_unlink(SEM_NAME);

    printf("All child processes have completed.\n");
    return 0;
}