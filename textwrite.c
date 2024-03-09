#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main() {
    int shmid;
    key_t key;
    char *shm;
    // Generate a unique key
    key = ftok("/home/deviant/Data/University/5-OS/Assignments/1/textwrite.c", 'R');
    // printf("Key: %d\n", key);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    // Create a shared memory segment
    shmid = shmget(key, sizeof(char) * 100, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    // Attach the shared memory segment
    shm = (char *)shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        perror("shmat");
        exit(1);
    }
    // Write data to the shared memory segment
    char str[100];
    printf("\nEnter the string to be checked (characters only): ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = 0;
    sprintf(shm, str);
    
    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        // Child process
        printf("Child process: %s\n", shm);
        exit(0);
    } else {
        // Parent process
        wait(NULL);
        printf("Parent process: %s\n", shm);
        // Detach the shared memory segment
        if (shmdt(shm) == -1) {
            perror("shmdt");
            exit(1);
        }
        // Remove the shared memory segment
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }
    }
    return 0;
}