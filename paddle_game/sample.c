/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT and joystick buttons
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "adc/adc.h"
#include "GLCD/GLCD.h"
#include "game/game.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
 
ball_t ball;
paddle_t paddle1;
paddle_t paddle2;
uint8_t depth_wall;
uint8_t p;
 
int main (void) {
	
  SystemInit();
	ADC_init();	
	BUTTON_init();
	LCD_Initialization();
	
	
	LCD_Clear(Black);
	GUI_Text(MAX_X/2-20, MAX_Y/2-16, (unsigned char*)"PRESS", White, Black);
	GUI_Text(MAX_X/2-20, MAX_Y/2+8, (unsigned char*)"START", White, Black);
	
	init_RIT(0x000C4B40);
	init_timer(0, 0x06108);
	init_timer(1, 0xABC20);
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);	

	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);	
	
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
