#include "cards.h"
#include "inputs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {   
    const int MIN_PLAYERS = 1;          // Cantidad minima permitida de jugadores.
    const int MAX_PLAYERS = 5;          // Cantidad maxima permitida de jugadores.
    const int MIN_ROUNDS = 2;           // Cantidad minima permitida de rondas.
    const int MAX_ROUNDS = 4;           // Cantidad maxima permitida de rondas.
    const int MIN_BET = 100;            // Cantidad minima permitida de apuestas.
    const int MAX_BET = 1500;           // Cantidad maxima permitida de apuestas.


    int banksWallet = 100000;           // Monto con el que empieza la banca.
    int card, players, rounds, bet, maxBet;
    double cardPoints;
    char option;
 

    printf("\n##################################");
    printf("\nBienvenido al juego Siete y Medio");
    printf("\n##################################\n");

    printf("\nPara comenzar, por favor ingrese los siguientes datos:\n");


    players = getUserInput("Cantidad de jugadores con los que desee jugar", MIN_PLAYERS, MAX_PLAYERS ); 

    double playerPoints[players];
    int playersWallet[players];     // Billetera donde se almacena el dinero de cada jugador.
    int playersBet[players];        // Array que contiene la apuesta que hace cada jugador por ronda.
    int i, j, p;                    //Contadores.

    // Le asignamos a cada jugador su monto inicial ($5000).
    for (i = 0; i < players; i++) {
        playersWallet[i] = 5000; 
    }

    rounds = getUserInput("Cantidad de rondas que dura la partida", MIN_ROUNDS, MAX_ROUNDS);

    for (i = 1; i <= rounds; i++) {

        int *cardStack = getCardsStack();   // Genero el array de cartas mezclado aleatoriamente.
        printf("\n\t========");
        printf("\n\tRonda %d\n", i);
        printf("\t========");
        
        for (j = 0; j < players ; j++) {

            /** 
             * Elijo una carta aleatoria y en su posicion dentro del array lo reemplazo por un 0 
             * para que no vuelva a aparecer en la ronda. 
             */ 
            card = popCard(cardStack);      

            //Si a un jugador le queda menos dinero que el minimo que se puede apostar, no puede jugar mas.
            if (playersWallet[j] < MIN_BET) {
                printf("Usted no posee saldo suficiente para apostar y en consecuencia no podra seguir jugando.");
                continue;
            }
            
            maxBet = MAX_BET <= playersWallet[j] 
                             ? MAX_BET 
                             : playersWallet[j];
            printf("\n*****Turno del jugador %d*****.", j+1);
            printf("\nHa recibido el %s.", getCardString(card));

            char numPly[25];
            sprintf(numPly,"Ingrese su apuesta");
            bet = getUserInput(numPly, MIN_BET, maxBet);  

            playersBet[j] = bet; 

            playersWallet[j] = playersWallet[j] - bet;
            printf("\t(Saldo %d)\n", playersWallet[j]);

           cardPoints = getCardPoints(card); 
           printf("\tPuntaje de la carta: %lf", cardPoints);

           do{
                printf("\nDesea pedir otra carta? [y/n]: ");
                scanf(" %c", &option);

                if (option == 'y' || option == 'Y') {
                    card = popCard(cardStack);      
                    printf("\nHa recibido el %s.", getCardString(card));
                    cardPoints = cardPoints + getCardPoints(card); 
                    printf("\n\tLleva acumulado %lf puntos.", cardPoints);

                    if (cardPoints > 7.5) {
                        printf("\nUsted perdio la ronda. Ha superado 7.5 :(\n");
                        banksWallet = banksWallet + bet;
                        break;
                    }
                }

            } while (option == 'y' || option == 'Y');

            playerPoints[j] = cardPoints; 
        }

        printf("\n*****Turno de la banca*****");
        card = popCard(cardStack);      
        printf("\nLa banca recibe el %s.", getCardString(card));
        cardPoints = getCardPoints(card);

        while (cardPoints <= 5.5) {
            card = popCard(cardStack);      
            printf("\nLa banca pide otra carta y recibe el %s.", getCardString(card));
            cardPoints = cardPoints + getCardPoints(card);
        } 
        
        if (cardPoints > 7.5) {
            printf("\nLa banca pierde: ha superado 7.5 puntos. Ganan los jugadores!\n");
            for (p = 1; p < players; p++) {
                playersWallet[p] = playersWallet[p] + playersBet[p];
            break;
            }
        }
        else if (cardPoints >= 6 && cardPoints <= 7.5) {
            printf("\nLa banca se planta. Ha acumulado %lf puntos.", cardPoints); 
        }        
            
         
    }  

    return 0;   
}


