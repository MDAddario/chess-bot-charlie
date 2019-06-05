#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chess_framework.h"

// The main attraction!
int main(int argc, char* argv[]){

	// Load global chess information
	Global* global = (Global*)malloc(sizeof(Global));
	GlobalLoadBBs(global);
	GlobalLoadTurns(global);

	// Load board state
	Board* board = (Board*)malloc(sizeof(Board));
	BoardStart(board);

	/*
	// Test against known perft results
	longPerftDebug(global, board);
	return 0;
	*/

	/*
	// Load FEN string
	loadFEN(board, "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ");
	*/

	// Perft testing
	if (argc < 2){
		printf("Please provide depth for perft() function.\n");
		return -1;
	}
	initPerft(global, board, atoi(argv[1]), 0);
	return 0;
}
