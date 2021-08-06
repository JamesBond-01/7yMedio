#include <cards.h>
#include <stdio.h>

int main () {

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

    printf ("\nCantidad de rondas de duracion de la partida [2 - 4]: ");
    scanf ("%d", &rounds);

    while (rounds < 2 || rounds > 4) {
        printf ("\nPor favor, ingrese un valor correcto [2 - 4]: ");
        scanf ("%d", &rounds);
    }


    return 0;
}