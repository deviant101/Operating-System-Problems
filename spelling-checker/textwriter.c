#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

char* create_attach_shared_memory(int *shmid);

int main(){
    
    int shmid;
    char *shm_ptr = create_attach_shared_memory(&shmid);

    // Write data to the shared memory segment
    char str[100];
    printf("\nEnter the string to be checked (characters only): ");
    fgets(str, sizeof(str), stdin);
    sprintf(shm_ptr, str);
    
    // Fork a child process
    pid_t pid = fork();
    if (pid == -1){
        perror("fork");
        exit(1);
    }
    if (pid == 0){
        // Detach shared memory segment from address space
        if (shmdt(shm_ptr) == -1){
            perror("shmdt");
            exit(1);
        }
        // Execute a new program
        execl("spellchecker", NULL);
        perror("execl");
        exit(1);
    }
    else{
        // Parent process
        wait(NULL);
        printf("\nEntered string : %s\n", str);
        printf("Spell errors : %s\n", shm_ptr);
        // Detach the shared memory segment
        if (shmdt(shm_ptr) == -1){
            perror("shmdt");
            exit(1);
        }
        // Remove the shared memory segment
        if (shmctl(shmid, IPC_RMID, NULL) == -1){
            perror("shmctl");
            exit(1);
        }
    }
    return 0;
}

char* create_attach_shared_memory(int *shmid){

    key_t key=221669;
    // Create a shared memory segment
    *shmid = shmget(key, sizeof(char) * 100, IPC_CREAT | 0666);
    if (*shmid == -1) {
        perror("shmget");
        exit(1);
    }
    // Attach the shared memory segment
    char *shm_ptr = (char *)shmat(*shmid, NULL, 0);
    if (shm_ptr == (char *)-1){
        perror("shmat");
        exit(1);
    }
    return shm_ptr;
}