#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define MAX_WORD_LENGTH 100

int loadDictionary(char *filename, char dictionary[][MAX_WORD_LENGTH]);
void checkSpelling(char *shm_ptr, int *numWords, char dictionary[][MAX_WORD_LENGTH]);
void strcat(char *str, char dest[100]);

int main() {

    // Reading dictionary file into memory
    char dictionary[10000][MAX_WORD_LENGTH];
    char fileName[] = "dictionary.txt";
    int numWords = loadDictionary(fileName, dictionary);

    key_t key=221669;
    int shmid = shmget(key, sizeof(char) * 100, 0666);
    if (shmid == -1){
        perror("shmget");
        exit(1);
    }
    char *shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1){
        perror("shmat");
        exit(1);
    }

    checkSpelling(shm_ptr, &numWords, dictionary);
    
    return 0;
}

int loadDictionary(char *filename, char dictionary[][MAX_WORD_LENGTH]){

    // Open the dictionary file
    FILE *dictionaryFile = fopen("./dictionary.txt", "r");
    if (dictionaryFile == NULL) {
        printf("Error opening dictionary file.\n");
        return 1;
    }

    // Read the dictionary file into memory
    int numWords = 0;
    while (fgets(dictionary[numWords], MAX_WORD_LENGTH, dictionaryFile) != NULL){
        // Remove the newline character from each word
        dictionary[numWords][strcspn(dictionary[numWords], "\n")] = '\0';
        numWords++;
    }

    // Close the dictionary file
    fclose(dictionaryFile);
    return numWords;
}

void checkSpelling(char *shm_ptr, int *numWords, char dictionary[][MAX_WORD_LENGTH]){

    // Reading from shared memory
    char inputString[100];
    strcpy(inputString, shm_ptr);
    char str[100];

    // Spliting the string words
    char *word = strtok(inputString, " \n");
    while (word != NULL) {
        // Check if the word is in the dictionary
        int found = 0;
        for (int i = 0; i < *numWords; i++) {
            if (strcmp(word, dictionary[i]) == 0) {
                found = 1;
                break;
            }
        }

        if (!found)
            strcat(word,str);
        word = strtok(NULL, " \n");     //next word
    }
    sprintf(shm_ptr, str);
}

void strcat(char *str, char dest[100]){
    int index = 0;
    static int dest_index=0;
    while(str[index] != '\0'){
        dest[dest_index] = str[index];
        index++;
        dest_index++;
    }
    dest[dest_index++] = ' ';
}
