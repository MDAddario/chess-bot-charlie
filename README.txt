## Definitions:

color: Refers to bitboards for white and black piece occupancy

turn: Determines what movesets the pieces have
	turn is computed by (ply % 2), so BlackTurn == 0, WhiteTurn == 1

Rank: Horizontal strips
	Officially indexed by numbers 1-8
	Code corresponds to numbers 0-7

File: Vertical strips
	Officially indexed by letters a-h
	Code corresponds to numbers 0-7

BB: Bitboard
	Refers to a U64 (64 bit unsigned int) to describe the 64 squares of a chess board

CBoard: Chess board

EP: En passant

Capture move: Capturing an opposing piece

Quiet move: No capture upon movement

Double step: Pawn moving two squares from initial rank
	e2e4

bit_from: The bit a moving piece starts on

bit_to: The bit a moving piece ends on

TTable: Transposition table
	Keeps track of positions already analzed.
	
perft: PERFormance Test
