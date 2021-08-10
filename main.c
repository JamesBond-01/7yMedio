#include "cards.h"
#include "inputs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {   
    const int MIN_PLAYERS = 1;
    const int MAX_PLAYERS = 5;
    const int MIN_ROUNDS = 2;
    const int MAX_ROUNDS = 4;
    const int MIN_BET = 100;            // Cantidad minima permitida para apostar.
    const int MAX_BET = 1500;           // Cantidad maxima permitida para apostar.


    int banksWallet = 100000;           // Monto con el que empieza la banca.
    int card, points, players, rounds, bet, maxBet;
 

    printf("\n=================================");
    printf("\nBienvenido al juego Siete y Medio");
    printf("\n=================================\n");

    printf("\nPara comenzar, por favor ingrese los siguientes datos:\n");

    char *playersQ = "Cantidad de jugadores con los que desee jugar";
    players = getUserInput(playersQ, MIN_PLAYERS, MAX_PLAYERS ); 

    int playersWallet[players];     // Billetera donde se almacena el dinero de cada jugador.
    int playersBet[players];        // Array que contiene la apuesta que hace cada jugador por ronda.
    int i, j;

    // Le asignamos a cada jugador su monto inicial ($5000).
    for (i = 0; i < players; i++) {
        playersWallet[i] = 5000; 
    }

    rounds = getUserInput("Cantidad de rondas que dura la partida", MIN_ROUNDS, MAX_ROUNDS);

    int *cardStack = getCardsStack();

    for (i = 1; i <= rounds; i++) {
        for (j = 0; j < players ; j++) {
            card = popCard(cardStack);
            getCardString(card);

            if (playersWallet[j] < MIN_BET) {
                printf ("Usted no posee saldo suficiente para apostar y en consecuencia no podra seguir jugando");
                continue;
            }
            maxBet = MAX_BET <= playersWallet[j] ? MAX_BET : playersWallet[j];
            bet = getUserInput("Ingrese su apuesta", MIN_BET, maxBet);  
            playersWallet[j] = playersWallet[j] - bet;

            printf("\nRonda %d, jugador %d aposto %d, saldo %d",i, j+1, playersBet[j], playersWallet[j]);
        }
    }  
    return 0;   
}


