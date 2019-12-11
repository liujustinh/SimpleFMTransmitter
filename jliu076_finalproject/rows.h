/*
 * rows.h
 *
 * Created: 6/4/2018 8:32:47 PM
 *  Author: Justin
 */ 


#ifndef ROWS_H_
#define ROWS_H_

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

unsigned short amp_value; 
unsigned short prev_amp_value; 
unsigned short base_mic_value = 512;
unsigned short prev_mic_value;
unsigned short cur_mic_value;
unsigned short difference; 
unsigned short change[]; 

void reset_rows(row** rows) {
	for (int i = 0; i < 8; ++i) {
		rows[i]->blueRow = 0xFF;
	}
}

void microphone_tick(row** rows) {
	prev_amp_value = amp_value;
	prev_mic_value = cur_mic_value; 
	cur_mic_value = readADC(2);
	amp_value = readADC(3);
	difference = abs(base_mic_value - cur_mic_value); 
	unsigned short change = amp_value - prev_amp_value; 
	/*if (change) {
		for (int i = 0; i < 8; ++i) {
			change[i] = readADC(3); 	
		}
	}*/
	
	if (difference > 15 && difference <= 20) {
		rows[7]->blueRow = 0xE7;
		
		rows[0]->blueRow = 0xE7;
	}
	else if (difference > 20 && difference <= 25) {
		rows[6]->blueRow = 0xE7;
		rows[7]->blueRow = 0xC3;
		
		rows[0]->blueRow = 0xC3;
		rows[1]->blueRow = 0xE7;
	}
	else if (difference > 25 && difference <= 30) {
		rows[5]->blueRow = 0xE7;
		rows[6]->blueRow = 0xC3;
		rows[7]->blueRow = 0x81;
		
		rows[2]->blueRow = 0xE7;
		rows[1]->blueRow = 0xC3;
		rows[0]->blueRow = 0x81;
	}
	else if (difference > 30) {
		rows[4]->blueRow = 0xE7;
		rows[5]->blueRow = 0xC3;
		rows[6]->blueRow = 0x81;
		rows[7]->blueRow = 0x00;
		
		rows[3]->blueRow = 0xE7;
		rows[2]->blueRow = 0xC3;
		rows[1]->blueRow = 0x81;
		rows[0]->blueRow = 0x00;
	}
	else {
		for (int i = 0; i < 8; ++i) {
			rows[i]->blueRow = 0xFF;
		}
	}
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

#endif /* ROWS_H_ */