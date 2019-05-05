#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chess_framework.h"

// The main attraction!
int main(){

	// Load global chess information
	Global* global = (Global*)malloc(sizeof(Global));
	GlobalLoadBBs(global);

	// Load board state
	Board* board = (Board*)malloc(sizeof(Board));
	BoardReset(board);

	// For Daniel
	total_of_218_moves(global, board);
	return 0;

	// Test game
	Move move;
	Move* moveList;
	U16 length;
	char input[6];

	for(;;){

		BoardPrint(board);
		moveList = pseudoMoveGenerator(global, board, &length);

		scanf("%s", input);

		for(U16 i = 0; i < length; i++){

			move = moveList[i];
			if (strcmp(input, moveToUCI(move)) == 0){
				makeMove(global, board, move);
				break;
			}
		}
	}
	return 0;
}
