#include "cards.h"
#include "inputs.h"
#include <stdio.h>

int main() {
	const int MIN_PLAYERS = 1;	//Cantidad minima permitida de jugadores.
	const int MAX_PLAYERS = 5;	//Cantidad maxima permitida de jugadores.
	const int MIN_ROUNDS = 2;	//Cantidad minima permitida de rondas.
	const int MAX_ROUNDS = 4;	//Cantidad maxima permitida de rondas.
	const double MIN_BET = 100;	//Cantidad minima permitida de apuestas.
	const double MAX_BET = 1500;	//Cantidad maxima permitida de apuestas.

	int card, players, rounds, currentCard, playerNumber, maxScore, profitWinner, cardNum, maxCardNum, temp, max;
	double benchPoints, bet, maxBet, benchBalance, totalBalance;
	bool option, secondCardPopped;

	int playersOut = 0;	//Contador para saber cuantos jugadores quedaron afuera en la partida.
	int sieteFigura = 0;	//Contador para saber cuantos jugadores lograron siete + figura.
	int benchStatus = 1;	//Variable para definir el estado de la banca (ganadora o perdedora). Valor por default 1 (ganadora).
	double maxProfit = 0;	//Variable para comparar ganancias.
	double benchWallet = 100000;	//Monto con el que empieza la banca.

	printf("\n================================================================");
	printf("\n\t\tBienvenido al juego Siete y Medio");
	printf("\n================================================================\n");

	printf("\nPara comenzar, por favor ingrese los siguientes datos:");

	//Determinamos la cantidad de jugadores que habra en la partida.
	players = getUserInputAsInt("\nCantidad de jugadores con los que desee jugar", MIN_PLAYERS, MAX_PLAYERS );

	double playerPoints[players];	//Array que contiene la cantidad de puntos de cada jugador.
	double playersWallet[players];	//Billetera donde se almacena el dinero de cada jugador.
	double playersBet[players];	//Array que contiene la apuesta que hace cada jugador por ronda.
	double playerGainRatio[players];//Array que contiene el porcentaje de ganancia por sobre la apuesta.
	double playersBalance[players];	//Array donde se guarda el playersBalance de cada jugador.
	int winners[players];		//Array que contiene los ganadores en caso de que la banca se plante.
	int cardsCount[12];		//Array para contabilizar los numeros de las cartas que salieron a lo largo de la partida.
	int indexes[] = {0,1,2,3,4,5,6,7,8,9,10,11};

	//Le asignamos a cada jugador su monto inicial ($5000).
	for (playerNumber = 0; playerNumber < players; playerNumber++) {
		playersWallet[playerNumber] = 5000;
	}

	//Inicializamos todos los valores del array en 0.
	for (int i = 0; i < 10; i++) {
		cardsCount[i] = 0;
	}

	//Determinamos la cantidad de rondas que habra en la partida.
	rounds = getUserInputAsInt("Cantidad de rondas que dura la partida", MIN_ROUNDS, MAX_ROUNDS);

	//Inicia la ronda.
	for (int roundNumber = 1; roundNumber <= rounds; roundNumber++) {

		//Genero el mazo de cartas mezclado aleatoriamente.
		int *cardStack = getCardsStack();

		printf("\n---------------------------------------------------------------");
		printf("\n\t\t\tRonda %d", roundNumber);
		printf("\n---------------------------------------------------------------\n");

		//Inicializamos el array de ganadores en cero.
		for (playerNumber = 0; playerNumber < players; playerNumber++) {
			winners[playerNumber] = 0;
		}

		//Turno de cada uno de los jugadores en la ronda.
		for (playerNumber = 0; playerNumber < players ; playerNumber++) {

			//Elijo una carta aleatoria y en su posicion dentro del array lo reemplazo por un 0 para que no vuelva a aparecer en la ronda.
			card = popCard(cardStack);
			playerPoints[playerNumber] = getCardPoints(card);

			/**
			 * Para saber que numero de carta salio mas en todo el juego, lo resolvemos de la siguiente manera:
			 * Tomo el valor numerico de la carta y a esa carta, le sumo 1 dentro del array cardsCount en esa posicion.
			 *
			 * Para hacer coincidir el numero de la carta con la posicion del array, hacemos la diferencia entre la posicion del array y el numero de la carta,
			 * y al resultado se lo sumo a i.
			 */
			cardNum = getCardNumber(card);
			for (int i = 0; i < 10; i++) {
				if ((i + 1) == cardNum) {
					cardsCount[i] = cardsCount[i] + 1;
				}
			}

			//Si a un jugador le queda menos dinero que el minimo que se puede apostar, no puede jugar mas.
			if (playersWallet[playerNumber] < MIN_BET) {
				printf("Usted no posee saldo suficiente para apostar y en consecuencia no puede seguir jugando.");
				continue;
			}

			printf("\nTurno del jugador %d", playerNumber + 1);
			printf("\n\tCarta recibida: %s", getCardString(card));

			//Elegimos el minimo valor entre el maximo permitido de apuesta (1500) y el saldo en la billetera del jugador.
			//El objetivo es mostrar en pantalla el maximo limite de apuesta. Es decir, si el jugador puede apostar menos
			//de 1500, se mostrara lo maximo que el jugador pueda apostar.
			maxBet = MAX_BET <= playersWallet[playerNumber]
					? MAX_BET
					: playersWallet[playerNumber];
			bet = getUserInputAsDouble("Ingrese su apuesta $", MIN_BET, maxBet);

			playersBet[playerNumber] = bet;

			//Le restamos a la billetera del jugador el monto apostado, y mostramos su saldo.
			playersWallet[playerNumber] = playersWallet[playerNumber] - bet;
			printf("\t(Saldo %.2f)", playersWallet[playerNumber]);
			printf("\t\tPuntaje de la carta: %.2f", playerPoints[playerNumber]);

			secondCardPopped = true;
			playerGainRatio[playerNumber] = 1; // Valor por defecto en caso de que no gane.

			//Preguntamos si quiere otra carta y evaluamos su puntaje total.
			do {
				option = getUserInputAsBool("\n\tDesea pedir otra carta?");
				if (option) {
					currentCard = popCard(cardStack);
					playerPoints[playerNumber] += getCardPoints(currentCard);
					printf("\tCarta recibida: %s", getCardString(currentCard));

					cardNum = getCardNumber(currentCard);
					for (int i = 0; i < 10; i++) {
						if ((i + 1) == cardNum) {
							cardsCount[i] = cardsCount[i] + 1;
						}
					}
					//Analisis de 7.5 puntos con 2 cartas.
					if (secondCardPopped) {
						secondCardPopped = false;

						int cardType1 = getCardType(card);
						int cardType2 = getCardType(currentCard);
						int cardNum1 = getCardNumber(card);
						int cardNum2 = getCardNumber(currentCard);

						if (playerPoints[playerNumber] == 7.5) {
							if ((cardType1 == 2 && cardType2 == 2) && (cardNum1 == 12 || cardNum2 == 12)) {
								playerGainRatio[playerNumber] = 2;	//Si gana con: 7 + figura (palo oro y figura rey), premio 100% de lo apostado.
							} else if (cardType1 == cardType2) {
								playerGainRatio[playerNumber] = 1.75;	//Si gana con: 7 + figura, del mismo palo, premio 75% de lo apostado
							} else {
								playerGainRatio[playerNumber] = 1.50;	//Si gana con: 7 + figura, el premio es 50% de lo apostado
							}
							printf("\nHa alcanzado 7.5 puntos con 2 cartas.\n");
							sieteFigura ++;
							break;
						}
					}
					//Analisis de 7.5 puntos con 3 o mas cartas.
					if (playerPoints[playerNumber] == 7.5){
						playerGainRatio[playerNumber] = 1.25;
						printf("\nHa alcanzado 7.5 puntos con 3 o mas cartas.\n");
						break;
					}

					//Si se pasa de 7.5 pierde la apuesta.
					if (playerPoints[playerNumber] > 7.5) {
						printf("\nUsted pierde la ronda. Ha superado 7.5. Total de puntos: %.2f\n", playerPoints[playerNumber]);
						playersOut ++;
						break;
					}
					printf ("\tJugador %d tiene %.2f puntos", playerNumber + 1, playerPoints[playerNumber]);
				}
			} while (option);
		}

		//Turno de la banca.
		printf("\nTurno de la banca");
		card = popCard(cardStack);
		cardNum = getCardNumber(card);
		for (int i = 0; i < 10; i++) {
			if ((i + 1) == cardNum) {
				cardsCount[i] = cardsCount[i] + 1;
			}
		}

		benchPoints = getCardPoints(card);
		printf("\n\tLa banca recibe el %s", getCardString(card));

		//Mientras el puntaje de la banca sea menor o igual a 5.5, pide otra carta.
		while (benchPoints <= 5.5) {
			card = popCard(cardStack);
			printf("\n\tLa banca pide otra carta y recibe el %s. ", getCardString(card));
			benchPoints += getCardPoints(card);
			printf("Puntos acumulados: %.2f", benchPoints);
			cardNum = getCardNumber(card);
			for (int i = 0; i < 10; i++) {
				if ((i + 1) == cardNum) {
					cardsCount[i] = cardsCount[i] + 1;
				}
			}
		}

		//Si la banca gana con 7.5, todos los jugadores pierden sus apuestas independientemente de sus respectivos puntajes.
		if (benchPoints == 7.5) {
			printf("\nLa banca gana con 7.5 puntos. Todos los jugadores pierden sus apuestas.\n");
			for(int i = 0; i < players; i++) {
				benchWallet += playersBet[i];
			}
			benchStatus = 1;
		}
		if (benchPoints > 7.5) {	//Si la banca se pasa 7.5, pierde y ganan los jugadores.
			printf("\nLa banca pierde: ha superado 7.5 puntos. Ganan los jugadores!\n");
			for (int i = 0; i < players; i++) {
				if (playerPoints[i] == 7.5) {
					playersWallet[i] += playersBet[i] * playerGainRatio[i];
					benchWallet = benchWallet - (playersBet[i] * (playerGainRatio[i] - 1));
				} else {
					playersWallet[i] += playersBet[i];
				}

				winners[i] = 1;
			}
			benchStatus = 0;
		}
		if (benchPoints >= 6 && benchPoints < 7.5) {	//Si la banca tiene entre 6 y 7.5 puntos, se planta y se comparan los puntajes con los jugadores en juego.
			printf("\nLa banca se planta. Ha acumulado %.2f puntos.\n", benchPoints);
			maxScore = 0;
			/**
			 * La comparacion de puntajes la hacemos recorriendo el array de puntajes y guardando el mayor puntaje en la variable maxScore.
			 *
			 * A medida que recorremos el array, comparamos si el siguiente puntaje es mayor al que estaba almacenado en maxScore y, si es asi, reemplazamos el valor de la variable
			 * por el nuevo puntaje mas alto. A su vez, mientras vamos encontrando mayores puntuaciones, guardamos la posicion del array del jugador con dicho puntaje en
			 * un nuevo array denominado Winners[]. Si encontramos otro puntaje mas alto a ese, limpiamos el array de Winners[] y guardamos unicamente la posicion del jugador
			 * que supero la puntuacion.
			 *
			 * Por otro lado, si encontramos un puntaje igual al mas alto, guardamos la posicion de ambos jugadores para indicar que los dos son ganadores.
			 */
			for (int i = 0; i < players; i++){
				if ((playerPoints[i] < 8) && (playerPoints[i] > benchPoints)) {
					if (playerPoints[i] > maxScore) {
						maxScore = playerPoints[i];
						for (int j = 0; j < players; j++) {
							winners[j] = 0;
						}
						winners[i] = 1;
					} else if (playerPoints[i] == maxScore) {
						winners[i] = 1;
					}
					benchStatus = 0; //Si hay al menos un jugador ganador, pierde la banca.
				}
			}
			/**
			 * Luego de que tenemos los ganadores definidos, calculamos la parte que se lleva cada uno.
			 *
			 * Si uno de esos ganadores gana con 7.5, se lleva lo apostado mas el porcentaje de ganancia que le corresponda.
			 * Por otra parte, si solo gana por tener puntuacion mas alta sin necesariamente haber conseguido 7.5, se le devuelve el valor de lo apostado.
			 */
			for(int i = 0; i < players; i++) {
				if (winners[i] == 1) {
					if (playerPoints[i] == 7.5) {
						playersWallet[i] += playersBet[i] * playerGainRatio[i];
						benchWallet = benchWallet - (playersBet[i] * (playerGainRatio[i] - 1));
					} else {
						playersWallet[i] += playersBet[i];
					}
				} else {
					benchWallet += playersBet[i];
				}
			}
		}

		//Muestro estadisticas al finalizar cada ronda y vacio array de apuestas.
		printf("\nFin de la ronda %d.\nEstadisticas:", roundNumber);
		if (benchStatus == 1) {
			printf("\n\tGano la banca. %.2f puntos. Tesoro: $%.2f", benchPoints, benchWallet);
		} else {
			printf("\n\tPerdio la banca. %.2f puntos. Tesoro: $%.2f", benchPoints, benchWallet);
		}
		for(int i = 0; i < players; i++) {
			if (winners[i] == 1) {
				printf("\n\tJugador %d gano. Puntos: %.2f. Aposto: %.2f. Saldo: $%.2f", i + 1, playerPoints[i], playersBet[i], playersWallet[i]);
			} else {
				printf("\n\tJugador %d perdio. Puntos: %.2f. Aposto: %.2f. Saldo: $%.2f", i + 1, playerPoints[i], playersBet[i], playersWallet[i]);
			}
			playersBet[i] = 0;
		}

		benchStatus = 1;	//Reinicio valor por default del estado de la banca.

	}
	//Termina la ronda.

	/**
	 * Para saber que jugador fue el que mas ganancias tuvo y el balance general de todos los jugadores:
	 *
	 * - Calculamos el balance general realizando la diferencia entre el monto actual y el monto inicial de cada jugador.
	 *
	 * - Conocemos el jugador que mas dinero gano, comparando todas las diferencias de balances entre ellos y guardando el mayor
	 * en la variable maxProfit. Luego, guardamos el numero de jugador que logro tener mayor ganancia en la variable profitWinner.
	 */
	for(int i = 0; i < players; i++) {
		totalBalance = totalBalance + (playersWallet[i] - 5000);
		if ((playersWallet[i] - 5000) > maxProfit) {
			maxProfit = playersWallet[i] - 5000;
			profitWinner = i;
		}
	}

/*maxcardNum = 0;
	for (int i = 0; i < 10; i++) {
		if (cardsCount[i] > maxcardNum) {
			maxcardNum = cardsCount[i];
			for (int j = 0; j < 10; j++) {
				topCards[j] = 0;
			}
			topCards[i] = 1;
		} else if (cardsCount[i] == maxcardNum) {
			topCards[i] = 1;
		}
	}
*/

	for (int i = 0; i < 5; i++) {
		max = i;
		for (int j = i+1; j < 10; j++) {
			if (cardsCount[j] > cardsCount[max]) {
				max = j;
			}
		}
		temp = cardsCount[i];
		cardsCount[i] = cardsCount[max];
		cardsCount[max] = temp;

		temp = indexes[i];
		indexes[i] = indexes[max];
		indexes[max] = temp;
	}


	//Muestro estadisticas en toda la partida.
	printf("\n---------------------------------------------------------------");
	printf("\nFin de la partida.\nEstadisticas:");
	printf("\n\tJugadores que lograron 7.5 + figura: %d", sieteFigura);
	printf("\n\tJugadores que perdieron por pasarse de 7.5: %d", playersOut);
	if ((benchWallet - 100000) > maxProfit) {
		printf("\n\tLa banca gano mas dinero con un total de $%.2f en ganancias.", (benchWallet - 100000));
	} else {
		printf("\n\tEl jugador %d gano mas dinero con un total de $%.2f en ganancias.", (profitWinner + 1), maxProfit);
	}
	printf("\n\tBalance general del conjunto de jugadores: $%.2f. ", totalBalance);
	if (totalBalance < 0) {
		printf("Fueron mas perdidas que ganancias.\n");
	} else {
		printf("Fueron mas ganancias que perdidas.\n");
	}
	printf("\n\tEl top 5 de los numeros de las cartas que mas salieron son:\n");
	for (int i = 0; i < 5; i++) {
		if (cardsCount[i] > 1) {
			printf("\t\t%d - Carta %d: %d veces.\n", i + 1, indexes[i] + 1, cardsCount[i]);
		} else {
			printf("\t\t%d - Carta %d: %d vez.\n", i + 1, indexes[i] + 1, cardsCount[i]);
		}
	}
	printf("\n\n");

	return 0;
}


