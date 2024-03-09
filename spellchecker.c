#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100

int main() {
    // Open the dictionary file
    FILE *dictionaryFile = fopen("./dictionary.txt", "r");
    if (dictionaryFile == NULL) {
        printf("Error opening dictionary file.\n");
        return 1;
    }

    // Read the dictionary file into memory
    char dictionary[10000][MAX_WORD_LENGTH];
    int numWords = 0;
    while (fgets(dictionary[numWords], MAX_WORD_LENGTH, dictionaryFile) != NULL) {
        // Remove the newline character from each word
        dictionary[numWords][strcspn(dictionary[numWords], "\n")] = '\0';
        numWords++;
    }

    // Close the dictionary file
    fclose(dictionaryFile);

    // Get the string from the user
    char inputString[MAX_WORD_LENGTH];
    printf("Enter a string: ");
    fgets(inputString, MAX_WORD_LENGTH, stdin);

    // Split the string into individual words
    char *word = strtok(inputString, " \n");
    while (word != NULL) {
        // Check if the word is in the dictionary
        int found = 0;
        for (int i = 0; i < numWords; i++) {
            if (strcmp(word, dictionary[i]) == 0) {
                found = 1;
                break;
            }
        }

        // Print the result
        if (found){
            printf("%s is spelled correctly.\n", word);
        }
        else{
            printf("%s is misspelled.\n", word);
        }

        // Get the next word
        word = strtok(NULL, " \n");
    }

    return 0;
}