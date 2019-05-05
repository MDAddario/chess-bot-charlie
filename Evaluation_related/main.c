#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chess_engine.h"

int main(){

	GlobalLoadValues(global);
	GlobalLoadTables(global);
	GlobalLoadZobristKeys(global);
	
	return 0;
}
