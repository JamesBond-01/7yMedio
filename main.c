#include "cards.h"
#include "inputs.h"
#include <stdio.h>
#include <stdbool.h>

int main() {
	const int MIN_PLAYERS = 1;	//Cantidad minima permitida de jugadores.
	const int MAX_PLAYERS = 5;	//Cantidad maxima permitida de jugadores.
	const int MIN_ROUNDS = 2;	//Cantidad minima permitida de rondas.
	const int MAX_ROUNDS = 4;	//Cantidad maxima permitida de rondas.
	const double MIN_BET = 100;	//Cantidad minima permitida de apuestas.
	const double MAX_BET = 1500;	//Cantidad maxima permitida de apuestas.


	double benchWallet = 100000;	//Monto con el que empieza la banca.
	int card, players, rounds, currentCard;
	double benchPoints, bet, maxBet;
	bool option, secondCardPopped;

	printf("\n##################################");
	printf("\nBienvenido al juego Siete y Medio");
	printf("\n##################################\n");

	printf("\nPara comenzar, por favor ingrese los siguientes datos:\n");

	//Determinamos la cantidad de jugadores que habra en la partida.
	players = getUserInputAsInt("Cantidad de jugadores con los que desee jugar", MIN_PLAYERS, MAX_PLAYERS );

	double playerPoints[players];	//Array que contiene la cantidad de puntos de cada jugador.
	double playersWallet[players];	//Billetera donde se almacena el dinero de cada jugador.
	double playersBet[players];	//Array que contiene la apuesta que hace cada jugador por ronda.
	double playerGainRatio[players];//Array que contiene el porcentaje de ganancia por sobre la apuesta.

	int playerNumber;	//Variable para saber el numero del jugador.
	bool isPlayerOut = false;	//Variable para comprobar si un jugador queda afuera o no.
	int playersOut[players];

	//Le asignamos a cada jugador su monto inicial ($5000).
	for (playerNumber = 0; playerNumber < players; playerNumber++) {
		playersWallet[playerNumber] = 5000;
	}

	//Determinamos la cantidad de rondas que habra en la partida.
	rounds = getUserInputAsInt("Cantidad de rondas que dura la partida", MIN_ROUNDS, MAX_ROUNDS);

	//Repeticion de rondas.
	for (int roundNumber = 1; roundNumber <= rounds; roundNumber++) {

		//Genero el mazo de cartas mezclado aleatoriamente.
		int *cardStack = getCardsStack();

		printf("\n\t========");
		printf("\n\tRonda %d\n", roundNumber);
		printf("\t========\n");

		//Turno de cada uno de los jugadores en la ronda.
		for (playerNumber = 0; playerNumber < players ; playerNumber++) {

			//Elijo una carta aleatoria y en su posicion dentro del array lo reemplazo por un 0 para que no vuelva a aparecer en la ronda.
			card = popCard(cardStack);
			playerPoints[playerNumber] = getCardPoints(card);

			//Si a un jugador le queda menos dinero que el minimo que se puede apostar, no puede jugar mas.
			if (playersWallet[playerNumber] < MIN_BET) {
				printf("Usted no posee saldo suficiente para apostar y en consecuencia no puede seguir jugando.");
				continue;
			}

			printf("\n***** Turno del jugador %d *****.", playerNumber + 1);
			printf("\nCarta recibida: %s", getCardString(card));

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
			printf("\nPuntaje de la carta: %.2f", playerPoints[playerNumber]);

			secondCardPopped = true;
			playerGainRatio[playerNumber] = 1; // Valor por defecto en caso de que no gane.

			isPlayerOut = false;
			playersOut[playerNumber] = 0;

			//Preguntamos si quiere otra carta y evaluamos su puntaje total.
			do {
				option = getUserInputAsBool("\nDesea pedir otra carta?");
				if (option) {
					currentCard = popCard(cardStack);
					playerPoints[playerNumber] += getCardPoints(currentCard);
					printf("\nCarta recibida: %s", getCardString(currentCard));

					//Analisis con 2 cartas.
					if (secondCardPopped) {
						secondCardPopped = false;

						int cardType1 = getCardType(card);
						int cardType2 = getCardType(currentCard);
						int cardNum1 = getCardNumber(card);
						int cardNum2 = getCardNumber(currentCard);

						if (playerPoints[playerNumber] == 7.5) {
							//Gana con: 7 + figura (palo oro y figura rey), premio 100% de lo apostado.
							if ((cardType1 == 2 && cardType2 == 2) && (cardNum1 == 12 || cardNum2 == 12)) {
								playerGainRatio[playerNumber] = 2;
							}
							//Gana con: 7 + figura, del mismo palo, premio 75% de lo apostado
							else if (cardType1 == cardType2) {
								playerGainRatio[playerNumber] = 1.75;
							}
							//Gana con: 7 + figura, el premio es 50% de lo apostado
							else {
								playerGainRatio[playerNumber] = 1.50;
							}

							printf("\nUsted gana solo con 2 cartas !");
							printf ("\nJugador %d tiene $%.2f", playerNumber + 1, playersWallet[playerNumber]);
							break;
						}
					}
					//Analisis con 3 o mas cartas, en caso de que llegue a conseguir 7.50 puntos exactamente.
					if (playerPoints[playerNumber] == 7.5){
						printf("\nUsted gana !");
						playerGainRatio[playerNumber] = 1.25;
						printf ("\nJugador %d tiene $%.2f", playerNumber + 1, playersWallet[playerNumber]);
						break;
					}

					//Si se pasa de 7.5 pierde la apuesta.
					if (playerPoints[playerNumber] > 7.5) {
						printf("\nUsted pierde la ronda. Ha superado 7.5. Total de puntos: %.2f\n", playerPoints[playerNumber]);
						printf ("\nJugador %d tiene $%.2f", playerNumber + 1, playersWallet[playerNumber]);
						isPlayerOut = true;
						if(isPlayerOut) {
							playersOut[playerNumber] = playerNumber + 1;
						}
						benchWallet += bet;
						break;
					}

					playersWallet[playerNumber] = playersWallet[playerNumber] * playerGainRatio[playerNumber];
					printf ("\nJugador %d tiene %.2f puntos", playerNumber + 1, playerPoints[playerNumber]);
				}
			} while (option);
		}

		//Turno de la banca.
		printf("\n\n ***** Turno de la banca *****\n");
		card = popCard(cardStack);
		benchPoints = getCardPoints(card);
		printf("\nLa banca recibe el %s", getCardString(card));

		//Mientras el puntaje de la banca sea menor o igual a 5.5, pide otra carta.
		while (benchPoints <= 5.5) {
			card = popCard(cardStack);
			printf("\nLa banca pide otra carta y recibe el %s.", getCardString(card));
			benchPoints += getCardPoints(card);
		}

		//Si la banca se pasa 7.5, pierde y ganan los jugadores en competencia.
		if (benchPoints > 7.5) {
			printf("\nLa banca pierde: ha superado 7.5 puntos. Ganan los jugadores!\n");
			for (int p = 0; p < players; p++) {
				if(playersOut[p] == 0) {
					playersWallet[p] += playersBet[p];
					playerGainRatio[p] = 1;
				}
			}
		}

		//Si la banca tiene entre 6 y 7.5 puntos, se planta y se comparan los puntajes con los jugadores en juego.
		else if (benchPoints >= 6 && benchPoints <= 7.5) {
			printf("\nLa banca se planta. Ha acumulado %.2f puntos.\n", benchPoints);
			int counter1, counter2, swap_points, swap_playerNumber;
			int playersPointsAndNumber[players][2];

			for (playerNumber = 0; playerNumber < players; playerNumber++) {
				playersPointsAndNumber[playerNumber][0] = playerPoints[playerNumber];// aca van los puntos
				playersPointsAndNumber[playerNumber][1] = playerNumber; // aca van el nro de jugador
			}

			//Algoritmo de ordenamiento "Metodo Burbuja" para ordenar los puntajes de los jugadores.
			for (counter1 = 0; counter1 < players - 1; counter1++) {
				for (counter2 = 0; counter2 < players - counter1 - 1; counter2++) {
					if (playersPointsAndNumber[counter2][0] > playersPointsAndNumber[counter2 + 1][0]) {
						swap_points = playersPointsAndNumber[counter2][0];
						swap_playerNumber = playersPointsAndNumber[counter2][1];
						playersPointsAndNumber[counter2][0] = playersPointsAndNumber[counter2 + 1][0];
						playersPointsAndNumber[counter2][1] = playersPointsAndNumber[counter2 + 1][1];
						playersPointsAndNumber[counter2 + 1][0] = swap_points;
						playersPointsAndNumber[counter2 + 1][1] = swap_playerNumber;
					}
				}
			}
		}

		printf("\n***Fin de la ronda.***\nEstadisticas:\n");
		for(int i = 0; i < players; i++) {
			printf("Jugador %d: %.2f puntos. Apuesta: $%.2f. Saldo: $%.2f\n", i + 1, playerPoints[i], playersBet[i], playersWallet[i]);
		}
		printf("Banca: %.2f puntos. Tesoro: $%.2f\n", benchPoints, benchWallet);
	}

	//Comparar la banca contra la primera posicion. Si gana la banca se finaliza la ronda.
	//Si esto no pasa, se compara el primero con el segundo para ver si hay empate.
	//Si no hay empate, gana el primero. Sino, se compara el puntaje del segundo contra el tercero para ver si no hubo un empate triple.

	return 0;
}
