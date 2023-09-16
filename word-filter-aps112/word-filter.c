#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

int main(void) {
    // two components of this project: 
    // - take input and compare to a list of banned words
    // - or add to the list of banned words

    // first, introduce the program to the user and take the mode
    char simulateMode = 'W';
    printf("Welcome to the simulation program for Sentry, the word filter algorithm!\n");
    bool continueProgram = true;
    bool swearDetected = false;
    int codeProbability = 0;

    char *listOfSwears[30] = {
        "duck", // 0
        "witch",
        "lock",
        "bass",
        "ship", // 4
        "bullship",
        "tussy",
        "custard",
        "dunkey",
        "idiom", // 9
        "stupid",
        "stud",
        "kill",
        "smash",
        "maggot", // 14
        "mothertrucker" // index 15, 16th entry
        }; // a ragged array of pointers to "string" stype which contains swear words to censor.
    int swearBank = 0; // swearBank represents the number of banned words
    // count an actual number for swearBank here

    char *listOfSym[10] = {
        "$", 
        "3",
        "1",
        "|",
        "@",
        "0",
        "+",
        "*",
        "4",
        "!"
    };

    while (continueProgram = true) {
        printf("To test the word filter, enter W. To add to the list of banned words, enter B (Enter S to stop).\n");
        
        scanf(" %c", &simulateMode);
        if (simulateMode == 'S') {
            continueProgram = false;
            printf("Terminating program...");
            break;
        }

        char buffer = getchar();

        char userInput[50];

        int iterable = 0;

        swearBank = 0;

        while (listOfSwears[iterable] != NULL) {
            swearBank++;
            iterable++;
        }

        // int itest = 0;
        // while(listOfSwears[itest] != NULL) {
        //     printf("%s\n", listOfSwears[itest]);
        //     itest++;
        // }

        if (simulateMode == 'W') { // begin code for comparing input to list
            int riskScore = 0;
            printf("\n");
            printf("Beginning word filter simulation...\n");
            printf("Enter your message here: ");
            fgets(userInput, sizeof(userInput), stdin);
            for (int i = 0; i < swearBank; i++) {
                if (strstr(userInput, listOfSwears[i]) != NULL) {
                    swearDetected = true;
                }
            }
            for (int j = 0; j < 10; j++) {
                if (strstr(userInput, listOfSym[j]) != NULL) {
                    riskScore++;
                }
            }

            // case for when the last character is an exclamation
            int jterable = 0;
            while (userInput[jterable+2] != NULL) {
                jterable++;
            }
            
            if (userInput[jterable] == '!') {
                riskScore--;
            }

            if (swearDetected == true) {
                printf("Your message was blocked because we detected inappropriate language in your message.\n");
            } else {
                printf("User: %sThe message has a risk score of %d\n", userInput, riskScore);
            }
            swearDetected = false;
            
        } else if (simulateMode == 'B') {
            char newWord[20];
            printf("\n");
            printf("Expanding the filter's vocabulary\n");
            printf("Enter your new word here: ");
            scanf("%s", &newWord);
            listOfSwears[swearBank] = newWord;
            // printf("test: %s is assigned at listOfSwears[%d] now using %s", listOfSwears[swearBank], swearBank, newWord);
        }

        printf("\n");
    }

    return 0;
}