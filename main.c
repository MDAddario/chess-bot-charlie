#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chess_framework.h"

// The main attraction!
int main(int argc, char* argv[]){

	// Load global chess information
	Global* global = (Global*)malloc(sizeof(Global));
	GlobalLoadBBs(global);

	// Load board state
	Board* board = (Board*)malloc(sizeof(Board));
	BoardReset(board);







	// Debug chess functions
	Move move;

	printf("e4\n");
	configureMove(&move, 12, 28, DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move);
	makeMove(global, board, move, NO);

	printf("a6\n");
	configureMove(&move, 48, 40, Quiet, Pawn, Null_piece);
	debugBoard(global, board, move);
	makeMove(global, board, move, NO);

	printf("e5\n");
	configureMove(&move, 28, 36, Quiet, Pawn, Null_piece);
	debugBoard(global, board, move);
	makeMove(global, board, move, NO);

	printf("d5\n");
	configureMove(&move, 51, 35, DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move);
	makeMove(global, board, move, NO);

	printf("exd\n");
	configureMove(&move, 36, 43, EPCapture, Pawn, Pawn);
	debugBoard(global, board, move);
	makeMove(global, board, move, NO);

	return 0;








	/*
	Obvious move generation bug when playing the following sequence:
	12 -> 0 -> 29 -> 5 ->
	There are two ways to capture the d6 pawn?
	Move 29, 30 breaks the game
	-------------------------------------------
	Clearly, there is an issue with EP, in at least one of

		- psuedoMoveGenerator()
		- validateMove()
		- makeMove()
		- undoMove()
	*/

	// Interactive chess playing
	char input, buffer;
	Move* move_list;
	U16 length, move_num, castling_rights, EP_files;

	for(;;){

		BoardPrint(board);
		printf("\n\nWould you like to makeMove (m) or undoMove (u): ");
		input = getchar();
		while ((buffer = getchar()) != '\n' && buffer != EOF) { }

		if (input == 'm'){

			move_list = legalMoveGenerator(global, board, &length, isInCheck(global, board, 64, YES));
			movePrinter(global, board);
			printf("Please input which move you would like to make: ");
			scanf("%hu", &move_num);
			while ((buffer = getchar()) != '\n' && buffer != EOF) { }

			castling_rights = board->castlingRights;
			EP_files = board->EPFiles;	
			makeMove(global, board, move_list[move_num], NO);
			
		}
		else if (input == 'u'){

			if (board->ply == 1){
				printf("You need to make a move first idiot.\n");
				continue;
			}
			undoMove(global, board, move_list[move_num], castling_rights, EP_files);
		}
		else
			printf("Please input 'm' or 'u' for make or undo.\n");
	}
	return 0;

	// FEN loading
	if (argc < 5){
		printf("Please provide a complete FEN string.\n");
		return -1;
	}
	loadFEN(board, argc, argv);
	BoardPrint(board);
	return 0;

	// Perft testing
	if (argc < 2){
		printf("Please provide depth for perft() function.\n");
		return -1;
	}
	initPerft(global, board, atoi(argv[1]));
	return 0;
}
