#include <cards.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

int main () {
    const int CARDS_STACK_SIZE = 40;     // Tamaño total del mazo de cartas.
    const int MIN_BET = 100;            // Cantidad minima permitida para apostar.
    const int MAX_BET = 1500;           // Cantidad minima permitida para apostar.    
    
    int banksWallet = 100000;           // Monto con el que empieza la banca.
    int card, points, players, rounds;
    int cardStack[CARDS_STACK_SIZE];     // Array que contiene el mazo de cartas.
 

    printf ("\n=================================");
    printf ("\nBienvenido al juego Siete y Medio");
    printf ("\n=================================\n");

    printf ("\nPara comenzar, por favor ingrese los siguientes datos:\n");

    printf ("\nCantidad de jugadores con los que desee jugar [1 - 5]: ");
    scanf ("%d", &players);
    
    while (players < 1 || players > 5) {
        printf ("\nPor favor, ingrese un valor correcto [1 - 5]: ");
        scanf ("%d", &players);
    }

    int playersWallet[players];     // Billetera donde se almacena el dinero de cada jugador.
    int playersBet[players];        // Array que contiene la apuesta que hace cada jugador por ronda.
    size_t i, j, r1, r2, temp;

    // Le asignamos a cada jugador su monto inicial ($5000).
    for (i = 0; i <= players - 1; i++) {
        playersWallet[i] = 5000; 
    }

    printf ("\nCantidad de rondas de duracion de la partida [2 - 4]: ");
    scanf ("%d", &rounds);

    while (rounds < 2 || rounds > 4) {
        printf ("\nPor favor, ingrese un valor correcto [2 - 4]: ");
        scanf ("%d", &rounds);
    }

    //genero maso
    int index = 0;
    int cardType, cardNumber;
    for (cardType = 1; cardType <= 4; ++cardType) {
        for (cardNumber = 1; cardNumber <= 12; ++cardNumber) {
            if (cardNumber == 8 || cardNumber == 9) {
                continue;
            }
            cardStack[index] = cardType * 100 + cardNumber;
            index++;
        }
    }

    // Inicio ronda
    for (i = 1; i <= rounds; i++) {
        //mezclo mazo
        srand(time(NULL));
        for (j = 1; j <= 100; j++) {
            r1 = rand() % 40;
            r2 = rand() % 40;
            temp = cardStack[r1];      
            cardStack[r1] = cardStack[r2];
            cardStack[r2] = temp;
        }

        //inicio apuestas
        for (j = 0; j <= players - 1; j++) {

            printf ("\nApuesta jugador %d [%d - %d]: ", (j+1), MIN_BET, MAX_BET);
            scanf ("%d", &playersBet[j]);

            while (playersBet[j] < MIN_BET || playersBet[j] > MAX_BET) {
                printf ("\nApuesta valida entre %d - %d: ", MIN_BET, MAX_BET);
                scanf ("%d", &playersBet[j]);  
            }
            playersWallet[j] = playersWallet[j] - playersBet[j];

            printf("\nRonda %d, jugador %d aposto %d, saldo %d",i, j+1, playersBet[j], playersWallet[j]);

        } //fin apuestas

    } //fin ronda 

    return 0;   
}