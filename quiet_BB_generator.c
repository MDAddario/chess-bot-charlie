#include <stdio.h>
#include <stdlib.h>
#include "chess_framework.h"

int main(){

	U64 quietBB;
	FILE* fptr;
	U16 rank_from, file_from, rank_to, file_to, turn;
	S16 step;

	for(turn = BlackTurn; turn <= WhiteTurn; turn++){

		// Pawn
		fptr = fopen(filenames_quiet[turn][Pawn], "w+");

		if (turn == WhiteTurn)
			step = 1;
		else
			step = -1;

		for(rank_from = 0; rank_from < 8; rank_from++)
			for(file_from = 0; file_from < 8; file_from++){

				quietBB = 0ULL;

				rank_to = rank_from + step;
				file_to = file_from;
				if (isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				fprintf(fptr, "%llu\n", quietBB);
			}
		fclose(fptr);

		// Rook
		fptr = fopen(filenames_quiet[turn][Rook], "w+");

		for(rank_from = 0; rank_from < 8; rank_from++)
			for(file_from = 0; file_from < 8; file_from++){

				quietBB = 0ULL;

				for(step = 1;; step++){
					rank_to = rank_from + step;
					file_to = file_from;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from - step;
					file_to = file_from;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from;
					file_to = file_from + step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from;
					file_to = file_from - step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				fprintf(fptr, "%llu\n", quietBB);
			}
		fclose(fptr);

		// Knight
		fptr = fopen(filenames_quiet[turn][Knight], "w+");

		for(rank_from = 0; rank_from < 8; rank_from++)
			for(file_from = 0; file_from < 8; file_from++){

				quietBB = 0ULL;

				rank_to = rank_from + 2;
				file_to = file_from + 1;
				if (isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from + 1;
				file_to = file_from + 2;
				if (isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from + 2;
				file_to = file_from - 1;
				if (isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from + 1;
				file_to = file_from - 2;
				if (isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from - 2;
				file_to = file_from + 1;
				if (isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from - 1;
				file_to = file_from + 2;
				if (isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from - 2;
				file_to = file_from - 1;
				if (isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from - 1;
				file_to = file_from - 2;
				if (isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);
				
				fprintf(fptr, "%llu\n", quietBB);
			}
		fclose(fptr);

		// Bishop
		fptr = fopen(filenames_quiet[turn][Bishop], "w+");

		for(rank_from = 0; rank_from < 8; rank_from++)
			for(file_from = 0; file_from < 8; file_from++){

				quietBB = 0ULL;

				for(step = 1;; step++){
					rank_to = rank_from + step;
					file_to = file_from + step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from + step;
					file_to = file_from - step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from - step;
					file_to = file_from + step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from - step;
					file_to = file_from - step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				fprintf(fptr, "%llu\n", quietBB);
			}
		fclose(fptr);

		// Queen
		fptr = fopen(filenames_quiet[turn][Queen], "w+");

		for(rank_from = 0; rank_from < 8; rank_from++)
			for(file_from = 0; file_from < 8; file_from++){

				quietBB = 0ULL;

				for(step = 1;; step++){
					rank_to = rank_from + step;
					file_to = file_from;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from - step;
					file_to = file_from;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from;
					file_to = file_from + step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from;
					file_to = file_from - step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from + step;
					file_to = file_from + step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from + step;
					file_to = file_from - step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from - step;
					file_to = file_from + step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				for(step = 1;; step++){
					rank_to = rank_from - step;
					file_to = file_from - step;

					if(isRankFileInBounds(rank_to, file_to))
						U64SetBit(&quietBB, rank_to, file_to, 1);
					else
						break;
				}

				fprintf(fptr, "%llu\n", quietBB);
			}
		fclose(fptr);

		// King
		fptr = fopen(filenames_quiet[turn][King], "w+");

		for(rank_from = 0; rank_from < 8; rank_from++)
			for(file_from = 0; file_from < 8; file_from++){

				quietBB = 0ULL;

				rank_to = rank_from + 1;
				file_to = file_from;
				if(isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from - 1;
				file_to = file_from;
				if(isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from;
				file_to = file_from + 1;
				if(isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from;
				file_to = file_from - 1;
				if(isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from + 1;
				file_to = file_from + 1;
				if(isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from + 1;
				file_to = file_from - 1;
				if(isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from - 1;
				file_to = file_from + 1;
				if(isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				rank_to = rank_from - 1;
				file_to = file_from - 1;
				if(isRankFileInBounds(rank_to, file_to))
					U64SetBit(&quietBB, rank_to, file_to, 1);

				fprintf(fptr, "%llu\n", quietBB);
			}
		fclose(fptr);
	}
	return 0;
}
