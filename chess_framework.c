#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chess_framework.h"

static inline void U64SetBitOn(U64* number, U8 rank, U8 file){

	*number |= 1ULL << rank*8 + file;
	return;
}

static inline void U64SetBitOff(U64* number, U8 rank, U8 file){

	*number &= ~(1ULL << rank*8 + file);
	return;
}

static inline void U8SetBitOn(U8* number, U8 bit){

	*number |= 1 << bit;
	return;
}

static inline void U8SetBitOff(U8* number, U8 bit){

	*number &= ~(1 << bit);
	return;
}

static inline U8 U64GetBit(U64 number, U8 rank, U8 file){

	return number >> rank*8 + file & 1ULL;
}

static inline U8 U8GetBit(U8 number, U8 bit){

	return number >> bit & 1;
}

/*
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
*/

static inline U8 isPromo(Move move){

	return U8GetBit(move.move_type, 3);
}

static inline U8 isCapture(Move move){

	return U8GetBit(move.move_type, 2);
}

static inline void setCastlingFlagOn(Board* board, U8 bit){

	U8SetBitOn(&(board->castling_flags), bit);
	return;
}

static inline void setCastlingFlagOff(Board* board, U8 bit){

	U8SetBitOff(&(board->castling_flags), bit);
	return;
}

static inline void setEPFlagOn(Board* board, U8 bit){

	U8SetBitOn(&(board->EP_flags), bit);
	return;
}

static inline void setEPFlagOff(Board* board, U8 bit){

	U8SetBitOff(&(board->EP_flags), bit);
	return;
}

static inline U8 getCastlingFlag(Board* board, U8 bit){

	return U8GetBit(board->castling_flags, bit);
}

static inline U8 getEPFlag(Board* board, U8 bit){

	return U8GetBit(board->EP_flags, bit);
}

static inline void clearPiecesBB(Board* board){

	memset(&(board->pieces_BB), 0, 64);
	return;
}

static inline void lowerAllCastlingFlags(Board* board){

	board->castling_flags &= 0xF0;
	return;
}

static inline void raiseAllCastlingFlags(Board* board){

	board->castling_flags |= 0x0F;
	return;
}

static inline void lowerAllEPFlags(Board* board){

	board->EP_flags &= 0x00;
	return;
}

static inline void setPieceOn(Board* board, U8 color, U8 piece, U8 rank, U8 file){

	U64SetBitOn(board->pieces_BB + color, rank, file);
	U64SetBitOn(board->pieces_BB + piece, rank, file);
	return;
}

static inline void setPieceOff(Board* board, U8 color, U8 piece, U8 rank, U8 file){

	U64SetBitOff(board->pieces_BB + color, rank, file);
	U64SetBitOff(board->pieces_BB + piece, rank, file);
	return;
}

void BBPrint(U64 BB){

	for(S8 rank = 7; rank >= 0; rank--){
		printf("[");
		for(U8 file = 0; file < 8; file++){

			if (U64GetBit(BB, rank, file))
				printf("  X  ");
			else
				printf("  .  ");
		}
		printf("]\n");
	}
	return;
}

void BoardStart(Board* board){

	loadFEN(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	return;
}

void BoardPrint(Board* board){

	char color;

	for(S8 rank = 7; rank >= 0; rank--){

		printf("[");
		
		for(U8 file = 0; file < 8; file++){

			color = '\0';
			
			if (U64GetBit(board->pieces_BB[White], rank, file))
				color = 'w';
			else if (U64GetBit(board->pieces_BB[Black], rank, file))
				color = 'b';

			if (color){
				for(U8 piece = Pawn; piece <= King; piece++){
					if (U64GetBit(board->pieces_BB[piece], rank, file)){

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

void loadFEN(Board* board, char* FEN_string){

	// Clear board
	clearPiecesBB(board);

	// Castling rights
	lowerAllCastlingFlags(board);

	// EP rights
	lowerAllEPFlags(board);

	// Parse piece layout
	char* cptr = FEN_string;
	for(S8 rank = 7; rank >= 0; rank--){
		for(U8 file = 0; file < 8;){

			// Empty spaces
			if (49 <= *cptr && *cptr <= 57){
				file += *cptr - 48;
			}

			// Piece
			else{
				switch(*cptr){
							
					case 'P':
						setPieceOn(board, White, Pawn, rank, file);
						break;
					case 'N':
						setPieceOn(board, White, Knight, rank, file);
						break;
					case 'B':
						setPieceOn(board, White, Bishop, rank, file);
						break;
					case 'R':
						setPieceOn(board, White, Rook, rank, file);
						break;
					case 'Q':
						setPieceOn(board, White, Queen, rank, file);
						break;
					case 'K':
						setPieceOn(board, White, King, rank, file);
						break;

					case 'p':
						setPieceOn(board, Black, Pawn, rank, file);
						break;
					case 'n':
						setPieceOn(board, Black, Knight, rank, file);
						break;
					case 'b':
						setPieceOn(board, Black, Bishop, rank, file);
						break;
					case 'r':
						setPieceOn(board, Black, Rook, rank, file);
						break;
					case 'q':
						setPieceOn(board, Black, Queen, rank, file);
						break;
					case 'k':
						setPieceOn(board, Black, King, rank, file);
						break;
				}
				file++;
			}
			cptr++;
		}
		cptr++;
	}

	// Side to move
	if (*cptr == 'w')
		board->ply = 1;
	else
		board->ply = 2;
	cptr += 2;

	// Castling rights
	if (*cptr != '-'){

		while (*cptr != ' ' && *cptr != '\0'){

			switch(*cptr){
							
				case 'K':
					setCastlingFlagOn(board, ShortW);
					break;
				case 'Q':
					setCastlingFlagOn(board, LongW);
					break;
				case 'k':
					setCastlingFlagOn(board, ShortB);
					break;
				case 'q':
					setCastlingFlagOn(board, LongB);
					break;
			}
			cptr++;
		}
	}
	else
		cptr++;
	cptr++;

	// EP flag
	if (*cptr != '-'){
		setEPFlagOn(board, *cptr-97);
		cptr += 2;
	}
	else
		cptr++;

	if (*cptr == '\0')
		return;
	cptr++;

	if (*cptr == '\0')
		return;

	// Halfmove clock DOES NOT CURRENTLY HAVE FUNCTIONALITY HERE.

	cptr++;

	if (*cptr == '\0')
		return;
	cptr++;

	if (*cptr == '\0')
		return;

	// Fullmove counter
	board->ply += 2*(*cptr - 49);
	return;
}

void GlobalLoadBBs(Global* global){

	char* filename = (char*)malloc(200*sizeof(char));
	FILE* fptr;
	U64 BB;

	for(U8 turn = BlackTurn; turn <= WhiteTurn; turn++){

		for(U8 piece = Pawn; piece <= King; piece++){

			strcpy(filename, "./captureBB/");
			strcat(filename, filenames_capture[turn][piece]);

			fptr = fopen(filename, "r");

			if (fptr == NULL){
				printf("Unable to find %s\n", filename);
				return;
			}

			for(U8 bit_from = 0; bit_from < 64; bit_from++){

				fscanf(fptr, "%llu\n", &BB);
				global->capture_BB[turn][piece][bit_from] = BB;
			}
			fclose(fptr);
		}

		for(U8 piece = Pawn; piece <= King; piece++){

			strcpy(filename, "./quietBB/");
			strcat(filename, filenames_quiet[turn][piece]);

			fptr = fopen(filename, "r");

			if (fptr == NULL){
				printf("Unable to find %s\n", filename);
				return;
			}

			for(U8 bit_from = 0; bit_from < 64; bit_from++){

				fscanf(fptr, "%llu\n", &BB);
				global->quiet_BB[turn][piece][bit_from] = BB;
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

U8 isRankFileInBounds(U8 rank, U8 file){

	return rank < 8 && file < 8;	// Thanks to unsigned overflow
}

Move* pseudoMoveGenerator(Global* global, Board* board, U8* length, U8 num_checks){

	// Allocate memory for move list
	Move* list = (Move*)malloc(250*sizeof(Move));
	*length = 0;

	// Bitboard for occupied and empty squares
	U64 occupied_BB = board->pieces_BB[White] | board->pieces_BB[Black];
	U64 empty_BB = ~occupied_BB;

	// Bitboard for potential quiet moves and captures
	U64 quiet_mask, capture_mask;

	// Local variables
	U8 rank_from, file_from, rank_to, file_to, captured, turn, color, opp_color,
		pawn_starting_rank, pawn_EP_rank, pawn_promo_rank, short_key, long_key;
	S8 pawn_forward_bitshift, pawn_EP_left_bitshift, pawn_EP_right_bitshift;

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

		for(U8 bit_from = 0; bit_from < 64; bit_from++)
			if (U64GetBit(board->pieces_BB[King], 0, bit_from))
				if (U64GetBit(board->pieces_BB[color], 0, bit_from)){

					quiet_mask = global->quiet_BB[turn][King][bit_from] & empty_BB;
					capture_mask = global->capture_BB[turn][King][bit_from] & board->pieces_BB[opp_color];

					// Determine list of potential moves
					for(U8 bit_to = 0; bit_to < 64; bit_to++){

						// Quiet moves
						if (U64GetBit(quiet_mask, 0, bit_to)){

							configureMove(list+*length, bit_from, bit_to, Quiet, King, Null_piece);
							(*length)++;
						}

						// Captures
						else if (U64GetBit(capture_mask, 0, bit_to)){

							// Determine captured piece
							for(captured = Pawn; captured <= Queen; captured++)
								if (U64GetBit(board->pieces_BB[captured], 0, bit_to))
									break;

							configureMove(list+*length, bit_from, bit_to, Capture, King, captured);
							(*length)++;
						}
					}
				}
		return list;
	}

	// Scan BB for friendly pieces
	for(U8 bit_from = 0; bit_from < 64; bit_from++){
		if (U64GetBit(board->pieces_BB[color], 0, bit_from)){

			// Scan piece types
			for(U8 piece = Pawn; piece <= King; piece++){
				if (U64GetBit(board->pieces_BB[piece], 0, bit_from)){

					// Pawns
					if (piece == Pawn){

						quiet_mask = global->quiet_BB[turn][piece][bit_from] & empty_BB;
						capture_mask = global->capture_BB[turn][piece][bit_from] & board->pieces_BB[opp_color];

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
								if (getEPFlag(board, file_from-1)){

									configureMove(list+*length, bit_from, bit_from + pawn_EP_left_bitshift, EPCapture, Pawn, Pawn);
									(*length)++;
								}

							// To the right
							if (isRankFileInBounds(rank_from, file_from + 1))
								if (getEPFlag(board, file_from+1)){

									configureMove(list+*length, bit_from, bit_from + pawn_EP_right_bitshift, EPCapture, Pawn, Pawn);
									(*length)++;
								}
						}

						// Determine list of potential moves (pawn specific for promotions)
						for(U8 bit_to = 0; bit_to < 64; bit_to++){

							// Quiet moves
							if (U64GetBit(quiet_mask, 0, bit_to)){

								// Promotion
								if (rank_from == pawn_promo_rank){
									for (U8 promo = RPromo; promo <= QPromo; promo++){
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
							else if (U64GetBit(capture_mask, 0, bit_to)){

								// Determine captured piece
								for(captured = Pawn; captured <= Queen; captured++)
									if (U64GetBit(board->pieces_BB[captured], 0, bit_to))
										break;

								// Promotion
								if (rank_from == pawn_promo_rank){
									for (U8 promoCap = RPromoCapture; promoCap <= QPromoCapture; promoCap++){
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

						quiet_mask = global->quiet_BB[turn][piece][bit_from] & empty_BB;
						capture_mask = global->capture_BB[turn][piece][bit_from] & board->pieces_BB[opp_color];

						// Castling
						if (num_checks == 0){
							if (getCastlingFlag(board, short_key)){

								if (U64GetBit(empty_BB, 0, bit_from + 1) 
									&& U64GetBit(empty_BB, 0, bit_from + 2)){

									configureMove(list+*length, bit_from, bit_from + 2, ShortCastle, King, Null_piece);
									(*length)++;
								}
							}
							if (getCastlingFlag(board, long_key)){

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

						quiet_mask = global->quiet_BB[turn][piece][bit_from] & empty_BB;
						capture_mask = global->capture_BB[turn][piece][bit_from] & board->pieces_BB[opp_color];
					}

					// Bishop, rook, queen moves (the sliding pieces)
					else{

						quiet_mask = 0ULL;
						capture_mask = 0ULL;

						rank_from = bit_from / 8;
						file_from = bit_from % 8;

						for(U8 direction = 0; direction < 8; direction++){

							rank_to = rank_from + delta_rank[direction];
							file_to = file_from + delta_file[direction];

							while(isRankFileInBounds(rank_to, file_to)){

								// If square empty, add to quiet moves BB
								if (U64GetBit(empty_BB, rank_to, file_to)){
									U64SetBitOn(&quiet_mask, rank_to, file_to);
									rank_to += delta_rank[direction];
									file_to += delta_file[direction];
									continue;
								}
								// If square of opposite color, add to capture BB
								else if (U64GetBit(board->pieces_BB[opp_color], rank_to, file_to))
									U64SetBitOn(&capture_mask, rank_to, file_to);
								break;
							}
						}

						// Compare these masks to piece-specific masks
						quiet_mask &= global->quiet_BB[turn][piece][bit_from];
						capture_mask &= global->capture_BB[turn][piece][bit_from];
					}

					// Determine list of potential moves (for all but pawn)
					for(U8 bit_to = 0; bit_to < 64; bit_to++){

						// Quiet moves
						if (U64GetBit(quiet_mask, 0, bit_to)){

							configureMove(list+*length, bit_from, bit_to, Quiet, piece, Null_piece);
							(*length)++;
						}

						// Captures
						else if (U64GetBit(capture_mask, 0, bit_to)){

							// Determine captured piece
							for(captured = Pawn; captured <= Queen; captured++)
								if (U64GetBit(board->pieces_BB[captured], 0, bit_to))
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

S8 delta_rank[8] = {0, 1, 1, 1, 0, -1, -1, -1};
S8 delta_file[8] = {1, 1, 0, -1, -1, -1, 0, 1};

Move* legalMoveGenerator(Global* global, Board* board, U8* real_length, U8 num_checks){

	Move* pseudo_list; 
	Move* legal_list;
	U8 pseudo_length;

	pseudo_list = pseudoMoveGenerator(global, board, &pseudo_length, num_checks);
	legal_list = (Move*)malloc(pseudo_length * sizeof(Move));

	*real_length = 0;

	for(U8 i = 0; i < pseudo_length; i++){

		if (validateMove(global, board, pseudo_list[i])){

			legal_list[*real_length] = pseudo_list[i];
			(*real_length)++;
		}
	}

	legal_list = (Move*)realloc(legal_list, (*real_length) * sizeof(Move));

	free(pseudo_list);
	return legal_list;
}

static inline void configureMove(Move* move, U8 bit_from, U8 bit_to,
									U8 move_type, U8 moving, U8 captured){

	move->bit_from 		 = bit_from;
	move->bit_to 		 = bit_to;
	move->move_type 	 = move_type;
	move->moving_piece 	 = moving;
	move->captured_piece = captured;
	return;
}

U8 validateMove(Global* global, Board* board, Move move){

	// Local variables
	U8 color, opp_color, castle_bit;
	U8 isLegal = 1;
	S8 pawn_forward_bitshift;

	// White move
	if (board->ply % 2){
		color = White;
		opp_color = Black;
		castle_bit = 4;
		pawn_forward_bitshift = 8;
	}
	// Black move
	else{
		color = Black;
		opp_color = White;
		castle_bit = 60;
		pawn_forward_bitshift = -8;
	}

	// Check for captured piece
	if (move.move_type == EPCapture)
		setPieceOff(board, opp_color, Pawn, 0, move.bit_to - pawn_forward_bitshift);
	else if (isCapture(move))
		setPieceOff(board, opp_color, move.captured_piece, 0, move.bit_to);

	// Update moving piece
	setPieceOff(board, color, move.moving_piece, 0, move.bit_from);
	setPieceOn(board, color, move.moving_piece, 0, move.bit_to);

	// Validating if move leaves player in check
	if (move.moving_piece != King){
		
		if (isInCheck(global, board, 64))
			isLegal = 0;
	}

	// King moves
	else{

		// Short castle
		if (move.move_type == ShortCastle){

			if (isInCheck(global, board, castle_bit + 1) || 
				isInCheck(global, board, castle_bit + 2))
				isLegal = 0;
		}

		// Long castle
		else if (move.move_type == LongCastle){

			if (isInCheck(global, board, castle_bit - 1) || 
				isInCheck(global, board, castle_bit - 2))
				isLegal = 0;
		}

		// Not-castle
		else{

			if (isInCheck(global, board, move.bit_to))
				isLegal = 0;
		}
	}

	// Revert changes
	setPieceOn(board, color, move.moving_piece, 0, move.bit_from);
	setPieceOff(board, color, move.moving_piece, 0, move.bit_to);

	// Check for captured piece
	if (move.move_type == EPCapture)
		setPieceOn(board, opp_color, Pawn, 0, move.bit_to - pawn_forward_bitshift);
	else if (isCapture(move))
		setPieceOn(board, opp_color, move.captured_piece, 0, move.bit_to);

	return isLegal;
}

void makeMove(Global* global, Board* board, Move move){

	// Local variables
	U8 turn, color, opp_color, file, castle_bit;
	S8 pawn_forward_bitshift;

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

	// Check for captured piece
	if (move.move_type == EPCapture)
		setPieceOff(board, opp_color, Pawn, 0, move.bit_to - pawn_forward_bitshift);
	else if (isCapture(move))
		setPieceOff(board, opp_color, move.captured_piece, 0, move.bit_to);

	// Update moving piece
	setPieceOff(board, color, move.moving_piece, 0, move.bit_from);
	setPieceOn(board, color, move.moving_piece, 0, move.bit_to);

	// Update castling flags
	if (move.bit_from == 4){						// White king bit
		setCastlingFlagOff(board, LongW);
		setCastlingFlagOff(board, ShortW);
	}
	if (move.bit_from == 60){						// Black king bit
		setCastlingFlagOff(board, LongB);
		setCastlingFlagOff(board, ShortB);
	}
	if (move.bit_from == 0 || move.bit_to == 0)		// White queen rook bit
		setCastlingFlagOff(board, LongW);

	if (move.bit_from == 7 || move.bit_to == 7)		// White king rook bit
		setCastlingFlagOff(board, ShortW);
	
	if (move.bit_from == 56 || move.bit_to == 56)	// Black queen rook bit
		setCastlingFlagOff(board, LongB);
	
	if (move.bit_from == 63 || move.bit_to == 63)	// Black king rook bit
		setCastlingFlagOff(board, ShortB);

	// Check for promotion
	if (isPromo(move)){

		U64SetBitOff(board->pieces_BB + Pawn, 0, move.bit_to);

		switch((move.move_type - RPromo) % 4){
						
			case 0:
				U64SetBitOn(board->pieces_BB + Rook, 0, move.bit_to);
				break;
			case 1:
				U64SetBitOn(board->pieces_BB + Knight, 0, move.bit_to);
				break;
			case 2:
				U64SetBitOn(board->pieces_BB + Bishop, 0, move.bit_to);
				break;
			case 3:
				U64SetBitOn(board->pieces_BB + Queen, 0, move.bit_to);
				break;
		}
	}

	// Reset EP flags
	lowerAllEPFlags(board);

	// Special cases
	switch(move.move_type){

		case DoubleStep:
			file = move.bit_from % 8;
			setEPFlagOn(board, file);
			break;

		case ShortCastle:
			setPieceOff(board, color, Rook, 0, move.bit_from + 3);
			setPieceOn(board, color, Rook, 0, move.bit_from + 1);
			break;

		case LongCastle:
			setPieceOff(board, color, Rook, 0, move.bit_from - 4);
			setPieceOn(board, color, Rook, 0, move.bit_from - 1);
			break;
	}

	(board->ply)++;
	return;
}

void undoMove(Global* global, Board* board, Move move, U8 castling_flags, U8 EP_flags){

	// Countback ply
	(board->ply)--;

	// Local variables
	U8 color, opp_color;
	S8 pawn_forward_bitshift;

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
	setPieceOn(board, color, move.moving_piece, 0, move.bit_from);
	setPieceOff(board, color, move.moving_piece, 0, move.bit_to);

	// Check for captured piece
	if (move.move_type == EPCapture)
		setPieceOn(board, opp_color, Pawn, 0, move.bit_to - pawn_forward_bitshift);
	else if (isCapture(move))
		setPieceOn(board, opp_color, move.captured_piece, 0, move.bit_to);

	// Check for promotion
	if (isPromo(move)){

		switch((move.move_type - RPromo) % 4){
						
			case 0:
				U64SetBitOff(board->pieces_BB + Rook, 0, move.bit_to);
				break;
			case 1:
				U64SetBitOff(board->pieces_BB + Knight, 0, move.bit_to);
				break;
			case 2:
				U64SetBitOff(board->pieces_BB + Bishop, 0, move.bit_to);
				break;
			case 3:
				U64SetBitOff(board->pieces_BB + Queen, 0, move.bit_to);
				break;
		}
	}

	// Special cases
	switch(move.move_type){

		case ShortCastle:
			setPieceOn(board, color, Rook, 0, move.bit_from + 3);
			setPieceOff(board, color, Rook, 0, move.bit_from + 1);
			break;

		case LongCastle:
			setPieceOn(board, color, Rook, 0, move.bit_from - 4);
			setPieceOff(board, color, Rook, 0, move.bit_from - 1);
			break;
	}

	// Restore castling rights, EP files
	board->castling_flags = castling_flags;
	board->EP_flags = EP_flags;

	return;
}

U8 isInCheck(Global* global, Board* board, U8 king_bit){

	U8 checks, turn, opp_turn, color, opp_color, 
		king_rank, king_file, ray_rank, ray_file, piece;
	U64 knight_checks;

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
			if (U64GetBit(board->pieces_BB[King], 0, king_bit))
				if (U64GetBit(board->pieces_BB[color], 0, king_bit))
					break;

	// Knight checks
	knight_checks = board->pieces_BB[opp_color] & board->pieces_BB[Knight] & global->capture_BB[opp_turn][Knight][king_bit];

	// No checks
	if (knight_checks == 0)
		checks = 0;
	// Power of two; one check
	else if ((knight_checks & (knight_checks - 1)) == 0)
		checks = 1;
	// Not power of two; multiple checks
	else
		return 2;

	// Bitboard for occupied squares
	U64 occupied_BB = board->pieces_BB[White] | board->pieces_BB[Black];

	// Determine ray checks
	king_rank = king_bit / 8;
	king_file = king_bit % 8;

	for(U8 direction = 0; direction < 8; direction++){

		ray_rank = king_rank + delta_rank[direction];
		ray_file = king_file + delta_file[direction];

		while(isRankFileInBounds(ray_rank, ray_file)){

			// If square occupied, check if opposite color
			if (U64GetBit(occupied_BB, ray_rank, ray_file)){
				if (U64GetBit(board->pieces_BB[opp_color], ray_rank, ray_file)){

					// Determine enemy piece
					for(piece = Pawn; piece <= Queen; piece++)
						if (U64GetBit(board->pieces_BB[piece], ray_rank, ray_file))
							break;

					// See if piece attacks king
					if (U64GetBit(global->capture_BB[opp_turn][piece][8*ray_rank + ray_file], 0, king_bit)){
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

void perftResults(Global* global, Board* board, U64** results, U8 current_depth, U8 max_depth){

	Move* move_list;
	U8 length, castling_rights, EP_files, num_checks;

	// Update results from last layer
	if (current_depth == 0){

		num_checks = isInCheck(global, board, 64);
		move_list = legalMoveGenerator(global, board, &length, num_checks);
	}
	else{

		// Checks
		num_checks = isInCheck(global, board, 64);
		if (num_checks)
			results[current_depth-1][5] += 1;

		// Double checks
		if (num_checks == 2)
			results[current_depth-1][6] += 1;

		// Checkmates
		move_list = legalMoveGenerator(global, board, &length, num_checks);
		if (length == 0)
			results[current_depth-1][7] += 1;

		/*
		CURRENTLY NO DISCREPANCY BETWEEN CHECKMATES AND STALEMATES
		*/
	}

	// Nodes
	results[current_depth][0] += length;

	// Update results array
	for(U8 i = 0; i < length; i++){

		// Captures
		if (isCapture(move_list[i]))
			results[current_depth][1] += 1;

		// EP
		if (move_list[i].move_type == EPCapture)
			results[current_depth][2] += 1;

		// Castles
		if (move_list[i].move_type == ShortCastle || move_list[i].move_type == LongCastle)
			results[current_depth][3] += 1;

		// Promotions
		if (isPromo(move_list[i]))
			results[current_depth][4] += 1;
	}

	// Base case
	if (current_depth == max_depth-1){
		free(move_list);
		return;
	}

	// Not a leaf node
	castling_rights = board->castling_flags;
	EP_files 		= board->EP_flags;

	for(U8 i = 0; i < length; i++){

		makeMove(global, board, move_list[i]);
		perftResults(global, board, results, current_depth + 1, max_depth);
		undoMove(global, board, move_list[i], castling_rights, EP_files);
	}

	free(move_list);
	return;
}

U64 perftBenchmark(Global* global, Board* board, U8 depth){

	U8 length, castling_rights, EP_files;
	U64 total_nodes = 0;
	Move* move_list = legalMoveGenerator(global, board, &length, isInCheck(global, board, 64));

	// Base case
	if (depth == 1){
		free(move_list);
		return length;
	}

	// Not a leaf node
	castling_rights = board->castling_flags;
	EP_files 		= board->EP_flags;

	for(U8 i = 0; i < length; i++){

		makeMove(global, board, move_list[i]);
		total_nodes += perftBenchmark(global, board, depth-1);
		undoMove(global, board, move_list[i], castling_rights, EP_files);
	}

	free(move_list);
	return total_nodes;
}

void initPerft(Global* global, Board* board, U8 max_depth, U8 do_results){

	// Ensure legitimate input
	if (max_depth == 0){
		printf("Please provide a non-zero depth.\n");
		return;
	}

	if (!do_results){

		clock_t t = clock();

		U64 total_nodes = perftBenchmark(global, board, max_depth);

		t = clock() - t;
		double time_taken = ((double)t)/CLOCKS_PER_SEC;

		printf("Total number of nodes: %llu\n", total_nodes);
		printf("Time taken to execute: %lf s\n", time_taken); 

		return;
	}

	// Store perft() results in array
	U64** results = (U64**)malloc(max_depth * sizeof(U64*));
	for(U8 i = 0; i < max_depth; i++)
		results[i] = (U64*)calloc(8, sizeof(U64));

	/*	Result codes:
	 *	0: Nodes
	 *	1: Captures
	 *	2: EPs
	 *	3: Castles
	 *	4: Promotions
	 *	5: Checks
	 *	6: Double checks
	 *	7: Checkmates
	 */

	perftResults(global, board, results, 0, max_depth);

	// Print output
	printf("%5s %11s %11s %11s %11s %11s %11s %11s %11s\n",
			"Depth", "Nodes", "Captures", "EPs", "Castles", "Promos", "Checks", "2Checks", "Checkmates");
	for(U8 i = 0; i < max_depth; i++)

		if (i == max_depth-1)
			printf("%5u %11llu %11llu %11llu %11llu %11llu %11s %11s %11s\n",
					i+1, results[i][0], results[i][1], results[i][2], results[i][3], 
					results[i][4], "---", "---", "---");

		else
			printf("%5u %11llu %11llu %11llu %11llu %11llu %11llu %11llu %11llu\n",
					i+1, results[i][0], results[i][1], results[i][2], results[i][3], 
					results[i][4], results[i][5], results[i][6], results[i][7]);

	// Free memory
	for(U8 i = 0; i < max_depth; i++)
		free(results[i]);
	free(results);

	return;
}

void longPerftDebug(Global* global, Board* board){

	/*
	https://www.chessprogramming.org/Perft_Results
	*/

	U64 accepted_node_counts[] = {119060324ULL,
								  193690690ULL,
								  178633661ULL,
								  15833292ULL,
								  89941194ULL,
								  164075551ULL};

	U8 accepted_depths[] = {6,
							5,
							7,
							5,
							5,
							5};

	char* FEN_strings[] = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
						   "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
						   "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -",
						   "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
						   "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
						   "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"};

	U8 length = 6;

	U64 total_nodes;
	for(U8 i = 3; i < length; i++){

		printf("Performing perft #%hu / %hu...\n", i+1, length);
		loadFEN(board, FEN_strings[i]);
		total_nodes = perftBenchmark(global, board, accepted_depths[i]);

		if (total_nodes == accepted_node_counts[i]){
			printf("Perft #%hu successful.\n\n", i+1);
			continue;
		}
		else{
			printf("Perft #%hu failed.\n\n", i+1);
			printf("Program determined %llu nodes.\n", total_nodes);
			printf("Accepted value is %llu nodes.\n", accepted_node_counts[i]);
			return;
		}
	}
	printf("Successfully completed all %hu perfts.\n", length);
	return;
}

void compareBoards(Board* board_1, Board* board_2, char* move_name, char* func_name){

	// Compare bitboards
	for(U8 i = Pawn; i <= Black; i++)
		if (board_1->pieces_BB[i] != board_2->pieces_BB[i]){

			printf("%s error for %s:\n", func_name, move_name);
			printf("\t-> Difference in bitboard %hu.\n", i);
		}

	// Compare remaining fields
	if (board_1->castling_flags != board_2->castling_flags){

		printf("%s error for %s:\n", func_name, move_name);
		printf("\t-> Difference in castling rights.\n");
	}
	if (board_1->EP_flags != board_2->EP_flags){

		printf("%s error for %s:\n", func_name, move_name);
		printf("\t-> Difference in EP files.\n");
	}
	if (board_1->ply != board_2->ply){

		printf("%s error for %s:\n", func_name, move_name);
		printf("\t-> Difference in ply.\n");
	}

	return;
}

void debugBoard(Global* global, Board* board, Move move, char* move_name){

	// Make copy of board
	Board* original_board = (Board*)malloc(sizeof(Board));
	memcpy(original_board, board, sizeof(Board));

	// isInCheck()
	U8 num_checks = isInCheck(global, board, 64);
	compareBoards(board, original_board, move_name, "isInCheck()");

	// validateMove()
	validateMove(global, board, move);
	compareBoards(board, original_board, move_name, "validateMove()");

	// legalMoveGenerator()
	U8 length;
	legalMoveGenerator(global, board, &length, num_checks);
	compareBoards(board, original_board, move_name, "legalMoveGenerator()");

	// makeMove() && undoMove()
	U8 castling_rights = board->castling_flags;
	U8 EP_files = board->EP_flags;
	makeMove(global, board, move);
	undoMove(global, board, move, castling_rights, EP_files);
	compareBoards(board, original_board, move_name, "makeMove() && undoMove()");

	return;
}

U8 bitFromAlgeb(char* algebraic){

	U8 file = algebraic[0] - 97;
	U8 rank = algebraic[1] - 1;

	return 8*rank + file;
}

void bitTesting(){

	Move move;
	for(move.move_type = 0; move.move_type <= 15; move.move_type++){

		printf("move_type = %hu", move.move_type);

		if (isPromo(move))
			printf("\t\tPromotion == Ye");
		else
			printf("\t\tPromotion == No");

		if (isCapture(move))
			printf("\t\tCapture == Ye\n");
		else
			printf("\t\tCapture == No\n");

	}
	return ;
}

void unitTests(Global* global, Board* board){

	// Debug chess functions
	Move move;

	// Quiet test
	printf("==========================\n");
	printf("QUIET TEST\n");
	printf("==========================\n");
	BoardStart(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e7"), bitFromAlgeb("e5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("b1"), bitFromAlgeb("c3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nc3");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("b8"), bitFromAlgeb("c6"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nc6");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("g1"), bitFromAlgeb("h3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nh3");
	makeMove(global, board, move);

	//BoardPrint(board);


	// Capture test
	printf("==========================\n");
	printf("CAPTURE TEST\n");
	printf("==========================\n");
	BoardStart(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e7"), bitFromAlgeb("e5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("g1"), bitFromAlgeb("f3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nf3");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("g8"), bitFromAlgeb("f6"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nf6");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("f3"), bitFromAlgeb("e5"), Capture, Knight, Pawn);
	debugBoard(global, board, move, "Nxe5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("f6"), bitFromAlgeb("e4"), Capture, Knight, Pawn);
	debugBoard(global, board, move, "Nxe4");
	makeMove(global, board, move);

	//BoardPrint(board);


	// Check test
	printf("==========================\n");
	printf("CHECK TEST\n");
	printf("==========================\n");
	BoardStart(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("f7"), bitFromAlgeb("f5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "f5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("d1"), bitFromAlgeb("h5"), Quiet, Queen, Null_piece);
	debugBoard(global, board, move, "Qh5+");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("g7"), bitFromAlgeb("g6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "g6");
	makeMove(global, board, move);

	//BoardPrint(board);


	// Scholars mate test
	printf("==========================\n");
	printf("SCHOLARS MATE TEST\n");
	printf("==========================\n");
	BoardStart(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("a7"), bitFromAlgeb("a5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "a5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("f1"), bitFromAlgeb("c4"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bc4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("b7"), bitFromAlgeb("b6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "b6");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("d1"), bitFromAlgeb("f3"), Quiet, Queen, Null_piece);
	debugBoard(global, board, move, "Qf3");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("h7"), bitFromAlgeb("h6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "h6");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("f3"), bitFromAlgeb("f7"), Capture, Queen, Pawn);
	debugBoard(global, board, move, "Qf7#");
	makeMove(global, board, move);

	//BoardPrint(board);


	// Short castle test
	printf("==========================\n");
	printf("SHORT CASTLE TEST\n");
	printf("==========================\n");
	BoardStart(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e7"), bitFromAlgeb("e5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("g1"), bitFromAlgeb("f3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nf3");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("g8"), bitFromAlgeb("f6"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Nf6");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("f1"), bitFromAlgeb("b5"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bb5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("f8"), bitFromAlgeb("b4"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bb4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e1"), bitFromAlgeb("g1"), ShortCastle, King, Null_piece);
	debugBoard(global, board, move, "O-O");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e8"), bitFromAlgeb("g8"), ShortCastle, King, Null_piece);
	debugBoard(global, board, move, "O-O");
	makeMove(global, board, move);

	//BoardPrint(board);


	// Long castle test
	printf("==========================\n");
	printf("LONG CASTLE TEST\n");
	printf("==========================\n");
	BoardStart(board);

	configureMove(&move, bitFromAlgeb("d2"), bitFromAlgeb("d4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "d4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("d7"), bitFromAlgeb("d5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "d5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("b1"), bitFromAlgeb("a3"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Na3");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("b8"), bitFromAlgeb("a6"), Quiet, Knight, Null_piece);
	debugBoard(global, board, move, "Na6");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("c1"), bitFromAlgeb("g5"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bg5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("c8"), bitFromAlgeb("g4"), Quiet, Bishop, Null_piece);
	debugBoard(global, board, move, "Bg4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("d1"), bitFromAlgeb("d2"), Quiet, Queen, Null_piece);
	debugBoard(global, board, move, "Qd2");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("d8"), bitFromAlgeb("d7"), Quiet, Queen, Null_piece);
	debugBoard(global, board, move, "Qd7");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e1"), bitFromAlgeb("c1"), LongCastle, King, Null_piece);
	debugBoard(global, board, move, "O-O-O");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e8"), bitFromAlgeb("c8"), LongCastle, King, Null_piece);
	debugBoard(global, board, move, "O-O-O");
	makeMove(global, board, move);

	//BoardPrint(board);


	// En passant test
	printf("==========================\n");
	printf("EN PASSANT TEST\n");
	printf("==========================\n");
	BoardStart(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("a7"), bitFromAlgeb("a5"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "a5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e4"), bitFromAlgeb("e5"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("d7"), bitFromAlgeb("d5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "d5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e5"), bitFromAlgeb("d6"), EPCapture, Pawn, Pawn);
	debugBoard(global, board, move, "exd6e.p.");
	makeMove(global, board, move);

	//BoardPrint(board);


	// Promotion test
	printf("==========================\n");
	printf("PROMOTION TEST\n");
	printf("==========================\n");
	BoardStart(board);

	configureMove(&move, bitFromAlgeb("e2"), bitFromAlgeb("e4"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "e4");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("a7"), bitFromAlgeb("a5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "a5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e4"), bitFromAlgeb("e5"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "e5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("b7"), bitFromAlgeb("b5"), DoubleStep, Pawn, Null_piece);
	debugBoard(global, board, move, "b5");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e5"), bitFromAlgeb("e6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "e6");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("d7"), bitFromAlgeb("d6"), Quiet, Pawn, Null_piece);
	debugBoard(global, board, move, "d6");
	makeMove(global, board, move);
	
	configureMove(&move, bitFromAlgeb("e6"), bitFromAlgeb("f7"), Capture, Pawn, Pawn);
	debugBoard(global, board, move, "exf7+");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("e8"), bitFromAlgeb("d7"), Quiet, King, Null_piece);
	debugBoard(global, board, move, "Kd7");
	makeMove(global, board, move);

	configureMove(&move, bitFromAlgeb("f7"), bitFromAlgeb("g8"), QPromoCapture, Pawn, Knight);
	debugBoard(global, board, move, "fxg8N");
	makeMove(global, board, move);

	//BoardPrint(board);

	BoardStart(board);
	return;
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
	if (isPromo(move)){

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
	U8 length;
	char UCI_string[30], moved[30], move_type[30], promo[30], captured[30];
	moveList = legalMoveGenerator(global, board, &length, isInCheck(global, board, 64));

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
		if (isPromo(move)){

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
		if (isCapture(move)){

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
		printf("%d.: %s %s %s; %s; %s\n", i, UCI_string, moved, move_type, promo, captured);
	}
	free(moveList);
	return;
}
