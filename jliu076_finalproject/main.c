
/////////////////////////////////////////////////////////////////////////////
//////////////////////////CS122A FINAL PROJECT///////////////////////////////
//////////////////////////SimpleFMTransmitter////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/io.h>

#include "bit.h"
#include "matrix_write.h"
#include "nokia5110.h"
#include "joystick.h"
#include "timer.h"
#include "tasks.h"
#include "rows.h"
//#include "usart_ATmega1284.h"
#include "spi.h"

#define START (~PINB & 0x08)
#define BACK (~PINB & 0x10)
#define BT_CONNECTED (~PINB & 0x01)
#define BT_DISCONNECTED (~PINB & 0x02)
#define BT_READY (~PINB & 0x04)

unsigned int bt_ready = 0;
unsigned int bt_paired = 0; 
unsigned int mode = 0; 
unsigned int led_matrix_on = 1; 
unsigned short test_value; 
unsigned int led_matrix_color = 1; //blue = 1, red = 2

///////INITIALIZE MATRIX COLUMN / ROWS//////////
row ROW_0, ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7;
row *rows[8] = { &ROW_0, &ROW_1, &ROW_2, &ROW_3, &ROW_4, &ROW_5, &ROW_6, &ROW_7};
	
	
///////STATE MACHINE 1: User menu interface/////////
enum SM1_States {SM1_Init, SM1_Wait1, SM1_Wait2, SM1_Wait3, SM1_Start, SM1_LEDMenu1, SM1_LEDMenu2} SM1_State;
int SM1_Menu(int SM1_State) {
	switch(SM1_State) {
		case -1: 
			SM1_State = SM1_Init; 
			break; 
		case SM1_Init:
			SM1_State = SM1_Wait1; 
			break;   
		case SM1_Wait1:
			if (down == true) {
				nokia_lcd_clear();
				//nokia_lcd_set_cursor(0, 15);
				//nokia_lcd_write_string(integer_to_string(test_value), 1);
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(0, 35);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				if (bt_paired == 1) {
					nokia_lcd_set_cursor(20, 9);
					nokia_lcd_write_string("Paired", 1);
				}
				else if (bt_ready == 1) {
					nokia_lcd_set_cursor(3, 9);
					nokia_lcd_write_string("Ready to pair!", 1);
				}
				nokia_lcd_render();
				SM1_State = SM1_Wait2; 
			}
			else if (BT_READY) {
				bt_ready = 1; 
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				nokia_lcd_set_cursor(3, 9);
				nokia_lcd_write_string("Ready to pair!", 1);
				nokia_lcd_set_cursor(0, 25);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				nokia_lcd_render();
				SM1_State = SM1_Wait1;
			}
			else if (BT_CONNECTED) {
				bt_paired = 1; 
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				nokia_lcd_set_cursor(20, 9);
				nokia_lcd_write_string("Paired", 1);
				nokia_lcd_set_cursor(0, 25);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				nokia_lcd_render();
				SM1_State = SM1_Wait1; 
			}
			else if (BT_DISCONNECTED) {
				bt_paired = 0;
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				nokia_lcd_set_cursor(3, 9);
				nokia_lcd_write_string("Ready to pair!", 1);
				nokia_lcd_set_cursor(0, 25);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				nokia_lcd_render();
				SM1_State = SM1_Wait1;
			}
			else if (START) {
				mode = 1; 
				SM1_State = SM1_Wait1; 
			}
			else {
				//nokia_lcd_clear();
				//nokia_lcd_set_cursor(0, 0);
				//nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				//nokia_lcd_set_cursor(0, 15);
				//nokia_lcd_write_string(integer_to_string(cur_mic_value), 1);
				//nokia_lcd_set_cursor(0, 25);
				//nokia_lcd_write_string("*", 1);
				//nokia_lcd_set_cursor(5, 25);
				//nokia_lcd_write_string("Speaker Mode", 1);
				//nokia_lcd_set_cursor(5, 35);
				//nokia_lcd_write_string("LED Matrix", 1);
				//nokia_lcd_render();
				SM1_State = SM1_Wait1; 
			}
			break; 
		case SM1_Wait2:
			//if (BT_CONNECTED) {
				//nokia_lcd_clear();
				//nokia_lcd_set_cursor(0, 0);
				//nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				//nokia_lcd_set_cursor(5, 15);
				//nokia_lcd_write_string("Paired", 0.75);
				//nokia_lcd_set_cursor(5, 25);
				//nokia_lcd_write_string("Speaker Mode", 1);
				//nokia_lcd_set_cursor(0, 35);
				//nokia_lcd_write_string("*", 1);
				//nokia_lcd_set_cursor(5, 35);
				//nokia_lcd_write_string("LED Matrix", 1);
				//nokia_lcd_render();
			//}
			if (up == true) {
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				//nokia_lcd_set_cursor(0, 15);
				//nokia_lcd_write_string(integer_to_string(cur_mic_value), 1);
				nokia_lcd_set_cursor(0, 25);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				if (bt_paired == 1) {
					nokia_lcd_set_cursor(20, 9);
					nokia_lcd_write_string("Paired", 1);
				}
				else if (bt_ready == 1) {
					nokia_lcd_set_cursor(3, 9);
					nokia_lcd_write_string("Ready to pair!", 1);
				}
				nokia_lcd_render();
				SM1_State = SM1_Wait1; 
			}
			else if (BT_READY) {
				bt_ready = 1; 
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				nokia_lcd_set_cursor(3, 9);
				nokia_lcd_write_string("Ready to pair!", 1);
				nokia_lcd_set_cursor(0, 35);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				nokia_lcd_render();
				SM1_State = SM1_Wait1;
			}
			else if (BT_CONNECTED) {
				bt_paired = 1;
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				nokia_lcd_set_cursor(20, 9);
				nokia_lcd_write_string("Paired", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(0, 35);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				nokia_lcd_render();
				SM1_State = SM1_Wait2; 
			}
			else if (BT_DISCONNECTED) {
				bt_paired = 0;
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				nokia_lcd_set_cursor(3, 9);
				nokia_lcd_write_string("Ready to pair!", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(0, 35);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				nokia_lcd_render();
				SM1_State = SM1_Wait2;
			}
			else if (START) {
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("LED Settings", 1.5);
				nokia_lcd_set_cursor(5, 15);
				nokia_lcd_write_string("Power: ", 1);
				if (led_matrix_on == 1) {
					nokia_lcd_write_string("On", 1);
				}
				else {
					nokia_lcd_write_string("Off", 1);
				}
				nokia_lcd_set_cursor(0, 15);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Color: ", 1);
				if (led_matrix_color == 1) {
					nokia_lcd_write_string("Blue", 1);
				}
				else if (led_matrix_color == 2) {
					nokia_lcd_write_string("Red", 1);
				}
				nokia_lcd_render();
				SM1_State = SM1_LEDMenu1; 
				break;
				//if (led_matrix_on == 1) {
					//led_matrix_on = 0; 
					//nokia_lcd_clear();
					//nokia_lcd_set_cursor(0, 0);
					//nokia_lcd_write_string("SimpleBTPlayer", 1.5);
					//nokia_lcd_set_cursor(5, 25);
					//nokia_lcd_write_string("Speaker Mode", 1);
					//nokia_lcd_set_cursor(0, 35);
					//nokia_lcd_write_string("*", 1);
					//nokia_lcd_set_cursor(5, 35);
					//nokia_lcd_write_string("LED Matrix       on", 1);
					//if (bt_paired == 1) {
						//nokia_lcd_set_cursor(20, 9);
						//nokia_lcd_write_string("Paired", 1);
					//}
					//else if (bt_ready == 1) {
						//nokia_lcd_set_cursor(3, 9);
						//nokia_lcd_write_string("Ready to pair!", 1);
					//}
					//nokia_lcd_render();
				//}
				//else {
					//led_matrix_on = 1; 
					//nokia_lcd_clear();
					//nokia_lcd_set_cursor(0, 0);
					//nokia_lcd_write_string("SimpleBTPlayer", 1.5);
					//nokia_lcd_set_cursor(5, 25);
					//nokia_lcd_write_string("Speaker Mode", 1);
					//nokia_lcd_set_cursor(0, 35);
					//nokia_lcd_write_string("*", 1);
					//nokia_lcd_set_cursor(5, 35);
					//nokia_lcd_write_string("LED Matrix       off", 1);
					//if (bt_paired == 1) {
						//nokia_lcd_set_cursor(20, 9);
						//nokia_lcd_write_string("Paired", 1);
					//}
					//else if (bt_ready == 1) {
						//nokia_lcd_set_cursor(3, 9);
						//nokia_lcd_write_string("Ready to pair!", 1);
					//}
					//nokia_lcd_render();
				//}
				//SM1_State = SM1_Wait2; 
			
			//else if (down == true) {
				//nokia_lcd_clear();
				//nokia_lcd_set_cursor(0, 0);
				//nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				//nokia_lcd_set_cursor(5, 15);
				//nokia_lcd_write_string("FM Mode", 1);
				//nokia_lcd_set_cursor(5, 25);
				//nokia_lcd_write_string("Speaker Mode", 1);
				//nokia_lcd_set_cursor(5, 35);
				//nokia_lcd_write_string("LED Matrix", 1);
				//nokia_lcd_set_cursor(0, 35);
				//nokia_lcd_write_string("*", 1);
				//nokia_lcd_render();
				//SM1_State = SM1_Wait3; 
			//}
			}
			else {
				SM1_State = SM1_Wait2; 
			}
			break; 
		case SM1_Wait3:
			if (up == true) {
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(0, 25);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				nokia_lcd_render();
				SM1_State = SM1_Wait2;
			}
			else {
				SM1_State = SM1_Wait3; 
			}
			break; 
		case SM1_LEDMenu1: 
			if (down == true) {
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("LED Settings", 1.5);
				nokia_lcd_set_cursor(5, 15);
				nokia_lcd_write_string("Power: ", 1);
				if (led_matrix_on == 1) {
					nokia_lcd_write_string("On", 1);
				}
				else {
					nokia_lcd_write_string("Off", 1);
				}
				nokia_lcd_set_cursor(0, 25);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Color: ", 1);
				if (led_matrix_color == 1) {
					nokia_lcd_write_string("Blue", 1);
				}
				else if (led_matrix_color == 2) {
					nokia_lcd_write_string("Red", 1);
				}
				nokia_lcd_render();
				SM1_State = SM1_LEDMenu2;
				break;
			}
			else if (START) {
				if (led_matrix_on == 1) {
					reset_rows(rows);
					led_matrix_on = 0;
				}
				else {
					led_matrix_on = 1;
				}
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("LED Settings", 1.5);
				nokia_lcd_set_cursor(5, 15);
				nokia_lcd_write_string("Power: ", 1);
				if (led_matrix_on == 1) {
					nokia_lcd_write_string("On", 1);
				}
				else {
					nokia_lcd_write_string("Off", 1);
				}
				nokia_lcd_set_cursor(0, 15);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Color: ", 1);
				if (led_matrix_color == 1) {
					nokia_lcd_write_string("Blue", 1);
				}
				else if (led_matrix_color == 2) {
					nokia_lcd_write_string("Red", 1);
				}
				nokia_lcd_render();
				SM1_State = SM1_LEDMenu1;
				break;
			}
			else if (BACK) {
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				//nokia_lcd_set_cursor(0, 15);
				//nokia_lcd_write_string(integer_to_string(cur_mic_value), 1);
				nokia_lcd_set_cursor(0, 25);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				if (bt_paired == 1) {
					nokia_lcd_set_cursor(20, 9);
					nokia_lcd_write_string("Paired", 1);
				}
				else if (bt_ready == 1) {
					nokia_lcd_set_cursor(3, 9);
					nokia_lcd_write_string("Ready to pair!", 1);
				}
				nokia_lcd_render();
				SM1_State = SM1_Wait1;
				break;
			}
			break; 
		case SM1_LEDMenu2: 
			if (up == true) {
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("LED Settings", 1.5);
				nokia_lcd_set_cursor(5, 15);
				nokia_lcd_write_string("Power: ", 1);
				if (led_matrix_on == 1) {
					nokia_lcd_write_string("On", 1);
				}
				else {
					nokia_lcd_write_string("Off", 1);
				}
				nokia_lcd_set_cursor(0, 15);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Color: ", 1);
				if (led_matrix_color == 1) {
					nokia_lcd_write_string("Blue", 1);
				}
				else if (led_matrix_color == 2) {
					nokia_lcd_write_string("Red", 1);
				}
				nokia_lcd_render();
				SM1_State = SM1_LEDMenu1;
				break; 
			}
			else if (START) {
				if (led_matrix_color == 1) {
					led_matrix_color = 2;
				}
				else if (led_matrix_color == 2) {
					led_matrix_color = 1;
				}
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("LED Settings", 1.5);
				nokia_lcd_set_cursor(5, 15);
				nokia_lcd_write_string("Power: ", 1);
				if (led_matrix_on == 1) {
					nokia_lcd_write_string("On", 1);
				}
				else {
					nokia_lcd_write_string("Off", 1);
				}
				nokia_lcd_set_cursor(0, 25);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Color: ", 1);
				if (led_matrix_color == 1) {
					nokia_lcd_write_string("Blue", 1);
				}
				else if (led_matrix_color == 2) {
					nokia_lcd_write_string("Red", 1);
				}
				nokia_lcd_render();
				SM1_State = SM1_LEDMenu2;
				break;
			}
			else if (BACK) {
				nokia_lcd_clear();
				nokia_lcd_set_cursor(0, 0);
				nokia_lcd_write_string("SimpleBTPlayer", 1.5);
				//nokia_lcd_set_cursor(0, 15);
				//nokia_lcd_write_string(integer_to_string(cur_mic_value), 1);
				nokia_lcd_set_cursor(0, 25);
				nokia_lcd_write_string("*", 1);
				nokia_lcd_set_cursor(5, 25);
				nokia_lcd_write_string("Speaker Mode", 1);
				nokia_lcd_set_cursor(5, 35);
				nokia_lcd_write_string("LED Matrix", 1);
				if (bt_paired == 1) {
					nokia_lcd_set_cursor(20, 9);
					nokia_lcd_write_string("Paired", 1);
				}
				else if (bt_ready == 1) {
					nokia_lcd_set_cursor(3, 9);
					nokia_lcd_write_string("Ready to pair!", 1);
				}
				nokia_lcd_render();
				SM1_State = SM1_Wait1;
				break;
			}
			break; 
		default: 
			SM1_State = -1; 
			break;  
	}
	switch (SM1_State) {
		case SM1_Init: 
			nokia_lcd_clear();
			nokia_lcd_set_cursor(0, 0);
			nokia_lcd_write_string("SimpleBTPlayer", 1.5);
			//nokia_lcd_set_cursor(5, 15);
			//nokia_lcd_write_string("FM Mode", 1);
			//nokia_lcd_set_cursor(0, 15);
			//nokia_lcd_write_string(integer_to_string(cur_mic_value), 1);
			nokia_lcd_set_cursor(0, 25);
			nokia_lcd_write_string("*", 1);
			nokia_lcd_set_cursor(5, 25);
			nokia_lcd_write_string("Speaker Mode", 1);
			nokia_lcd_set_cursor(5, 35);
			nokia_lcd_write_string("LED Matrix", 1);
			nokia_lcd_render();
			break; 
		case SM1_Wait1: 
			resetJoystick(); 
			break; 
		case SM1_Wait2: 
			resetJoystick();
			break; 
		case SM1_Wait3:
			resetJoystick(); 
			break; 
		case SM1_LEDMenu1:
			resetJoystick();
			break;
		case SM1_LEDMenu2:
			resetJoystick();
			break;
		case SM1_Start: 
			break; 
	}
	return SM1_State; 
}
	
	
///////STATE MACHINE 2: Displays pattern on LED matrix/////////
enum SM2_States {ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7} SM2_State;
int SM2_MatrixDisplay(int SM2_State) {
	switch(SM2_State) {
		case -1:
		SM2_State = ROW0;
		break;
		case ROW0:
		SM2_State = ROW1;
		break;
		case ROW1:
		SM2_State = ROW2;
		break;
		case ROW2:
		SM2_State = ROW3;
		break;
		case ROW3:
		SM2_State = ROW4;
		break;
		case ROW4:
		SM2_State = ROW5;
		break;
		case ROW5:
		SM2_State = ROW6;
		break;
		case ROW6:
		SM2_State = ROW7;
		break;
		case ROW7:
		SM2_State = ROW0;
		break;
		default:
		SM2_State = -1;
		break;
	}
	switch(SM2_State) {
		case ROW0:
		transmit_data(column, rows[0]->col);
		transmit_data(blue, rows[0]->blueRow);
		transmit_data(red, 0xFF);
		
		transmit_data(column, rows[0]->col);
		transmit_data(red, rows[0]->redRow);
		transmit_data(blue, 0xFF);
		
		pulseLED();
		break;
		case ROW1:
		transmit_data(column, rows[1]->col);
		transmit_data(blue, rows[1]->blueRow);
		transmit_data(red, 0xFF);
		
		transmit_data(column, rows[1]->col);
		transmit_data(red, rows[1]->redRow);
		transmit_data(blue, 0xFF);
		
		pulseLED();
		break;
		case ROW2:
		transmit_data(column, rows[2]->col);
		transmit_data(blue, rows[2]->blueRow);
		transmit_data(red, 0xFF);
		
		transmit_data(column, rows[2]->col);
		transmit_data(red, rows[2]->redRow);
		transmit_data(blue, 0xFF);
		
		pulseLED();
		break;
		case ROW3:
		transmit_data(column, rows[3]->col);
		transmit_data(blue, rows[3]->blueRow);
		transmit_data(red, 0xFF);
		
		transmit_data(column, rows[3]->col);
		transmit_data(red, rows[3]->redRow);
		transmit_data(blue, 0xFF);
		
		pulseLED();
		break;
		case ROW4:
		transmit_data(column, rows[4]->col);
		transmit_data(blue, rows[4]->blueRow);
		transmit_data(red, 0xFF);
		
		transmit_data(column, rows[4]->col);
		transmit_data(red, rows[4]->redRow);
		transmit_data(blue, 0xFF);
		
		pulseLED();
		break;
		case ROW5:
		transmit_data(column, rows[5]->col);
		transmit_data(blue, rows[5]->blueRow);
		transmit_data(red, 0xFF);
		
		transmit_data(column, rows[5]->col);
		transmit_data(red, rows[5]->redRow);
		transmit_data(blue, 0xFF);
		
		pulseLED();
		break;
		case ROW6:
		transmit_data(column, rows[6]->col);
		transmit_data(blue, rows[6]->blueRow);
		transmit_data(red, 0xFF);
		
		transmit_data(column, rows[6]->col);
		transmit_data(red, rows[6]->redRow);
		transmit_data(blue, 0xFF);
		
		pulseLED();
		break;
		case ROW7:
		transmit_data(column, rows[7]->col);
		transmit_data(blue, rows[7]->blueRow);
		transmit_data(red, 0xFF);
		
		transmit_data(column, rows[7]->col);
		transmit_data(red, rows[7]->redRow);
		transmit_data(blue, 0xFF);
		
		pulseLED();
		break;
		
	}
	return SM2_State;
}

///////STATE MACHINE 3: Raspberry Pi Communication via USART/////////
enum SM3_States {SM3_Init, SM3_Wait, SM3_Test} SM3_State;
int SM3_PiInterface(int SM3_State) {
	switch(SM3_State) {
		case -1:
			SM3_State = SM3_Init;
			break;
		case SM3_Init:
			SM3_State = SM3_Wait; 
			break; 
		case SM3_Wait:
			if (BT_READY) {
				SM3_State = SM3_Test; 
			}
			else {
				SM3_State = SM3_Wait;
			}
			break;
		case SM3_Test:
			SM3_State = SM3_Wait; 
			break; 
		default: 
			SM3_State = -1;
			break; 
	}
	switch(SM3_State) {
		case SM3_Init: 
			break; 
		case SM3_Wait: 
			break; 
		case SM3_Test:
			break; 
	}
	return SM3_State; 
}



int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0x00; PORTB = 0xFF; 
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    ADC_init();
    nokia_lcd_init();
    nokia_lcd_clear();
	
	row_init(rows); 
	
	
	//period for tasks
	unsigned long int SMTick1_calc = 100;
	unsigned long int SMTick2_calc = 1;
	unsigned long int SMTick3_calc = 5; 
	
	//GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);
	unsigned long int GCD = tmpGCD;
	
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	unsigned long int SMTick3_period = SMTick3_calc/GCD;
	
	static task task1, task2, task3;
	task *tasks[] = {&task1, &task2, &task3};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//task1
	task1.state = -1;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &SM1_Menu;//Function pointer for the tick.
	
	//task2
	task2.state = -1;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &SM2_MatrixDisplay;//Function pointer for the tick.
	
	//task3
	task3.state = -1;//Task initial state.
	task3.period = SMTick3_period;//Task Period.
	task3.elapsedTime = SMTick3_period;//Task current elapsed time.
	task3.TickFct = &SM3_PiInterface;//Function pointer for the tick.
	
	TimerSet(GCD);
	TimerOn();
    while(1)
    {
		joys_tick(); 
		if (bt_paired == 1 && led_matrix_on == 1) {
			microphone_tick(rows);
		}
	    for (int i = 0; i < numTasks; i++ ) {
		    if ( tasks[i]->elapsedTime == tasks[i]->period ) {
			    tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			    tasks[i]->elapsedTime = 0;
		    }
		    tasks[i]->elapsedTime += 1;
		}
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
}
