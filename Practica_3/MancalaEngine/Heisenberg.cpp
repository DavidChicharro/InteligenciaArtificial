/*
 * Heisenberg.cpp
 *
 *  Created on: mayo 2018
 *      Author: David Carrasco Chicharro
 */

#include "Heisenberg.h"

#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

Heisenberg::Heisenberg() {
	// Inicializar las variables necesarias para ejecutar la partida

}

Heisenberg::~Heisenberg() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void Heisenberg::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string Heisenberg::getName() {
	return "Heisenberg"; // Sustituir por el nombre del bot
}

Move Heisenberg::buscarMovimientoValido(GameState estado, Player turno){	
	for(int i=1 ; i<=6 ; i++)
		if(estado.getSeedsAt(turno, (Position)i) > 0)
			return (Move)i;

	return M_NONE;
}

int Heisenberg::Heuristica(GameState estado, Player turno){
	int valor = 0;
	int seeds_1 = 0, seeds_2 = 0;
	Player cont = (turno==J1)?J2:J1;	
	
	for(int n=1 ; n<=6 ; n++){
		int x = estado.getSeedsAt(turno, (Position) n);
		if (x > 0) {
			if(x <= (n-1) && estado.getSeedsAt(turno, (Position) (n-x)) == 0){
				int p = estado.getSeedsAt(cont, (Position) (7-(n-x)));
				if (p > 1)
					valor += p*20;
			}
		}

		if (n == x)
			valor += 80;
		
		if (n > x)
			valor += 50;

		seeds_1 += x;

		/* Oponente */
		x = estado.getSeedsAt(cont, (Position) n);
		if(x > 0){
			if(x <= (n-1) && estado.getSeedsAt(cont, (Position) (n-x) ) == 0){
				int p = estado.getSeedsAt(turno, (Position) (7-(n-x)));
				if(p > 1)
					valor -= p*25;
			}
		}

		if (n == x)
			valor -= 80;
			
		if (n > x)
			valor -= 50;

		seeds_2 +=x;
	}

	if (seeds_1 > seeds_2)
		valor += 50;
	else
		valor -= 50;


	int mi_punt = estado.getScore(turno);
	int punt_cont = estado.getScore(cont);

	if(mi_punt > punt_cont){
		valor += 50;
		if(mi_punt > 24)
			valor = 1500;
	}
	else{
		valor -= 50;
		if(punt_cont > 24)
			valor = -1500;
	}


	return valor;
}


pair<int,Move> Heisenberg::minimax(GameState estado, int prof, Player turno, int movimiento){
	if(prof==0 || estado.isFinalState()){
		int heuristica = Heuristica(estado, turno);
		return make_pair(heuristica,(Move)movimiento);
	}

	Player cont = (turno==J1)?J2:J1;
	pair<int,Move> v = make_pair(0,M_NONE);

	if(turno==estado.getCurrentPlayer()){
		pair<int,Move> mejor_valor = make_pair(-1000,M_NONE);
		for(int i=1 ; i<=6 ; i++){
			if(estado.getSeedsAt(turno, (Position)i) > 0){
				GameState hijo = estado.simulateMove( (Move) i);

				v = minimax(hijo,prof-1,turno,i);
				if(v.first > mejor_valor.first){
					mejor_valor.first = v.first;
					mejor_valor.second = (Move)i;
				}
			}
		}
		if(mejor_valor.second == M_NONE)
			mejor_valor.second = buscarMovimientoValido(estado,turno);
		
		return mejor_valor;
	}
	else{
		pair<int,Move> mejor_valor = make_pair(1000,(Move)0);
		for(int i=1 ; i<=6 ; i++){
			if(estado.getSeedsAt(cont, (Position)i) > 0){
				GameState hijo = estado.simulateMove( (Move) i);

				v =  minimax(hijo,prof-1,turno,i);
				if(v.first < mejor_valor.first){
					mejor_valor.first = v.first;
					mejor_valor.second = (Move)i;
				}
			}
		}
		if(mejor_valor.second == M_NONE)
			mejor_valor.second = buscarMovimientoValido(estado,turno);
		
		return mejor_valor;
	}
}

Move Heisenberg::nextMove(const vector<Move> &adversary, const GameState &state) {

	Player turno = this->getPlayer();
	long timeout = this->getTimeOut();

	Move movimiento = M_NONE;

	movimiento = minimax(state,7,turno,7).second;

	return movimiento;
}
