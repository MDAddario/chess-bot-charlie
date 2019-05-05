#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chess_engine.h"

void GlobalLoadValues(Global* global){

	char* filename = "./pieceSquareTables/piece_values.txt";
	FILE* fptr;
	char dummy[20];
	U16 value;

	fptr = fopen(filename, "r");

	if (fptr == NULL){
		printf("Unable to find %s\n", filename);
		return;
	}

	for(U16 type = 0; type < 6; type++){

		fscanf(fptr, "%s = %hu\n", dummy, &value);
		global->pieceValues[type] = value;
	}
	fclose(fptr);
	return;
}

void GlobalLoadTables(Global* global){

	char* filename = "./pieceSquareTables/piece_square_tables.txt";
	FILE* fptr;
	char dummy[20];
	S16 value;

	fptr = fopen(filename, "r");

	if (fptr == NULL){
		printf("Unable to find %s\n", filename);
		return;
	}

	for(U16 type = 0; type < 6; type++){

		fscanf(fptr, "%s\n", dummy);

		for(S16 i = 7; i >= 0; i--)
			for(U16 j = 0; j < 8; j++){

				fscanf(fptr, " %hi, ", &value);
				global->pieceSquareTables[type][i*8+j] = value;
			}
	}
	fclose(fptr);
	return;
}

void GlobalLoadZobristKeys(Global* global){

	srand(260753313);

	for(U16 i = 0; i < 12; i++)
		for(U16 j = 0; j < 64; j++)
			global->zobristPieceKeys[i][j] = randU64();

	global->zobristBlackMoveKey = randU64();

	for(U16 i = 0; i < 4; i++)
		global->zobristCastlingKeys[i] = randU64();

	for(U16 i = 0; i < 8; i++)
		global->zobristEPFileKeys[i] = randU64();

	return;
}	

void GlobalTest(Global* global){

	printf("\nKing piece value:\n");
	printf("%hu\n", global->pieceValues[King]);

	printf("\nBishop piece-square table:\n");
	for(S16 i = 7; i >= 0; i--){
		printf("[");
		for(U16 j = 0; j < 8; j++)
			printf("\t%hi", global->pieceSquareTables[Bishop][i*8+j]);
		printf("]\n");
	}

	printf("\nWhite short castle zobrist key:\n");
	printf("%llu\n", global->zobristCastlingKeys[ShortW]);

	printf("Expected value:\n");
	printf("2712767669186068480\n\n");

	return;
}

U64 randU64(){

	U32 head = rand();
	U32 tail = rand();

	return (U64)head << 32 + (U64)tail;
}