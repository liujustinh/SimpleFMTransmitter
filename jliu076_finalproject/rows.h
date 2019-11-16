/*
 * rows.h
 *
 * Created: 6/4/2018 8:32:47 PM
 *  Author: Justin
 */ 


#ifndef ROWS_H_
#define ROWS_H_

/*
 * rows.c
 *
 * Created: 6/4/2018 8:07:36 PM
 *  Author: Justin
 */ 
#define blue 1
#define red 2
#define column 3
#define left_direction 6
#define right_direction 7
#define MAX_MOVES 56
#define HEIGHT 8
#define WIDTH 8

int winner; 
int player1_moves = 0; 
int player2_moves = 0; 
int full_board = 0; 
int turn_error = 0;
int player1_pos = 3;
int player2_pos = 4;
int placement_error = 0; 
unsigned char prev_red, prev_blue;  

int game_table[HEIGHT][WIDTH]; 

void init_gametable() {
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			game_table[i][j] = 0;
		}
	}
}


typedef struct row {
	unsigned char redRow; 
	unsigned char blueRow; 
	unsigned char col; 
	} row;

void row_init(row** rows) {					//initializes an empty LED matrix to start the game
	for (int i = 0; i < 8; ++i) {
		rows[i]->blueRow = 0xFF; 
		rows[i]->redRow = 0xFF; 
	}
	rows[0]->col = 0x01; 
	rows[1]->col = 0x02; 
	rows[2]->col = 0x04; 
	rows[3]->col = 0x08; 
	rows[4]->col = 0x10; 
	rows[5]->col = 0x20; 
	rows[6]->col = 0x40; 
	rows[7]->col = 0x80; 
}

void row_display(row** rows) {				//displays the game
	for (int i = 0; i < 8; ++i) {
		transmit_data(column, rows[i]->col);
		transmit_data(blue, rows[i]->blueRow);
		transmit_data(red, 0xFF);
		
		transmit_data(column, rows[i]->col);
		transmit_data(red, rows[i]->redRow);
		transmit_data(blue, 0xFF);
		
		pulseLED(); 
	}
}

void moveBlock(int player, row** rows, int direction) {
	if (player == 1) {
		if (direction == right_direction) {
			if (player1_pos == 0) {
				turn_error = 1; 
				return; 
			}
			else {
				rows[0]->blueRow = shiftRight(rows[0]->blueRow);
				--player1_pos;
				return;
			}
		}
		else if (direction == left_direction) {
			if (player1_pos == 7) {
				turn_error = 1; 
				return; 
			}
			else {
				rows[0]->blueRow = shiftLeft(rows[0]->blueRow);
				++player1_pos; 
				return; 
			}
		}
	}
	else if (player == 2) {
		if (direction == right_direction) {
			if(player2_pos == 0) {
				turn_error = 1; 
				return; 
			}
			else {
				rows[0]->redRow = shiftRight(rows[0]->redRow);
				--player2_pos;
				return;
			}
		}
		else if (direction == left_direction) {
			if (player2_pos == 7) {
				turn_error = 1; 
				return; 
			}
			else {
				rows[0]->redRow = shiftLeft(rows[0]->redRow);
				++player2_pos; 
			}	return; 
		}
	}
}

void placeBlock(int player, row** rows) {

	int player_pos = 0; 
	if (player == 1) {
		player_pos = player1_pos; 
	}
	else if (player == 2) {
		player_pos = player2_pos; 
	}
	int row_pos = 0; 
	for (int i = 1; i < 8; ++i) {		//made change here that can break the code
		if (GetBit(rows[i]->blueRow, player_pos) == 1 && GetBit(rows[i]->redRow, player_pos) == 1) {
			row_pos = row_pos + 1; 
		}
	}
	if (row_pos == 0) {
		placement_error = 1; 
		return; 
	}
	if (player == 1) {
		rows[row_pos]->blueRow = SetBit(rows[row_pos]->blueRow, player_pos, 0); 
		game_table[row_pos][player_pos] = 1;										//append player move to game table
		player1_moves = player1_moves + 1; 
	}
	else if (player == 2) {
		rows[row_pos]->redRow = SetBit(rows[row_pos]->redRow, player_pos, 0); 
		game_table[row_pos][player_pos] = 2;										//append player move to game table
		player2_moves = player2_moves + 1; 
	}
	//checkWin(); 
	checkFull(rows); 
	rows[0]->blueRow = 0xFF;
	rows[0]->redRow = 0xFF;
}

void newTurn(int player, row** rows) {
	rows[0]->blueRow = 0xFF; 
	rows[0]->redRow = 0xFF; 
	player1_pos = 4; 
	player2_pos = 3; 
	if (player == 1) {
		rows[0]->blueRow = 0xEF; 
	}
	else if (player == 2) {
		rows[0]->redRow = 0xF7; 
	}
}

void checkFull(row** rows) {
	unsigned int total_moves = player1_moves + player2_moves; 
	if (total_moves >= MAX_MOVES) {
		full_board = 1; 
	}
}

void checkWin() {
	for (int i = 1; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			int player_check = game_table[i][j]; 
			if (player_check != 1 && player_check != 2) {
				continue;
			}
			if (j + 3 < WIDTH) {
				if (player_check == game_table[i][j+1] && player_check == game_table[i][j+2] && player_check == game_table[i][j+3]) {
					winner = player_check; 
					return; 
				}
			}
			if (i + 3 < HEIGHT) {
				if (player_check == game_table[i+1][j] && player_check == game_table[i+2][j] && player_check == game_table[i+3][j]) {
					winner = player_check; 
					return; 
				}
				if (j + 3 < WIDTH) {
					if (player_check == game_table[i+1][j+1] && player_check == game_table[i+2][j+2] && player_check == game_table[i+3][j+3]) {
						winner = player_check; 
						return; 
					}
				}
				if (j - 3 >= 0) {
					if (player_check == game_table[i+1][j-1] && player_check == game_table[i+2][j-2] && player_check == game_table[i+3][j-3]) {
						winner = player_check; 
						return; 
					}
				}
			}	
		}
	}
}




#endif /* ROWS_H_ */