#include <stdio.h>
#include <stdlib.h>

int getUserInput(char * userQuestion, int min, int max){
    int userInput;  
    int scanfResult = -1;
    do {
        printf(scanfResult == -1
            ? ("\n%s [%d - %d]: ", userQuestion, min, max)
            : ("\nEso no es correcto ! Intenta nuevamente [%d - %d]: ", min, max));
        scanfResult = scanf("%i", &userInput);
        if (scanfResult != 1) {
            getchar();
        }
    } while (userInput < min || userInput > max);

    return userInput;
    
}