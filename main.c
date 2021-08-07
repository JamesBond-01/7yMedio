#include <cards.h>
#include <stdio.h>

int main () {

    //Monto con el que empieza la banca.
    int banksWallet = 100000;

    printf ("\n=================================");
    printf ("\nBienvenido al juego Siete y Medio");
    printf ("\n=================================\n");

    int players, rounds;
    printf ("\nPara comenzar, por favor ingrese los siguientes datos:\n");

    printf ("\nCantidad de jugadores con los que desee jugar [1 - 5]: ");
    scanf ("%d", &players);
    
    while (players < 1 || players > 5) {
        printf ("\nPor favor, ingrese un valor correcto [1 - 5]: ");
        scanf ("%d", &players);
    }

    int playersWallets[players];
    int playersBet[players];
    size_t i, j;

    //Le asignamos a cada jugador su monto inicial.
    for (i = 0; i <= players - 1; i++) {
        playersWallets[i] = 5000;
    }

    printf ("\nCantidad de rondas de duracion de la partida [2 - 4]: ");
    scanf ("%d", &rounds);

    while (rounds < 2 || rounds > 4) {
        printf ("\nPor favor, ingrese un valor correcto [2 - 4]: ");
        scanf ("%d", &rounds);
    }

    for (i = 1; i <= rounds; i++) {
        for (j = 0; j <= players - 1; j++) {
            printf ("\nJugador %d, ingrese el monto que desee apostar [$150 - $1500]: ", (j+1));
            scanf ("%d", &playersBet[j]);

            while (playersBet[j] < 150 || playersBet[j] > 1500) {
                printf ("\nPor favor, ingrese un valor correcto [$150 - $1500]: ");
                scanf ("%d", &playersBet[j]);  
            }

            playersWallets[j] = playersWallets[j] - playersBet[j];
        }

    }
    
    return 0;
}