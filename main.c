#include <stdio.h>
#include <stdlib.h>
#include "chess_framework.h"

// The main attraction!
int main(){

	// Load global chess information
	Global* global = (Global*)malloc(sizeof(Global));
	GlobalLoadBBs(global);

	// Load board state
	Board* board = (Board*)malloc(sizeof(Board));
	BoardReset(board);

	// Perft testing
	U16 depth = 1;
	printf("For a depth of %hu, perft() grants %llu total moves.\n", depth, perft(global, board, depth));
	return 0;
}
