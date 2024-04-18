#include "LPC17xx.h"
#include "../GLCD/GLCD.h" 
#include "game.h"
#include "stdlib.h"
#include "../timer/timer.h"
#include "stdio.h"

#define K_UP 360
#define K_LAT 180


extern ball_t ball;
extern paddle_t paddle1;
extern paddle_t paddle2;
extern uint8_t depth_wall;
extern uint8_t p;


char string[4];	

const int v_sin[72] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 1, 1, 1, 1, -1, -1, -1, -2, -2, -2, -3, -3, -4, -4, -4, -4, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -4, -4, -4, -4, -3, -3, -3, -2, -2, -1, -1, -1 };
const int v_cos[72] = {5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 1, 1, 0, 0, 0, -1, -1, -2, -2, -2, -3, -3, -4, -4, -4, -4, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -4, -4, -4, -4, -3, -3, -3, -2, -2, -1, -1, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5 };

uint8_t i;
uint16_t d;

void start_game(uint8_t balldim, uint8_t walldep, uint8_t paddlew, uint8_t paddleh, uint8_t padding){
	
	//ATTRIBUTES
	
	//ball
	ball.dimension=balldim;
	
	//wall
	depth_wall=walldep;
	
	//paddle
	paddle1.padding=padding;
	paddle1.height=paddleh;
	paddle1.width=paddlew;
	
	paddle2.padding=padding;
	paddle2.height=paddleh;
	paddle2.width=paddlew;
	
	reset_game();
}


void reset_game(){
	
	p=0;
	
  paddle1.score=0;
  paddle2.score=0;
	
	//SET or RESET Coordinate
	//paddle left
	paddle1.x=MAX_X/2;
	
	paddle2.x=MAX_X/2;


	
	//SCREEN
	LCD_Clear(Black);
	
	
	//wall
	for(i=0; i<depth_wall; i++){
		//LCD_DrawLine(0, i, MAX_X, i, Red);																							//soffitto
		LCD_DrawLine(i, 0+paddle1.padding+paddle1.height, i, MAX_Y-paddle1.padding-paddle1.height, Red);									//sinistra
		LCD_DrawLine(MAX_X-i, 0+paddle1.padding+paddle1.height, MAX_X-i, MAX_Y-paddle1.padding-paddle1.height, Red);			//destra
	}
		
	//paddle 1
	for(i=0; i<paddle1.height; i++){
		LCD_DrawLine(paddle1.x, MAX_Y-paddle1.padding-i, paddle1.x+paddle1.width, MAX_Y-paddle1.padding-i, Green);
	}
	
	//paddle 2
	for(i=0; i<paddle2.height; i++){
		LCD_DrawLine(paddle2.x, 0+paddle2.padding+i, paddle2.x+paddle2.width, 0+paddle2.padding+i, Green);
	}
	
	newBall();
	
	enable_timer(1);
	
}


void newBall(){
	
	ball.speed=10;
	ball.direction= 225;
	ball.move_y=v_sin[ball.direction/5];
	ball.move_x=v_cos[ball.direction/5];
	
	ball.x=MAX_X-depth_wall-ball.dimension-1;
	ball.y= MAX_Y/2;
	
		//ball
	for (i=0; i<ball.dimension; i++) {
		LCD_DrawLine(ball.x, ball.y+i, ball.x+ball.dimension, ball.y+i, Green);
	}
	
}


void sound(int freq){
		reset_timer(2);
		init_timer(2,freq);
		NVIC_SetPriority(TIMER0_IRQn, 1);
		NVIC_SetPriority(TIMER2_IRQn, 0);
		enable_timer(2);
}

void checkWall(){
 if(ball.x==depth_wall || ball.x+ball.dimension==MAX_X-depth_wall){
		ball.direction=(K_LAT-ball.direction+360)%360;
		ball.move_y=v_sin[ball.direction/5];
		ball.move_x=v_cos[ball.direction/5];
		sound(1263);
 }
}

uint8_t checkPaddle(){
	if(ball.y+ball.dimension==MAX_Y-paddle1.padding-paddle1.height){
	 if(ball.x+ball.dimension<paddle1.x || ball.x>paddle1.x+paddle1.width)
			return 1;
	if(ball.x+ball.dimension>=paddle1.x && ball.x<paddle1.x+paddle1.width/2){
			d=K_UP-ball.direction+5;
		  if(d<180)
				ball.direction=d;
			else
				ball.direction=d-10;

	 }else if(ball.x+ball.dimension>=paddle1.x+paddle1.width/2 && ball.x<=paddle1.x+paddle1.width){
			d=K_UP-ball.direction-5;
		  if(d>0)
				ball.direction=d;
			else
				ball.direction=d+10;
	 }
		ball.move_y=v_sin[ball.direction/5];
		ball.move_x=v_cos[ball.direction/5];

		sound(1062);
		
		return 0;
	}else if(ball.y==0+paddle2.padding+paddle2.height){
		if(ball.x+ball.dimension<paddle2.x || ball.x>paddle2.x+paddle2.width)
			return 2;
		if(ball.x+ball.dimension>=paddle1.x && ball.x<paddle1.x+paddle1.width/2){
			d=K_UP-ball.direction+5;
		  if(d<180)
				ball.direction=d;
			else
				ball.direction=d-10;

	 }else if(ball.x+ball.dimension>=paddle1.x+paddle1.width/2 && ball.x<=paddle1.x+paddle1.width){
			d=K_UP-ball.direction-5;
		  if(d>0)
				ball.direction=d;
			else
				ball.direction=d+10;
	 }
		ball.move_y=v_sin[ball.direction/5];
		ball.move_x=v_cos[ball.direction/5];
	
	 	 
	  //MODIFICA VELOCITA' PALLINA
	  if(ball.speed<=30)
			ball.speed+=1;
		
			sound(1062);
	}
	
	return 0;
}

void point(uint8_t k){
	if(k==1){
		paddle2.score+=5;
		if(paddle2.score==25)
			looser(1);
		else
			newBall();
	} else if(k==2){
		paddle1.score+=5;
		if(paddle1.score==25)
			looser(2);
		else
			newBall();
	}
}

void looser(uint8_t k) {
	disable_timer(0);
	disable_timer(1);
	// pulire score 1
	GUI_Text(15,MAX_Y/2-8,(unsigned char*)"  ", White, Black);
	// pulire score 2
	GUI_Text(MAX_X-30,MAX_Y/2-8,(unsigned char*)"  ", White, Black);
	
	if (k==1){
		GUI_Text(MAX_X/2-28,MAX_Y*1/4-8,(unsigned char*)"You Win", White, Black);
		GUI_Text(MAX_X/2-32,MAX_Y*3/4-8,(unsigned char*)"You Lose", White, Black);
	} else {
		GUI_Text(MAX_X/2-32,MAX_Y*1/4-8,(unsigned char*)"You Lose", White, Black);
		GUI_Text(MAX_X/2-28,MAX_Y*3/4-8,(unsigned char*)"You Win", White, Black);
	}

	return;
}

