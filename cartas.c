#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int TOTAL_DE_CARTAS = 40;

/**
 *
 * Esta función crea un array de enteros de 3 dígitos usando el lugar de la centena para codificar el tipo de palo
 * (copa, oro, basto, espada) y las otras 2 posiciones restantes para el número de la carta. Así, el 101 corresponde
 * al 1 de copa, el 306 al 6 de basto y el 412 al 12 de espada.
 * 
 * Luego de crear el arreglo, se le aplica un algoritmo de aleatorización de forma tal que cada vez que se llame a esta
 * función, las cartas nunca estarán en la misma posición. 
 *
 * @return array de 40 cartas mezcladas en orden aleatorio. 
 */

int *getMazo () {

    int *mazoDeCartas = (int*) malloc(TOTAL_DE_CARTAS * sizeof (int));
    int index = 0;
    int paloDeCarta;
    int numeroDeCarta;

    /**
     *
     * Pasamos las cartas a valores en centena, y guarda los valores en el array "mazoDeCartas" si y solo si el numero de la carta es 
     * distinto de 8 y 9. Cuando la variable "numeroDeCarta" es igual a 8 o 9, ignora la instruccion de guardarse en el array y continua 
     * con la siguiente iteracion del for.
     * 
     */

    for (paloDeCarta = 1; paloDeCarta <= 4; ++paloDeCarta) {
        
        for (numeroDeCarta = 1; numeroDeCarta <= 12; ++numeroDeCarta) {

            if (numeroDeCarta == 8 || numeroDeCarta == 9)
            continue; 

        }

        mazoDeCartas[index] = paloDeCarta * 100 + numeroDeCarta;
        index ++;

    }

    /**
     * 
     * Para aleatorizar las cartas, utilizamos el siguiente algoritmo que consiste en cambiar de posicion los valores dentro del array de
     * forma totalmente aleatoria.
     * 
     * Para que cada vez que se ejecute el programa no nos quede ordenado con la misma secuencia que la ejecucion anterior, 
     * llamamos a la funcion "srand ()" de tal manera que cada vez que tenga que mezclar, genere otro valor inicial (semilla).
     * De esta forma, nos aseguramos que nunca se va a repetir el orden por cada ejecucion que tenga el programa.
     * 
     */

    srand(time(NULL)); 
    size_t i;

    for (i = 0; i < TOTAL_DE_CARTAS - 1; i++) {
        
        size_t j = i + rand() / (RAND_MAX / (TOTAL_DE_CARTAS - i) + 1);
        int t = mazoDeCartas[j];
        mazoDeCartas[j] = mazoDeCartas[i];
        mazoDeCartas[i] = t;

    }

    return mazoDeCartas;

}

/**
 * 
 * Creamos una funcion 
 *  
*/

int 