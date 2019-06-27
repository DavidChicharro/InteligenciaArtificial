#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>

void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
	AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}

bool ComportamientoJugador::estaVisitado(const vector<pair<int,int> > &v, const estado &p){
	for(int i=0 ; i<v.size() ; i++)
		if(v[i] == make_pair(p.fila, p.columna))
			return true;

	return false;
}

bool ComportamientoJugador::esCasillaValida(const unsigned char contenidoCasilla){
	return (contenidoCasilla=='S' || contenidoCasilla=='T' || contenidoCasilla=='K');
}

int ComportamientoJugador::hayAldeano(const Sensores &sensores){
	for(int i=1 ; i<8 ; i++)
		if(i!=4 && sensores.superficie[i] == 'a')
			return i;

	return -1;
}

void bordearIzquierda(list<Action> &plan){
	plan.push_front(actTURN_L);
	plan.push_front(actFORWARD);
	plan.push_front(actTURN_R);
	plan.push_front(actFORWARD);
	plan.push_front(actFORWARD);
	plan.push_front(actTURN_R);
	plan.push_front(actFORWARD);
	plan.push_front(actTURN_L);
	plan.push_front(actFORWARD);
}

void bordearDerecha(list<Action> &plan){
	plan.push_front(actTURN_R);
	plan.push_front(actFORWARD);
	plan.push_front(actTURN_L);
	plan.push_front(actFORWARD);
	plan.push_front(actFORWARD);
	plan.push_front(actTURN_L);
	plan.push_front(actFORWARD);
	plan.push_front(actTURN_R);
	plan.push_front(actFORWARD);
}

void retroceder(list<Action> &plan){
	plan.push_front(actIDLE);
	plan.push_front(actIDLE);
	plan.push_front(actTURN_R);
	plan.push_front(actTURN_R);
	plan.push_front(actFORWARD);
	plan.push_front(actTURN_R);
	plan.push_front(actTURN_R);
}
void irIzquierda(list<Action> &plan){
	plan.push_front(actFORWARD);
	plan.push_front(actTURN_L);
}
void irDerecha(list<Action> &plan){
	plan.push_front(actFORWARD);
	plan.push_front(actTURN_R);
}

bool ComportamientoJugador::obstaculoIzquierda(){
	unsigned char contenidoCasilla;

	switch(brujula){
		case 0: contenidoCasilla = mapaResultado[fil][col-1]; break;
		case 1: contenidoCasilla = mapaResultado[fil-1][col]; break;
		case 2: contenidoCasilla = mapaResultado[fil][col+1]; break;
		case 3: contenidoCasilla = mapaResultado[fil+1][col]; break;
	}

	return !esCasillaValida(contenidoCasilla);
}

bool ComportamientoJugador::obstaculoDerecha(){
	unsigned char contenidoCasilla;

	switch(brujula){
		case 0: contenidoCasilla = mapaResultado[fil][col+1]; break;
		case 1: contenidoCasilla = mapaResultado[fil+1][col]; break;
		case 2: contenidoCasilla = mapaResultado[fil][col-1]; break;
		case 3: contenidoCasilla = mapaResultado[fil-1][col]; break;
	}

	return !esCasillaValida(contenidoCasilla); //meter opcion de aldeano
}


bool ComportamientoJugador::pathAldeano(int casillaAldeano, list<Action> &plan, Sensores sensores){
	bool seguirRecto = true;
	list<Action>::iterator it=plan.begin();
	for(int i=0 ; i<3 ; i++)
		if(*it != actFORWARD)
			seguirRecto = false;
	
	if(casillaAldeano==6){
		if(seguirRecto){
			if(esCasillaValida(sensores.terreno[2]) && esCasillaValida(sensores.terreno[12])){
				if(esCasillaValida(sensores.terreno[1]) && esCasillaValida(sensores.terreno[5])
					&& esCasillaValida(sensores.terreno[11]) ){
					for(int i=0 ; i<3 ; i++)
						plan.erase(plan.begin());

					bordearIzquierda(plan);
				}
				else if(esCasillaValida(sensores.terreno[3]) && esCasillaValida(sensores.terreno[7])
					&& esCasillaValida(sensores.terreno[13]) ){
					for(int i=0 ; i<3 ; i++)
						plan.erase(plan.begin());

					bordearDerecha(plan);
				}
			}
		}
	}
	else if(casillaAldeano==2 || casillaAldeano==6){ // aldeano enfrente
		bool girar = false;
		it=plan.begin(); it++;

		if(obstaculoIzquierda() && !obstaculoDerecha()){
			for(int i=0 ; i<2 && !girar ; i++)
				if(*it == actTURN_R)
					girar = true;
			if(!girar){
				plan.clear();
				irDerecha(plan);
				return true;
			}
		}
		else if(obstaculoDerecha() && !obstaculoIzquierda()){
			//it=plan.begin(); it++;
			for(int i=0 ; i<2 && !girar ; i++)
				if(*it == actTURN_L)
					girar = true;
			if(!girar){
				plan.clear();
				irIzquierda(plan);
				return true;
			}
		}
		if(girar){
			plan.clear();
			retroceder(plan);

			return true;
		}
	}

	return false;
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	plan.clear();

	estado nodo_padre, adyac, st=origen; 
	nodo_padre.fila=-1; nodo_padre.columna=-1; nodo_padre.orientacion=-1;

	queue<pair<estado,estado> > Q;
	vector<pair<estado,estado> > padre;
	vector<pair<int,int> > visitado;
	bool continuar = true;	

	Q.push(make_pair(st,nodo_padre));
	visitado.push_back(make_pair(st.fila, st.columna));
	

	while(!Q.empty() && continuar){
		unsigned char contenidoCasilla;
		st = Q.front().first;
		padre.push_back(Q.front());
		Q.pop();

		for(int i=0 ; i<4 && continuar; i++){
			switch(i){
				case 0: contenidoCasilla = mapaResultado[st.fila-1][st.columna]; break;
				case 1: contenidoCasilla = mapaResultado[st.fila][st.columna+1]; break;
				case 2: contenidoCasilla = mapaResultado[st.fila+1][st.columna]; break;
				case 3: contenidoCasilla = mapaResultado[st.fila][st.columna-1]; break;
			}

			if(i==0 && st.fila > 0){
				if(esCasillaValida(contenidoCasilla)){
					adyac.fila = st.fila-1;
					adyac.columna = st.columna;
					adyac.orientacion = i;
				}
			}
			else if(i==1 && st.fila < mapaResultado.size()){
				if(esCasillaValida(contenidoCasilla)){
					adyac.fila = st.fila;
					adyac.columna = st.columna+1;
					adyac.orientacion = i;
				}
			}
			else if(i==2 && st.columna < mapaResultado.size()){
				if(esCasillaValida(contenidoCasilla)){
					adyac.fila = st.fila+1;
					adyac.columna = st.columna;
					adyac.orientacion = i;
				}
			}
			else if(i==3 && st.columna > 0){
				if(esCasillaValida(contenidoCasilla)){
					adyac.fila = st.fila;
					adyac.columna = st.columna-1;
					adyac.orientacion = i;
				}
			}
			if(esCasillaValida(contenidoCasilla))
				if(!estaVisitado(visitado, adyac)){
					visitado.push_back(make_pair(adyac.fila, adyac.columna));
					Q.push(make_pair(adyac,st));
					if(adyac.fila == destino.fila && adyac.columna == destino.columna){
						padre.push_back(make_pair(adyac,st));
						continuar = false;;
					}
				}
		}
	}

	estado ac = padre[padre.size()-1].first;
	nodo_padre = padre[padre.size()-1].second;

	estado ult_ac = ac;
	vector<estado> camino;
	camino.push_back(ac);

	for(int i=padre.size()-2 ; i>=0 ; i--)
		if(nodo_padre.fila == padre[i].first.fila && nodo_padre.columna == padre[i].first.columna){
			ac = padre[i].first;
			nodo_padre = padre[i].second;
			camino.push_back(ac);
			ult_ac = ac;
		}

	ac = camino[camino.size()-1];
	ult_ac = origen;

	for(int i=camino.size()-2 ; i>=0; i--){
		ac = camino[i];
		if(ac.fila < ult_ac.fila){
			if(ult_ac.orientacion == 0)
				plan.push_back(actFORWARD);
			else if(ult_ac.orientacion == 1){
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
			}
			else if(ult_ac.orientacion == 3){
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
			}
			else{
				plan.push_back(actTURN_R);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
			}
			ac.orientacion = 0;
		}
		else if(ac.columna > ult_ac.columna){
			if(ult_ac.orientacion == 1)
				plan.push_back(actFORWARD);
			else if(ult_ac.orientacion == 2){
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
			}
			else if(ult_ac.orientacion == 0){
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
			}
			else{
				plan.push_back(actTURN_R);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
			}
			ac.orientacion = 1;
		}
		else if(ac.fila > ult_ac.fila){
			if(ult_ac.orientacion == 2)
				plan.push_back(actFORWARD);
			else if(ult_ac.orientacion == 3){
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
			}
			else if(ult_ac.orientacion == 1){
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
			}
			else{
				plan.push_back(actTURN_R);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
			}
			ac.orientacion = 2;
		}
		else if(ac.columna < ult_ac.columna){
			if(ult_ac.orientacion == 3)
				plan.push_back(actFORWARD);
			else if(ult_ac.orientacion == 0){
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
			}
			else if(ult_ac.orientacion == 2){
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
			}
			else{
				plan.push_back(actTURN_R);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
			}
			ac.orientacion = 3;
		}

		ult_ac = ac;	
	}
	
	VisualizaPlan(origen, plan);
	
	return true;
}

bool ComportamientoJugador::crearNuevoPlan(const Sensores &sensores){
	estado origen;
	origen.fila = fil;
	origen.columna = col;
	origen.orientacion = brujula;

	destino.fila = sensores.destinoF;
	destino.columna = sensores.destinoC;

	hayPlan = pathFinding(origen,destino,plan);
	VisualizaPlan(origen, plan);

	return hayPlan;
}

Action ComportamientoJugador::think(Sensores sensores) {
	if (sensores.mensajeF != -1){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
	}

	Action sigAccion;

	if(!accionReactiva){
		// Determinar si ha cambiado el destino desde la ultima planificacion
		if (hayPlan and (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna)){
			cout << "El destino ha cambiado\n";
			hayPlan = false;
		}

		unsigned char contenidoCasilla;
		switch(brujula){
			case 0: contenidoCasilla = mapaResultado[fil-1][col]; break;
			case 1: contenidoCasilla = mapaResultado[fil][col+1]; break;
			case 2: contenidoCasilla = mapaResultado[fil+1][col]; break;
			case 3: contenidoCasilla = mapaResultado[fil][col-1]; break;
		}

		// Determinar si tengo que construir un plan
		if (!hayPlan){
			hayPlan = crearNuevoPlan(sensores);
		}

		if (hayPlan and plan.size()>0){
			casillaAldeano = hayAldeano(sensores);
			if(casillaAldeano == -1){
				sigAccion = plan.front();
				plan.erase(plan.begin());
			}
			else{
				accionReactiva = pathAldeano(casillaAldeano, plan, sensores);

				if (accionReactiva)
					hayPlan = false;
				else{
					sigAccion = plan.front();
					plan.erase(plan.begin());
				}
			}
		}
		else {
			sigAccion = actIDLE;
			hayPlan = false;
		}
	}
	if(accionReactiva && plan.size()>0){
		sigAccion = plan.front();
		plan.erase(plan.begin());

		if(sigAccion == actFORWARD && !esCasillaValida(sensores.terreno[2])){
			sigAccion == actIDLE;
			accionReactiva = false;
		}
			
		if(plan.size() == 0)
			accionReactiva=false;
	}

	ultimaAccion = sigAccion;

	switch (ultimaAccion){
		case actTURN_R: brujula = (brujula+1)%4; break;
		case actTURN_L: brujula = (brujula+3)%4; break;
		case actFORWARD:
			switch (brujula){
				case 0: fil--; break;
				case 1: col++; break;
				case 2: fil++; break;
				case 3: col--; break;
			}
			cout << "fil: " << fil << "  col: " << col << " Or: " << brujula << endl;
	}

	return sigAccion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
