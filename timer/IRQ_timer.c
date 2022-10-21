/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../game/game.h"
#include "stdio.h"
#include "../adc/adc.h"

uint16_t SinTable[45] =                                       
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};
							 					 
extern ball_t ball;
extern paddle_t paddle1;
extern paddle_t paddle2;
extern uint8_t depth_wall;
extern uint8_t p;
extern uint16_t rec;

int x_del, y_del;
int tmp_x, tmp_y, tmp_x_pad; 	
int dir=1, paddle_speed=3;	
uint8_t j, k = 0;

char str[4];
	
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

//	MOVIMENTO 							 
void TIMER0_IRQHandler (void)
{
	//Old COORD
	x_del = ball.x;
	y_del = ball.y;

	//New COORD
	tmp_y = ball.y - ball.speed/10*ball.move_y;
	tmp_x = ball.x + ball.speed/10*ball.move_x;
	
	
	if(p==0){

		//Check coordinate		
		//Y 
		if(tmp_y >= 0+paddle2.height+paddle2.padding && tmp_y + ball.dimension <= MAX_Y-paddle1.height-paddle1.padding){
				ball.y = tmp_y;
		}else if(tmp_y < 0+paddle2.height+paddle2.padding){
			ball.y = 0+paddle2.height+paddle2.padding;
		} else{
				ball.y = MAX_Y-paddle1.height-paddle1.padding-ball.dimension;
		}
		
		//X 
		if(tmp_x >= depth_wall && tmp_x + ball.dimension <= MAX_X-depth_wall){
				ball.x = tmp_x;
		}else if(tmp_x <depth_wall){
				ball.x = depth_wall;
		} else{
				ball.x = MAX_X-depth_wall-ball.dimension;
		}
		
		//Check
		p=checkPaddle();
		checkWall();
		
	}else{
		ball.x = tmp_x;
		ball.y = tmp_y;
		if(ball.y>MAX_Y+ball.dimension || ball.y<0-ball.dimension || ball.x>MAX_X+ball.dimension || ball.x<0-ball.dimension){
			point(p);
			p=0;
		}
	}

	//ERASE BALL
	for (j=0; j<ball.dimension; j++) {
		LCD_DrawLine(x_del, y_del+j, x_del+ball.dimension, y_del+j, Black);
	}
		
	//NEW BALL			
	 for (j=0; j<ball.dimension; j++) {
		 LCD_DrawLine(ball.x, ball.y+j, ball.x+ball.dimension, ball.y+j, Green);
	 }

	//PADDLE
	 //paddle1
	ADC_start_conversion();
	 
	 k++;
	 if(k>=10){
		 if(dir==1){
				if(paddle2.x+k*paddle_speed+paddle2.width>MAX_X){
					dir=-1;
				}
				tmp_x_pad=paddle2.x+(k*paddle_speed*dir);
					//paddle 2
				for(j=0; j<paddle2.height; j++){
					LCD_DrawLine(paddle2.x, 0+paddle2.padding+j, paddle2.x+paddle2.width, 0+paddle2.padding+j, Black);
				}
			
				paddle2.x=tmp_x_pad;
			
				//new paddle2
				for(j=0; j<paddle2.height; j++){
					LCD_DrawLine(paddle2.x, 0+paddle2.padding+j, paddle2.x+paddle2.width, 0+paddle2.padding+j, Green);
			}
		 }else if(dir==-1){
				if(paddle2.x-k*paddle_speed<0){
					dir=1;
				}
				tmp_x_pad=paddle2.x+(k*paddle_speed*dir);
					//paddle 2
				for(j=0; j<paddle2.height; j++){
					LCD_DrawLine(paddle2.x, 0+paddle2.padding+j, paddle2.x+paddle2.width, 0+paddle2.padding+j, Black);
				}
			
				paddle2.x=tmp_x_pad;
			
				//new paddle2
				for(j=0; j<paddle2.height; j++){
					LCD_DrawLine(paddle2.x, 0+paddle2.padding+j, paddle2.x+paddle2.width, 0+paddle2.padding+j, Green);
			}
		}
		k=0;	
	 } 
	 
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
	return;
}


//	PRINT paddle1.score AND RECORD
void TIMER1_IRQHandler (void)
{ 
	// DRAW paddle1.score
	sprintf(str, "%2d", paddle1.score);
	GUI_Text(15,MAX_Y/2-8,(unsigned char*)str, White, Black);
	
	
	// DRAW paddle2.score
	sprintf(str, "%2d", paddle2.score);
	GUI_Text(MAX_X-30,MAX_Y/2-8,(unsigned char*)str, White, Black);

	
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}


// SOUND
void TIMER2_IRQHandler (void)
{
	static int tmp=0;
	/* DAC management */	
	LPC_DAC->DACR = (SinTable[tmp%45])<<6;
	tmp++;
	if(tmp>=90) {
		tmp=0;	
		NVIC_SetPriority(TIMER0_IRQn, 0);
		NVIC_SetPriority(TIMER2_IRQn, 1);
		disable_timer(2);
	}
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
