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

	// Quiet test
	printf("==========================\n");
	printf("QUIET TEST\n");
	printf("==========================\n");
	BoardReset(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e7"), bitFromAlgeb("e5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("b1"), bitFromAlgeb("c3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nc3");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("b8"), bitFromAlgeb("c6"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nc6");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("g1"), bitFromAlgeb("h3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nh3");
	makeMove(global, board, move, NO);

	//BoardPrint(board);


	// Capture test
	printf("==========================\n");
	printf("CAPTURE TEST\n");
	printf("==========================\n");
	BoardReset(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e7"), bitFromAlgeb("e5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("g1"), bitFromAlgeb("f3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nf3");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("g8"), bitFromAlgeb("f6"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nf6");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("f3"), bitFromAlgeb("e5"), Capture, Knight, Pawn);
	debugBoard(global, board, move, "Nxe5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("f6"), bitFromAlgeb("e4"), Capture, Knight, Pawn);
	debugBoard(global, board, move, "Nxe4");
	makeMove(global, board, move, NO);

	//BoardPrint(board);


	// Check test
	printf("==========================\n");
	printf("CHECK TEST\n");
	printf("==========================\n");
	BoardReset(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("f7"), bitFromAlgeb("f5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "f5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("d1"), bitFromAlgeb("h5"), Quiet, Queen, Null_piece);
	debugBoard(global, board, move, "Qh5+");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("g7"), bitFromAlgeb("g6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "g6");
	makeMove(global, board, move, NO);

	//BoardPrint(board);


	// Scholars mate test
	printf("==========================\n");
	printf("SCHOLARS MATE TEST\n");
	printf("==========================\n");
	BoardReset(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("a7"), bitFromAlgeb("a5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "a5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("f1"), bitFromAlgeb("c4"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bc4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("b7"), bitFromAlgeb("b6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "b6");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("d1"), bitFromAlgeb("f3"), Quiet, Queen, Null_piece);
	debugBoard(global, board, move, "Qf3");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("h7"), bitFromAlgeb("h6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "h6");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("f3"), bitFromAlgeb("f7"), Capture, Queen, Pawn);
	debugBoard(global, board, move, "Qf7#");
	makeMove(global, board, move, NO);

	//BoardPrint(board);


	// Short castle test
	printf("==========================\n");
	printf("SHORT CASTLE TEST\n");
	printf("==========================\n");
	BoardReset(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e7"), bitFromAlgeb("e5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("g1"), bitFromAlgeb("f3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nf3");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("g8"), bitFromAlgeb("f6"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nf6");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("f1"), bitFromAlgeb("b5"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bb5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("f8"), bitFromAlgeb("b4"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bb4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e1"), bitFromAlgeb("g1"), ShortCastle, King, Null_piece);
	debugBoard(global, board, move, "O-O");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e8"), bitFromAlgeb("g8"), ShortCastle, King, Null_piece);
	debugBoard(global, board, move, "O-O");
	makeMove(global, board, move, NO);

	//BoardPrint(board);


	// Long castle test
	printf("==========================\n");
	printf("LONG CASTLE TEST\n");
	printf("==========================\n");
	BoardReset(board);

	configureMove(&move, bitFromAlgeb("d2"), bitFromAlgeb("d4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "d4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("d7"), bitFromAlgeb("d5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "d5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("b1"), bitFromAlgeb("a3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Na3");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("b8"), bitFromAlgeb("a6"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Na6");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("c1"), bitFromAlgeb("g5"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bg5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("c8"), bitFromAlgeb("g4"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bg4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("d1"), bitFromAlgeb("d2"), Quiet, Queen, Null_piece);
	debugBoard(global, board, move, "Qd2");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("d8"), bitFromAlgeb("d7"), Quiet, Queen, Null_piece);
	debugBoard(global, board, move, "Qd7");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e1"), bitFromAlgeb("c1"), LongCastle, King, Null_piece);
	debugBoard(global, board, move, "O-O-O");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e8"), bitFromAlgeb("c8"), LongCastle, King, Null_piece);
	debugBoard(global, board, move, "O-O-O");
	makeMove(global, board, move, NO);

	//BoardPrint(board);


	// En passant test
	printf("==========================\n");
	printf("EN PASSANT TEST\n");
	printf("==========================\n");
	BoardReset(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("a7"), bitFromAlgeb("a5"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "a5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e4"), bitFromAlgeb("e5"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("d7"), bitFromAlgeb("d5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "d5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e5"), bitFromAlgeb("d6"), EPCapture, Pawn, Pawn);
	debugBoard(global, board, move, "exd6e.p.");
	makeMove(global, board, move, NO);

	//BoardPrint(board);


	// Promotion test
	printf("==========================\n");
	printf("PROMOTION TEST\n");
	printf("==========================\n");
	BoardReset(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("a7"), bitFromAlgeb("a5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "a5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e4"), bitFromAlgeb("e5"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("b7"), bitFromAlgeb("b5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "b5");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e5"), bitFromAlgeb("e6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "e6");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("d7"), bitFromAlgeb("d6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "d6");
	makeMove(global, board, move, NO);
	
	configureMove(&move, bitFromAlgeb("e6"), bitFromAlgeb("f7"), Capture, Pawn, Pawn);
	debugBoard(global, board, move, "exf7+");
	makeMove(global, board, move, NO);

	configureMove(&move, bitFromAlgeb("e8"), bitFromAlgeb("d7"), Quiet, King, Null_piece);
	debugBoard(global, board, move, "Kd7");
	makeMove(global, board, move, NO);

	BoardPrint(board);

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
