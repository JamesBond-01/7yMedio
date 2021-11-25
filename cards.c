#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Tamanio total del mazo de cartas.
const int CARDS_STACK_SIZE = 40;

/**
 * Esta funcion crea un array de enteros de 3 digitos usando el lugar de la centena para codificar el tipo de palo
 * (copa, oro, basto, espada) y las otras 2 posiciones restantes para el numero de la carta. Asi, el 101 corresponde
 * al 1 de copa, el 306 al 6 de basto y el 412 al 12 de espada.
 *
 * Luego de crear el arreglo, se le aplica un algoritmo de aleatorizacion de forma tal que cada vez que se llame a esta
 * funcion, las cartas nunca estaran en la misma posicion.
 *
 * @return array de 40 cartas mezcladas (ordenadas aleatoriamente)
 */
int *getCardsStack() {
	int *cardStack = (int*) malloc(CARDS_STACK_SIZE * sizeof(int));
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

	//Para mezclar las cartas, utilizamos el siguiente algoritmo de shuffle que consiste en cambiar de posicion los valores
	//dentro del array de forma totalmente aleatoria.
	//
	//Para que cada vez que se ejecute el programa las cartas no nos queden ordenadas de la misma forma que la ejecucion
	//anterior, llamamos a la funcion srand() de tal manera que cada vez que se tenga que mezclar, genere otro valor
	//inicial (semilla). De esta forma, nos aseguramos que nunca se va a repetir el orden por cada ejecucion del programa.

	srand(time(NULL));
	int j, r1, r2, temp;
	for (j = 1; j <= 100; j++) {
		r1 = rand() % 40;
		r2 = rand() % 40;
		temp = cardStack[r1];
		cardStack[r1] = cardStack[r2];
		cardStack[r2] = temp;
	}

	return cardStack;
}

/**
 * Recorremos el mazo de cartas desde la ultima posicion hasta la primera. Cuando encontramos una carta que no tenga un
 * 0 en su posicion, le asignamos un 0 a esa posicion y la devolvemos. De esta forma, la proxima vez que se llame a la
 * funcion, dicha carta no estara mas disponible.
 *
 * Nota:
 * Se hace de esta forma y no al reves porque por algun motivo el algoritmo de shuffle en getCardsStack() devuelve
 * durante un mismo minuto la misma primer carta. Sospechamos que esto se debe a la funcion srand() o rand().
 *
 * @param cardStack array de 40 cartas
 * @return carta elegida
 */
int popCard(int *cardStack) {
	size_t i;
	for (i = CARDS_STACK_SIZE - 1; i >= 0; i--) {
		if (cardStack[i] == 0) {
			continue;
		}

		int card = cardStack[i];
		cardStack[i] = 0;
		return card;
	}

	return 0;
}

/**
 * @param card la carta de la cual queremos obtener su tipo
 * @return el tipo de carta de forma numeral, por ej.: 1 representa Copa, 2 Oro, 3 Basto y 4 Espada
 */
int getCardType(int card) {
	return (int) (card / 100);
}

/**
 * @param card la carta de la cual queremos obtener su numero
 * @return el numero de la carta
 */
int getCardNumber(int card) {
	return (card - getCardType(card) * 100);
}

/**
 * @param card la carta de la cual queremos obtener su representacion literal
 * @return una representacion literal de la carta, por ej.: 9 de Copa
 */
char *getCardString(int card) {
	int cardNumber = getCardNumber(card);
	int cardType = getCardType(card);
	char *cardTypeString;
	switch (cardType) {
	case 1:
		cardTypeString = "Copa";
		break;
	case 2:
		cardTypeString = "Oro";
		break;
	case 3:
		cardTypeString = "Basto";
		break;
	case 4:
		cardTypeString = "Espada";
		break;
	default:
		printf("Error en la generacion de cartas: tipo no reconocido: %i", cardType);
		cardTypeString = "No reconocido";
	}

	//12 es el numero total que puede tener la cadena de texto mas larga, ej: 12 de Espada
	//Pero con 12 habia casos donde se imprimia basura. Agregando uno mas se arreglo.
	size_t size = 13 * sizeof(char);
	char *cardString = (char*) malloc(size);
	snprintf(cardString, size, "%i de %s", cardNumber, cardTypeString);

	return cardString;
}

/**
 * Las cartas valen tantos puntos como su valor numerico indica, por ejemplo, el 4 de copa vale 4, el 7 de oro
 * vale 7, el 1 de basto vale 1. La excepcion a esto son las figuras (sota(10), caballo(11), rey(12)) que valen
 * medio punto.
 *
 * @param card la carta de la cual queremos obtener su valor
 * @return 0.5 si la carta es 10, 11 o 12, de otra forma devuelve el valor intrinseco de la carta.
 */
double getCardPoints(int card) {
	int cardNumber = getCardNumber(card);
	if (cardNumber == 10 || cardNumber == 11 || cardNumber == 12) {
		return 0.5;
	}
	return cardNumber;
}
