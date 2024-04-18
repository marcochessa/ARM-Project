/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"
#include "../button_EXINT/button.h"
#include "../timer/timer.h"
#include "../game/game.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

static int down=0;
static int pause=0;
static int reset=0;

void RIT_IRQHandler (void)
{						
	down++;
	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
		 reset_RIT();
				if(down==1){
					disable_timer(0);
					reset_game();
					reset=1;
		}
	} 
	else if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
		reset_RIT();
		if(down==1){
			if(reset==0){
				start_game(5, 5, 40, 10, 32); //START (ball dimension, wall thick, paddle wheight, paddle height, padding)
				enable_timer(0);
			}
			
			if(reset==1)			
				enable_timer(0);
			
			reset=2;
		}
	}	
	else if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
		reset_RIT();
       if(down==1){
				 if(pause){
					 enable_timer(0);
					 enable_timer(1);
					 pause=0;
				 } else{
					 disable_timer(0);
					 disable_timer(1);
				   pause=1;
				 }
       }
	}	
	else {	/* button released */
		down=0;			
		disable_RIT();
		reset_RIT();
		NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
		NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
		NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
	}
		
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
