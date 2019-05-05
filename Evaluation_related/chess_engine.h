#define TTABLE_SIZE 100000

// Each entry in the tranposition table
typedef struct StructPosition{

	U16 isVisited;
	// Boolean for existence

	U64 ZobristKey;
	// Complete hash key
	
	Move optimalMove;
	// Optimal move previously determined
	
	S16 score;
	// Score of node

} Position;

// Global memory for static chess related values and transposition table
typedef struct StructChessGlobal{

	U16 pieceValues[6];
	// Six for pieces
	// Value of each piece

	S16 pieceSquareTables[6][64];
	// Six for pieces
	// 64, for each position

	U64 zobristPieceKeys[12][64];
	// Six for white pieces
	// Six for black pieces
	// 64, for each position

	U64 zobristBlackMoveKey;
	// Alternating moves

	U64 zobristCastlingKeys[4];
	// White short, long
	// Black short, long

	U64 zobristEPFileKeys[8];
	// 8 files

	Position TTable[TTABLE_SIZE];
	// Transposition table 

} Global;

// Zobrist piece types
enum ZobristPieceType{
	PawnW,		// 0
	RookW,		// 1
	KnightW,	// 2
	BishopW,	// 3
	QueenW,		// 4
	KingW,		// 5
	PawnB,		// 6
	RookB,		// 7
	KnightB,	// 8
	BishopB,	// 9
	QueenB,		// 10
	KingB 		// 11
};

// Load piece values
void GlobalLoadValues(Global*);

// Load piece-square tables
void GlobalLoadTables(Global*);

// Piece values and piece-square tables retrieved from:
// https://www.chessprogramming.org/Simplified_Evaluation_Function

// Generate the Zobrist keys used to generate hashes
void GlobalLoadZobristKeys(Global*);
/* Terminology:
 *
 *	Zobrist keys 	-> XOR 			-> Code
 *	Hash keys 		-> Hash Map 	-> Hash
 */

// Test to see if all elements loaded properly (debug code)
void GlobalTest(Global*);

// Generate random 64 bit integer
U64 randU64();
