#include <stdio.h>
#include <stdlib.h>
#include "chess_framework.h"

// The main attraction!
int main(int argc, char* argv[]){

	// Load global chess information
	Global* global = (Global*)malloc(sizeof(Global));
	GlobalLoadBBs(global);

	// Load board state
	Board* board = (Board*)malloc(sizeof(Board));
	BoardReset(board);

	// FEN testing
	if (argc < 5){
		printf("Please provide a complete FEN string.\n");
		return -1;
	}
	loadFEN(board, argc, argv);
	BoardPrint(board);
	printf("Ply: %d\n", board->ply);
	return 0;

	// Perft testing
	if (argc < 2){
		printf("Please provide depth for perft() function.\n");
		return -1;
	}
	initPerft(global, board, atoi(argv[1]));
	return 0;
}
