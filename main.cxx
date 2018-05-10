#include <iostream>
//#include <time.h>
#include "board.h"
#include "piece.h"
#include "game.h"
using namespace std;

int main(){

	game* GAME=new game();
	GAME->start();

	// could be implemented: choose black/white, clock, stockfish/AI

	delete GAME;
	return 0;
}
