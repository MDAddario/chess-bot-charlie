#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chess_framework.h"

void U64SetBit(U64* BB, U16 rank, U16 file, U16 toggle){

	if (toggle)
		*BB |= 1ULL << rank*8 + file;
	else
		*BB &= ~(1ULL << rank*8 + file);
	return;
}

U16 U64GetBit(U64 BB, U16 rank, U16 file){

	return BB >> rank*8 + file & 1ULL;
}

void U16SetBit(U16* number, U16 rank, U16 file, U16 toggle){

	if (toggle)
		*number |= 1 << rank*8 + file;
	else
		*number &= ~(1 << rank*8 + file);
	return;
}

U16 U16GetBit(U16 number, U16 rank, U16 file){

	return number >> rank*8 + file & 1;
}

void setPiece(Board* board, U16 color, U16 piece, U16 rank, U16 file, U16 toggle){

	U64SetBit(board->piecesBB + color, rank, file, toggle);
	U64SetBit(board->piecesBB + piece, rank, file, toggle);
	return;
}

void BBPrint(U64 BB){

	for(S16 rank = 7; rank >= 0; rank--){
		printf("[");
		for(U16 file = 0; file < 8; file++){

			if (U64GetBit(BB, rank, file))
				printf("  X  ");
			else
				printf("  .  ");
		}
		printf("]\n");
	}
	return;
}

void BoardReset(Board* board){

	// Erase all pieces
	memset(&(board->piecesBB), OFF, 64);

	// Pawns
	for(U16 file = 0; file < 8; file++){
		setPiece(board, White, Pawn, 1, file, ON);
		setPiece(board, Black, Pawn, 6, file, ON);
	}

	// First and last ranks
	setPiece(board, White, Rook,   0, 0, ON);
	setPiece(board, White, Knight, 0, 1, ON);
	setPiece(board, White, Bishop, 0, 2, ON);
	setPiece(board, White, Queen,  0, 3, ON);
	setPiece(board, White, King,   0, 4, ON);
	setPiece(board, White, Bishop, 0, 5, ON);
	setPiece(board, White, Knight, 0, 6, ON);
	setPiece(board, White, Rook,   0, 7, ON);

	setPiece(board, Black, Rook,   7, 0, ON);
	setPiece(board, Black, Knight, 7, 1, ON);
	setPiece(board, Black, Bishop, 7, 2, ON);
	setPiece(board, Black, Queen,  7, 3, ON);
	setPiece(board, Black, King,   7, 4, ON);
	setPiece(board, Black, Bishop, 7, 5, ON);
	setPiece(board, Black, Knight, 7, 6, ON);
	setPiece(board, Black, Rook,   7, 7, ON);

	// Castling rights
	for (U16 i = 0; i < 4; i++)
		U16SetBit(&(board->castlingRights), 0, i, ON);

	// En passant
	memset(&(board->EPFiles), OFF, 2);

	// Start ply counter
	board->ply = 1;

	return;
}

void BoardPrint(Board* board){

	char color;

	for(S16 rank = 7; rank >= 0; rank--){

		printf("[");
		
		for(U16 file = 0; file < 8; file++){

			color = '\0';
			
			if (U64GetBit(board->piecesBB[White], rank, file))
				color = 'w';
			else if (U64GetBit(board->piecesBB[Black], rank, file))
				color = 'b';

			if (color){
				for(U16 piece = Pawn; piece <= King; piece++){
					if (U64GetBit(board->piecesBB[piece], rank, file)){

						switch(piece){
							
							case Pawn:
								printf(" P");
								break;
							case Rook:
								printf(" R");
								break;
							case Knight:
								printf(" N");
								break;
							case Bishop:
								printf(" B");
								break;
							case Queen:
								printf(" Q");
								break;
							case King:
								printf(" K");
								break;
						}
						break;
					}
				}
				printf(":%c ", color);
			}
			else
				printf("  .  ");
		}
		printf("]\n");
	}
	return;
}

void GlobalLoadBBs(Global* global){

	char* filename = (char*)malloc(200*sizeof(char));
	FILE* fptr;
	U64 BB;

	for(U16 turn = BlackTurn; turn <= WhiteTurn; turn++){

		for(U16 piece = Pawn; piece <= King; piece++){

			strcpy(filename, "./captureBB/");
			strcat(filename, filenames_capture[turn][piece]);

			fptr = fopen(filename, "r");

			if (fptr == NULL){
				printf("Unable to find %s\n", filename);
				return;
			}

			for(U16 bit_from = 0; bit_from < 64; bit_from++){

				fscanf(fptr, "%llu\n", &BB);
				global->captureBB[turn][piece][bit_from] = BB;
			}
			fclose(fptr);
		}

		for(U16 piece = Pawn; piece <= King; piece++){

			strcpy(filename, "./quietBB/");
			strcat(filename, filenames_quiet[turn][piece]);

			fptr = fopen(filename, "r");

			if (fptr == NULL){
				printf("Unable to find %s\n", filename);
				return;
			}

			for(U16 bit_from = 0; bit_from < 64; bit_from++){

				fscanf(fptr, "%llu\n", &BB);
				global->quietBB[turn][piece][bit_from] = BB;
			}
			fclose(fptr);
		}
	}
	free(filename);
	return;
}

char* filenames_capture[2][6] = {"black_pawn_capture.txt",
								"black_rook_capture.txt",
								"black_knight_capture.txt",
								"black_bishop_capture.txt",
								"black_queen_capture.txt",
								"black_king_capture.txt",
								"white_pawn_capture.txt",
								"white_rook_capture.txt",
								"white_knight_capture.txt",
								"white_bishop_capture.txt",
								"white_queen_capture.txt",
								"white_king_capture.txt"};

char* filenames_quiet[2][6] = {"black_pawn_quiet.txt",
								"black_rook_quiet.txt",
								"black_knight_quiet.txt",
								"black_bishop_quiet.txt",
								"black_queen_quiet.txt",
								"black_king_quiet.txt",
								"white_pawn_quiet.txt",
								"white_rook_quiet.txt",
								"white_knight_quiet.txt",
								"white_bishop_quiet.txt",
								"white_queen_quiet.txt",
								"white_king_quiet.txt"};

U16 isRankFileInBounds(U16 rank, U16 file){

	return rank < 8 && file < 8;	// Thanks to unsigned overflow
}

Move* pseudoMoveGenerator(Global* global, Board* board, U16* length, U16 num_checks){

	// Allocate memory for move list
	Move* list = (Move*)malloc(250*sizeof(Move));
	*length = 0;

	// Bitboard for occupied and empty squares
	U64 occupied_BB = board->piecesBB[White] | board->piecesBB[Black];
	U64 empty_BB = ~occupied_BB;

	// Bitboard for potential quiet moves and captures
	U64 quiet_BB, capture_BB;

	// Local variables
	U16 rank_from, file_from, rank_to, file_to, captured, turn, color, opp_color,
		pawn_starting_rank, pawn_EP_rank, pawn_promo_rank, short_key, long_key;
	S16 pawn_forward_bitshift, pawn_EP_left_bitshift, pawn_EP_right_bitshift;

	// White turn to play
	if (board->ply % 2){
		turn = WhiteTurn;
		color = White;
		opp_color = Black;
		pawn_starting_rank = 1;
		pawn_forward_bitshift = 8;
		pawn_EP_rank = 4;
		pawn_EP_left_bitshift = 7;
		pawn_EP_right_bitshift = 9;
		pawn_promo_rank = 6;
		short_key = ShortW;
		long_key = LongW;
	}
	// Black turn to play
	else{
		turn = BlackTurn;
		color = Black;
		opp_color = White;
		pawn_starting_rank = 6;
		pawn_forward_bitshift = -8;
		pawn_EP_rank = 3;
		pawn_EP_left_bitshift = -9;
		pawn_EP_right_bitshift = -7;
		pawn_promo_rank = 1;
		short_key = ShortB;
		long_key = LongB;
	}

	// If in double check, king MUST move
	if (num_checks == 2){

		for(U16 bit_from = 0; bit_from < 64; bit_from++)
			if (U64GetBit(board->piecesBB[King], 0, bit_from))
				if (U64GetBit(board->piecesBB[color], 0, bit_from)){

					quiet_BB = global->quietBB[turn][King][bit_from] & empty_BB;
					capture_BB = global->captureBB[turn][King][bit_from] & board->piecesBB[opp_color];

					// Determine list of potential moves
					for(U16 bit_to = 0; bit_to < 64; bit_to++){

						// Quiet moves
						if (U64GetBit(quiet_BB, 0, bit_to)){

							configureMove(list+*length, bit_from, bit_to, Quiet, King, Null_piece);
							(*length)++;
						}

						// Captures
						else if (U64GetBit(capture_BB, 0, bit_to)){

							// Determine captured piece
							for(captured = Pawn; captured <= Queen; captured++)
								if (U64GetBit(board->piecesBB[captured], 0, bit_to))
									break;

							configureMove(list+*length, bit_from, bit_to, Capture, King, captured);
							(*length)++;
						}
					}
				}
		return list;
	}

	// Scan BB for friendly pieces
	for(U16 bit_from = 0; bit_from < 64; bit_from++){
		if (U64GetBit(board->piecesBB[color], 0, bit_from)){

			// Scan piece types
			for(U16 piece = Pawn; piece <= King; piece++){
				if (U64GetBit(board->piecesBB[piece], 0, bit_from)){

					// Pawns
					if (piece == Pawn){

						quiet_BB = global->quietBB[turn][piece][bit_from] & empty_BB;
						capture_BB = global->captureBB[turn][piece][bit_from] & board->piecesBB[opp_color];

						rank_from = bit_from / 8;

						// Double step
						if (rank_from == pawn_starting_rank){

							if (U64GetBit(empty_BB, 0, bit_from + pawn_forward_bitshift) 
								&& U64GetBit(empty_BB, 0, bit_from + 2*pawn_forward_bitshift)){

								configureMove(list+*length, bit_from, bit_from + 2*pawn_forward_bitshift, DoubleStep, Pawn, Null_piece);
								(*length)++;
							}
						}

						// En passant
						else if (rank_from == pawn_EP_rank){

							file_from = bit_from % 8;

							// To the left
							if (isRankFileInBounds(rank_from, file_from - 1))
								if (U16GetBit(board->EPFiles, 0, file_from - 1)){

									configureMove(list+*length, bit_from, bit_from + pawn_EP_left_bitshift, EPCapture, Pawn, Pawn);
									(*length)++;
								}

							// To the right
							if (isRankFileInBounds(rank_from, file_from + 1))
								if (U16GetBit(board->EPFiles, 0, file_from + 1)){

									configureMove(list+*length, bit_from, bit_from + pawn_EP_right_bitshift, EPCapture, Pawn, Pawn);
									(*length)++;
								}
						}

						// Determine list of potential moves (pawn specific for promotions)
						for(U16 bit_to = 0; bit_to < 64; bit_to++){

							// Quiet moves
							if (U64GetBit(quiet_BB, 0, bit_to)){

								// Promotion
								if (rank_from == pawn_promo_rank){
									for (U16 promo = RPromo; promo <= QPromo; promo++){
										configureMove(list+*length, bit_from, bit_to, promo, Pawn, Null_piece);
										(*length)++;
									}
								}
								else{
									configureMove(list+*length, bit_from, bit_to, Quiet, Pawn, Null_piece);
									(*length)++;
								}
							}

							// Captures
							else if (U64GetBit(capture_BB, 0, bit_to)){

								// Determine captured piece
								for(captured = Pawn; captured <= Queen; captured++)
									if (U64GetBit(board->piecesBB[captured], 0, bit_to))
										break;

								// Promotion
								if (rank_from == pawn_promo_rank){
									for (U16 promoCap = RPromoCapture; promoCap <= QPromoCapture; promoCap++){
										configureMove(list+*length, bit_from, bit_to, promoCap, Pawn, captured);
										(*length)++;
									}
								}
								else{
									configureMove(list+*length, bit_from, bit_to, Capture, Pawn, captured);
									(*length)++;
								}
							}
						}
						break;
					}

					// King moves
					else if (piece == King){

						quiet_BB = global->quietBB[turn][piece][bit_from] & empty_BB;
						capture_BB = global->captureBB[turn][piece][bit_from] & board->piecesBB[opp_color];

						// Castling
						if (num_checks == 0){
							if (U16GetBit(board->castlingRights, 0, short_key)){

								if (U64GetBit(empty_BB, 0, bit_from + 1) 
									&& U64GetBit(empty_BB, 0, bit_from + 2)){

									configureMove(list+*length, bit_from, bit_from + 2, ShortCastle, King, Null_piece);
									(*length)++;
								}
							}
							if (U16GetBit(board->castlingRights, 0, long_key)){

								if (U64GetBit(empty_BB, 0, bit_from - 1) 
									&& U64GetBit(empty_BB, 0, bit_from - 2) 
									&& U64GetBit(empty_BB, 0, bit_from - 3)){

									configureMove(list+*length, bit_from, bit_from - 2, LongCastle, King, Null_piece);
									(*length)++;
								}
							}
						}
					}

					//Knight moves
					else if (piece == Knight){

						quiet_BB = global->quietBB[turn][piece][bit_from] & empty_BB;
						capture_BB = global->captureBB[turn][piece][bit_from] & board->piecesBB[opp_color];
					}

					// Bishop, rook, queen moves (the sliding pieces)
					else{

						quiet_BB = 0ULL;
						capture_BB = 0ULL;

						rank_from = bit_from / 8;
						file_from = bit_from % 8;

						for(U16 direction = 0; direction < 8; direction++){

							rank_to = rank_from + delta_rank[direction];
							file_to = file_from + delta_file[direction];

							while(isRankFileInBounds(rank_to, file_to)){

								// If square empty, add to quiet moves BB
								if (U64GetBit(empty_BB, rank_to, file_to)){
									U64SetBit(&quiet_BB, rank_to, file_to, ON);
									rank_to += delta_rank[direction];
									file_to += delta_file[direction];
									continue;
								}
								// If square of opposite color, add to capture BB
								else if (U64GetBit(board->piecesBB[opp_color], rank_to, file_to))
									U64SetBit(&capture_BB, rank_to, file_to, ON);
								break;
							}
						}

						// Compare these masks to piece-specific masks
						quiet_BB &= global->quietBB[turn][piece][bit_from];
						capture_BB &= global->captureBB[turn][piece][bit_from];
					}

					// Determine list of potential moves (for all but pawn)
					for(U16 bit_to = 0; bit_to < 64; bit_to++){

						// Quiet moves
						if (U64GetBit(quiet_BB, 0, bit_to)){

							configureMove(list+*length, bit_from, bit_to, Quiet, piece, Null_piece);
							(*length)++;
						}

						// Captures
						else if (U64GetBit(capture_BB, 0, bit_to)){

							// Determine captured piece
							for(captured = Pawn; captured <= Queen; captured++)
								if (U64GetBit(board->piecesBB[captured], 0, bit_to))
									break;

							configureMove(list+*length, bit_from, bit_to, Capture, piece, captured);
							(*length)++;
						}
					}
					break;
				}
			}
		}
	}
	return list;
}

S16 delta_rank[8] = {0, 1, 1, 1, 0, -1, -1, -1};
S16 delta_file[8] = {1, 1, 0, -1, -1, -1, 0, 1};

Move* legalMoveGenerator(Global* global, Board* board, U16* real_length, U16 num_checks){

	Move* pseudo_list; 
	Move* legal_list;
	U16 pseudo_length;

	pseudo_list = pseudoMoveGenerator(global, board, &pseudo_length, num_checks);
	legal_list = (Move*)malloc(pseudo_length * sizeof(Move));

	*real_length = 0;

	for(U16 i = 0; i < pseudo_length; i++){

		if (validateMove(global, board, pseudo_list[i])){

			/*
			configureMove(legal_list + *real_length, pseudo_list[i].bit_from,
													 pseudo_list[i].bit_to,
													 pseudo_list[i].move_type,
													 pseudo_list[i].moving_piece,
													 pseudo_list[i].captured_piece)
			*/

			legal_list[*real_length] = pseudo_list[i];
			(*real_length)++;
		}
	}

	free(pseudo_list);
	return legal_list;
}

void configureMove(Move* move, U16 bit_from, U16 bit_to, U16 move_type, U16 moving, U16 captured){

	move->bit_from = bit_from;
	move->bit_to = bit_to;
	move->move_type = move_type;
	move->moving_piece = moving;
	move->captured_piece = captured;
	return;
}

U16 validateMove(Global* global, Board* board, Move move){

	// Local variables
	U16 color, opp_color, castle_bit;
	U16 isLegal = 1;

	// White move
	if (board->ply % 2){
		color = White;
		opp_color = Black;
		castle_bit = 4;
	}
	// Black move
	else{
		color = Black;
		opp_color = White;
		castle_bit = 60;
	}

	// Update moving piece
	setPiece(board, color, move.moving_piece, 0, move.bit_from, OFF);
	setPiece(board, color, move.moving_piece, 0, move.bit_to,    ON);

	// Check for captured piece
	if (U16GetBit(move.move_type, 0, 2))
		setPiece(board, opp_color, move.captured_piece, 0, move.bit_to, OFF);

	// Validating if move leaves player in check
	if (move.moving_piece != King){
		
		if (isInCheck(global, board, 64, NO))
			isLegal = 0;
	}

	// King moves
	else{

		// Short castle
		if (move.move_type == ShortCastle){

			if (isInCheck(global, board, castle_bit + 1, YES) || 
				isInCheck(global, board, castle_bit + 2, YES))
				isLegal = 0;
		}

		// Long castle
		else if (move.move_type == LongCastle){

			if (isInCheck(global, board, castle_bit - 1, YES) || 
				isInCheck(global, board, castle_bit - 2, YES))
				isLegal = 0;
		}

		// Not-castle
		else{

			if (isInCheck(global, board, move.bit_to, YES))
				isLegal = 0;
		}
	}

	// Revert changes
	setPiece(board, color, move.moving_piece, 0, move.bit_from, ON);
	setPiece(board, color, move.moving_piece, 0, move.bit_to,  OFF);

	if (U16GetBit(move.move_type, 0, 2))
		setPiece(board, opp_color, move.captured_piece, 0, move.bit_to, ON);

	return isLegal;
}

U16 makeMove(Global* global, Board* board, Move move, U16 do_validate){

	// Ensure move is legal
	if (do_validate)
		if (!validateMove(global, board, move))
			return 0;

	// Local variables
	U16 turn, color, opp_color, file, castle_bit;
	S16 pawn_forward_bitshift;

	// White move
	if (board->ply % 2){
		turn = WhiteTurn;
		color = White;
		opp_color = Black;
		castle_bit = 4;
		pawn_forward_bitshift = 8;
	}
	// Black move
	else{
		turn = BlackTurn;
		color = Black;
		opp_color = White;
		castle_bit = 60;
		pawn_forward_bitshift = -8;
	}

	// Update moving piece
	setPiece(board, color, move.moving_piece, 0, move.bit_from, OFF);
	setPiece(board, color, move.moving_piece, 0, move.bit_to, ON);

	// Check for captured piece
	if (U16GetBit(move.move_type, 0, 2))
		setPiece(board, opp_color, move.captured_piece, 0, move.bit_to, OFF);

	// Update castling flags
	if (move.bit_from == 4){
		U16SetBit(&(board->castlingRights), 0, LongW, OFF);
		U16SetBit(&(board->castlingRights), 0, ShortW, OFF);
	}
	if (move.bit_from == 60){
		U16SetBit(&(board->castlingRights), 0, LongB, OFF);
		U16SetBit(&(board->castlingRights), 0, ShortB, OFF);
	}
	if (move.bit_from == 0 || move.bit_to == 0)
		U16SetBit(&(board->castlingRights), 0, LongW, OFF);

	if (move.bit_from == 7 || move.bit_to == 7)
		U16SetBit(&(board->castlingRights), 0, ShortW, OFF);
	
	if (move.bit_from == 56 || move.bit_to == 56)
		U16SetBit(&(board->castlingRights), 0, LongB, OFF);
	
	if (move.bit_from == 63 || move.bit_to == 63)
		U16SetBit(&(board->castlingRights), 0, ShortB, OFF);

	// Check for promotion
	if (U16GetBit(move.move_type, 0, 3)){

		setPiece(board, color, move.moving_piece, 0, move.bit_to, OFF);

		switch((move.move_type - RPromo) % 4){
						
			case 0:
				setPiece(board, color, Rook, 0, move.bit_to, ON);
				break;
			case 1:
				setPiece(board, color, Knight, 0, move.bit_to, ON);
				break;
			case 2:
				setPiece(board, color, Bishop, 0, move.bit_to, ON);
				break;
			case 3:
				setPiece(board, color, Queen, 0, move.bit_to, ON);
				break;
		}
	}

	// Reset EP flags
	memset(&(board->EPFiles), OFF, 2);

	// Special cases
	switch(move.move_type){

		case DoubleStep:
			file = move.bit_from % 8;
			U16SetBit(&(board->EPFiles), 0, file, ON);
			break;

		case ShortCastle:
			setPiece(board, color, Rook, 0, move.bit_from + 3, OFF);
			setPiece(board, color, Rook, 0, move.bit_from + 1, ON);
			break;

		case LongCastle:
			setPiece(board, color, Rook, 0, move.bit_from - 4, OFF);
			setPiece(board, color, Rook, 0, move.bit_from - 1, ON);
			break;

		case EPCapture:
			setPiece(board, opp_color, Pawn, 0, move.bit_to - pawn_forward_bitshift, OFF);
			break;
	}

	(board->ply)++;
	return 1;
}

void undoMove(Global* global, Board* board, Move move, U16 castlingRights, U16 EPFiles){

	// Countback ply
	(board->ply)--;

	// Local variables
	U16 color, opp_color;
	S16 pawn_forward_bitshift;

	// White move
	if (board->ply % 2){
		color = White;
		opp_color = Black;
		pawn_forward_bitshift = 8;
	}
	// Black move
	else{
		color = Black;
		opp_color = White;
		pawn_forward_bitshift = -8;
	}

	// Update moving piece
	setPiece(board, color, move.moving_piece, 0, move.bit_from, ON);
	setPiece(board, color, move.moving_piece, 0, move.bit_to, OFF);

	// Check for captured piece
	if (U16GetBit(move.move_type, 0, 2))
		setPiece(board, opp_color, move.captured_piece, 0, move.bit_to, ON);

	// Check for promotion
	if (U16GetBit(move.move_type, 0, 3)){

		switch((move.move_type - RPromo) % 4){
						
			case 0:
				setPiece(board, color, Rook, 0, move.bit_to, OFF);
				break;
			case 1:
				setPiece(board, color, Knight, 0, move.bit_to, OFF);
				break;
			case 2:
				setPiece(board, color, Bishop, 0, move.bit_to, OFF);
				break;
			case 3:
				setPiece(board, color, Queen, 0, move.bit_to, OFF);
				break;
		}
	}

	// Special cases
	switch(move.move_type){

		case ShortCastle:
			setPiece(board, color, Rook, 0, move.bit_from + 3, ON);
			setPiece(board, color, Rook, 0, move.bit_from + 1, OFF);
			break;

		case LongCastle:
			setPiece(board, color, Rook, 0, move.bit_from - 4, ON);
			setPiece(board, color, Rook, 0, move.bit_from - 1, OFF);
			break;

		case EPCapture:
			setPiece(board, opp_color, Pawn, 0, move.bit_to - pawn_forward_bitshift, ON);
			break;
	}

	// Restore castling rights, EP files
	board->castlingRights = castlingRights;
	board->EPFiles = EPFiles;

	return;
}

U16 isInCheck(Global* global, Board* board, U16 king_bit, U16 do_knights){

	U16 checks = 0;
	U16 turn, opp_turn, color, opp_color;
	U16 king_rank, king_file, ray_rank, ray_file, piece;

	// White king
	if (board->ply % 2){
		turn = WhiteTurn;
		opp_turn = BlackTurn;
		color = White;
		opp_color = Black;
	}
	// Black king
	else{
		turn = BlackTurn;
		opp_turn = WhiteTurn;
		color = Black;
		opp_color = White;
	}

	// Locate king if location not provided
	if (king_bit == 64)
		for(king_bit = 0; king_bit < 64; king_bit++)
			if (U64GetBit(board->piecesBB[King], 0, king_bit))
				if (U64GetBit(board->piecesBB[color], 0, king_bit))
					break;

	// Only determine if a move is legal through the knights if the king moves on your turn or knight on opposing turn
	if (do_knights){

		U64 knight_checks;

		// Knight checks
		knight_checks = board->piecesBB[opp_color] & board->piecesBB[Knight] & global->captureBB[opp_turn][Knight][king_bit];

		// No checks
		if (knight_checks == 0)
			checks = 0;
		// Power of two; one check
		else if ((knight_checks & (knight_checks - 1)) == 0)
			checks = 1;
		// Not power of two; multiple checks
		else
			return 2;
	}

	// Bitboard for occupied squares
	U64 occupied_BB = board->piecesBB[White] | board->piecesBB[Black];

	// Determine ray checks
	king_rank = king_bit / 8;
	king_file = king_bit % 8;

	for(U16 direction = 0; direction < 8; direction++){

		ray_rank = king_rank + delta_rank[direction];
		ray_file = king_file + delta_file[direction];

		while(isRankFileInBounds(ray_rank, ray_file)){

			// If square occupied, check if opposite color
			if (U64GetBit(occupied_BB, ray_rank, ray_file)){
				if (U64GetBit(board->piecesBB[opp_color], ray_rank, ray_file)){

					// Determine enemy piece
					for(piece = Pawn; piece <= Queen; piece++)
						if (U64GetBit(board->piecesBB[piece], ray_rank, ray_file))
							break;

					// See if piece attacks king
					if (U64GetBit(global->captureBB[opp_turn][piece][8*ray_rank + ray_file], 0, king_bit)){
						checks++;
						if (checks >= 2)
							return 2;
					}
				}
				break;
			}

			ray_rank += delta_rank[direction];
			ray_file += delta_file[direction];
		}
	}
	return checks;
}

U64 perft(Global* global, Board* board, U16 depth){

	Move* move_list;
	U16 length, castling_rights, EP_files;
	U64 total_nodes;

	move_list = legalMoveGenerator(global, board, &length, isInCheck(global, board, 64, YES));

	// Base case
	if (depth == 1)
		return length;

	// Not leaf node
	castling_rights = board->castlingRights;
	EP_files = board->EPFiles;

	for(U16 i = 0; i < length; i++){

		makeMove(global, board, move_list[i], NO);
		total_nodes += perft(global, board, depth-1);
		undoMove(global, board, move_list[i], castling_rights, EP_files);

	}
	return total_nodes;
}

char* moveToUCI(Move move){

	char* UCI_string = (char*)malloc(6*sizeof(char));

	switch(move.bit_from % 8){

		case 0:
			UCI_string[0] = 'a';
			break;
		case 1:
			UCI_string[0] = 'b';
			break;
		case 2:
			UCI_string[0] = 'c';
			break;
		case 3:
			UCI_string[0] = 'd';
			break;
		case 4:
			UCI_string[0] = 'e';
			break;
		case 5:
			UCI_string[0] = 'f';
			break;
		case 6:
			UCI_string[0] = 'g';
			break;
		case 7:
			UCI_string[0] = 'h';
			break;
	}

	UCI_string[1] = (move.bit_from / 8) + 49;

	switch(move.bit_to % 8){
							
		case 0:
			UCI_string[2] = 'a';
			break;
		case 1:
			UCI_string[2] = 'b';
			break;
		case 2:
			UCI_string[2] = 'c';
			break;
		case 3:
			UCI_string[2] = 'd';
			break;
		case 4:
			UCI_string[2] = 'e';
			break;
		case 5:
			UCI_string[2] = 'f';
			break;
		case 6:
			UCI_string[2] = 'g';
			break;
		case 7:
			UCI_string[2] = 'h';
			break;
	}

	UCI_string[3] = (move.bit_to / 8) + 49;

	// Check for promotion
	if (U16GetBit(move.move_type, 0, 3)){

		switch((move.move_type - RPromo) % 4){
							
			case 0:
				UCI_string[4] = 'r';
				break;
			case 1:
				UCI_string[4] = 'n';
				break;
			case 2:
				UCI_string[4] = 'b';
				break;
			case 3:
				UCI_string[4] = 'q';
				break;
		}
		UCI_string[5] = '\0';
		return UCI_string;
	}

	UCI_string[4] = '\0';
	return UCI_string;
}

void movePrinter(Global* global, Board* board){

	Move move;
	Move* moveList;
	U16 length;
	char UCI_string[30], moved[30], move_type[30], promo[30], captured[30];
	moveList = pseudoMoveGenerator(global, board, &length, isInCheck(global, board, 64, YES));

	for(int i = 0; i < length; i++){

		move = moveList[i];
		strcpy(UCI_string, moveToUCI(move));

		switch(move.moving_piece){

			case Pawn:
				strcpy(moved, "Pawn");
				break;
			case Rook:
				strcpy(moved, "Rook");
				break;
			case Knight:
				strcpy(moved, "Knight");
				break;
			case Bishop:
				strcpy(moved, "Bishop");
				break;
			case Queen:
				strcpy(moved, "Queen");
				break;
			case King:
				strcpy(moved, "King");
				break;
		}

		switch(move.move_type){
							
			case Quiet:
				strcpy(move_type, "quiet move");
				break;
			case DoubleStep:
				strcpy(move_type, "double step");
				break;
			case ShortCastle:
				strcpy(move_type, "short castle");
				break;
			case LongCastle:
				strcpy(move_type, "long castle");
				break;
			case Capture:
				strcpy(move_type, "capture");
				break;
			case EPCapture:
				strcpy(move_type, "en passant");
				break;
			case RPromo:
				strcpy(move_type, "to rook promotion");
				break;
			case NPromo:
				strcpy(move_type, "to knight promotion");
				break;
			case BPromo:
				strcpy(move_type, "to bishop promotion");
				break;
			case QPromo:
				strcpy(move_type, "to queen promotion");
				break;
			case RPromoCapture:
				strcpy(move_type, "to rook promotion capture");
				break;
			case NPromoCapture:
				strcpy(move_type, "to knight promotion capture");
				break;
			case BPromoCapture:
				strcpy(move_type, "to bishop promotion capture");
				break;
			case QPromoCapture:
				strcpy(move_type, "to queen promotion capture");
				break;
		}

		// Check for promotion
		if (U16GetBit(move.move_type, 0, 3)){

			switch((move.move_type - RPromo) % 4){
							
				case 0:
					strcpy(promo, "Promo: Rook");
					break;
				case 1:
					strcpy(promo, "Promo: Knight");
					break;
				case 2:
					strcpy(promo, "Promo: Bishop");
					break;
				case 3:
					strcpy(promo, "Promo: Queen");
					break;
			}
		}
		else{
			strcpy(promo, "");
		}

		// Check for capture
		if (U16GetBit(move.move_type, 0, 2)){

			switch(move.captured_piece){
							
				case Pawn:
					strcpy(captured, "Cap: Pawn");
					break;
				case Rook:
					strcpy(captured, "Cap: Rook");
					break;
				case Knight:
					strcpy(captured, "Cap: Knight");
					break;
				case Bishop:
					strcpy(captured, "Cap: Bishop");
					break;
				case Queen:
					strcpy(captured, "Cap: Queen");
					break;
			}
		}
		else{
			strcpy(captured, "");
		}

		// Print out all the information
		printf("%d.: %s %s %s; %s; %s\n", i+1, UCI_string, moved, move_type, promo, captured);
	}
	free(moveList);
	return;
}
