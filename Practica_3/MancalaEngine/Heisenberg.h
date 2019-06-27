/*
 * Heisenberg.h
 *
 *  Created on: mayo 2018
 *      Author: David Carrasco Chicharro
 */

#include "Bot.h"

#ifndef HEISENBERG_H_
#define HEISENBERG_H_

class Heisenberg:Bot {
private:
	Move buscarMovimientoValido(GameState estado, Player turno);
	int Heuristica(GameState estado, Player turno);
	pair<int,Move> minimax(GameState estado, int prof, Player turno, int movimiento);

public:
	Heisenberg();
	~Heisenberg();


	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);
};

#endif /* HEISENBERG_H_ */
