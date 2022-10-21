/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h"
#include "../game/game.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */

extern paddle_t paddle1;
uint8_t z;

void ADC_IRQHandler(void) {
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current > AD_last +50 || AD_current < AD_last -50){
		//ERASE PADDLE
		for(z=0; z<paddle1.height; z++){
			LCD_DrawLine(paddle1.x, MAX_Y-paddle1.padding-z,  paddle1.x+paddle1.width, MAX_Y-paddle1.padding-z, Black);
		}
		paddle1.x= (AD_current*(240-paddle1.width)/0xFFF);	  // ad_last : AD_max = x : 7 		LED_Off((AD_last*7/0xFFF));	
		AD_last = AD_current;
		
			//NEW PADDLE
		for(z=0; z<paddle1.height; z++){
			LCD_DrawLine(paddle1.x, MAX_Y-paddle1.padding-z, paddle1.x+paddle1.width, MAX_Y-paddle1.padding-z, Green);
		}
  }
  return;	
}
