
/////////////////////////////////////////////////////////////////////////////
//////////////////////////CS122A FINAL PROJECT///////////////////////////////
//////////////////////////SimpleFMTransmitter////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/io.h>

#include "bit.h"
#include "matrix_write.h"
#include "nokia5110.h"
#include "joystick.h"
#include "timer.h"
#include "tasks.h"
#include "rows.h"


///////STATE MACHINE 1: User menu interface/////////
enum SM1_States {SM1_Init, SM1_Wait, SM1_Start} SM1_State;
int SM1_Menu(int SM1_State) {
	switch(SM1_State) {
		case -1: 
			SM1_State = SM1_Init; 
			break; 
		case SM1_Init:
			SM1_State = SM1_Wait; 
			break;   
		case SM1_Wait:
		
			break; 
		default: 
			SM1_State = -1; 
			break;  
	}
	switch (SM1_State) {
		case SM1_Init: 
			nokia_lcd_clear();
			nokia_lcd_set_cursor(10, 0);
			nokia_lcd_write_string("Connect 4", 1.5);
			nokia_lcd_set_cursor(5, 20);
			nokia_lcd_write_string("Start", 1);
			nokia_lcd_set_cursor(0, 20);
			nokia_lcd_write_string("*", 1);
			nokia_lcd_set_cursor(5, 30);
			nokia_lcd_write_string("Options", 1);
			nokia_lcd_render();
			break; 
		case SM1_Wait: 
			resetJoystick(); 
			break; 
		case SM1_Start: 
		
			break; 
	}
	return SM1_State; 
}

///////INITIALIZE MATRIX COLUMN ROWS//////////
row ROW_0, ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7;
row *rows[8] = { &ROW_0, &ROW_1, &ROW_2, &ROW_3, &ROW_4, &ROW_5, &ROW_6, &ROW_7};
	
	
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

int main(void)
{
    ADC_init();
    nokia_lcd_init();
    nokia_lcd_clear();
	
	//period for tasks
	unsigned long int SMTick1_calc = 100;
	unsigned long int SMTick2_calc = 1;
	
	//GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc );
	unsigned long int GCD = tmpGCD;
	
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	
	static task task1, task2;
	task *tasks[] = {&task1, &task2};
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
	
	TimerSet(GCD);
	TimerOn();
	
    while(1)
    {
		joys_tick(); 
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

