#include <signal.h>
#include <curses.h>
#include <stdlib.h>
#include "fireball.h"
#include "icespike.h"
#include "enemy.h"
#include "highscore.h"

void end ();

struct fireball
{
  int x;
  int y;
};

struct icespike
{
  int x;
  int y;
};

int
main ()
{
  setup ();
  int keypress;
  int y = LINES / 2;
  int x = COLS / 2;
  int prevfb = 0;
  int prevSpike = 0;
  int prevEnemy = 0;
  int score = 0;
  int hscore = 0;
  char scorestr[3];
  char hiscorestr[3];

  bool loop = true;
  drawTBig (y, x);
  refresh ();

  struct fireball ball;
  struct icespike spike;
  struct enemy enemy;
  
  ball.x = -1;
  ball.y = -1;
  spike.x = -10;
  spike.y = -10;
  enemy.x = -20;
  enemy.y = -20;
  
  while (loop)
	 {
		drawGround ();
		refresh ();
		pause ();
		keypress = getch ();
		
	 	if (prevEnemy == 0)
		{
			enemy = makeEnemy();
			prevEnemy++;
	 	}
		
		switch (keypress)
		  {
		  case 'w':
			 y = up (y, x);
			 break;
		  case 'a':
			 x = left (y, x);
			 break;
		  case 's':
			 y = down (y, x);
			 break;
		  case 'd':
			 x = right (y, x);
			 break;
		  case '7':
			 y = up (y, x);
			 x = left (y, x);
			 break;
		  case '8':
			 y = up (y, x);
			 break;
		  case '9':
			 y = up (y, x);
			 x = right (y, x);
			 break;
		  case '4':
			 x = left (y, x); 
			 break;
		  case '6':
			 x = right (y, x);
			 break;
		  case '1':
			 y = down (y, x);
			 x = left (y, x);
			 break;
		  case '2':
			 y = down (y, x);
			 break;
		  case '3':
			 y = down (y, x);
			 x = right (y, x);
			 break;
		  case 'Q':
			 gameover ();
			 cleanup ();
			 break;
		  case 'q':
			 if (prevfb == 0)
				{
				  ball = makeFireball (y, x);
				  prevfb++;
				}
			 break;
		  case 'e':
			 if (prevSpike == 0)
				{
				  spike = makeiceSpike (y, x);
				  prevSpike++;
				}
			 break;
			case ERR:
			 break;
		  }							  //end case 
		if (prevfb > 0 && ball.x < COLS)
		  ball = moveFireball (&ball);
		if (prevSpike > 0 && spike.x < COLS - 1)
		  spike = moveiceSpike (&spike);
		if (enemy.x > 1 && prevEnemy % 5 == 0)
		  enemy = moveEnemy(&enemy);
		else if(enemy.x == 1)
		{
			eraseEnemy(enemy);
			enemy.isDead = 1;
		}
		if (spike.x == COLS - 1)
	   {
			 deleteSpike (&spike);
			 prevSpike = 0;
		}
				
		if (prevfb != 0)
		  prevfb++;
		if (prevSpike != 0)
		  prevSpike++;
		if (prevEnemy != 0)
		  prevEnemy++;
				
		if (prevfb > 10 && ball.x == COLS)
		  prevfb = 0;
		if (prevSpike > 10 && spike.x == COLS - 1)
		  prevSpike = 0;
		move (LINES + 1, COLS + 1);
		
		if((((enemy.x >= x) && (enemy.x <= x+2)) || ((enemy.x+2 >= x) && (enemy.x+2 <= x+2))) && 
			(((enemy.y >= y-4) && (enemy.y <= y)) || ((enemy.y+1 >= y-4) && (enemy.y+1 <= y))))
		{
			gameover();	  
			cleanup();
		}
		
		if(spike.x >= enemy.x && spike.x <= enemy.x+3 && spike.y >= enemy.y-2 && spike.y <= enemy.y)
		{
			enemy.isDead = 1;
			score++;
		}
		
		if(ball.x >= enemy.x && ball.x <= enemy.x+3 && ball.y >= enemy.y-2 && ball.y <= enemy.y)
		{
			enemy.isDead = 1;
			score++;
		}
		
		if(enemy.isDead)
		{
			prevEnemy = 0;
			eraseEnemy(enemy);
			enemy.x = -1;
			enemy.y = -1;
		}
		
		hscore = hiscore(score);
		
		sprintf(scorestr, "%d", score);
		sprintf(hiscorestr, "%d", hscore);		
		
		move(0, (COLS/2)-13);
		addstr("Score: ");
		move(0, (COLS/2)-6);
		addstr(scorestr);
		move(0, (COLS/2)-2);
		addstr("High Score: ");
		move(0, (COLS/2)+10);
		addstr(hiscorestr);
		
		refresh ();
	 }									  //end while loop is true
  cleanup ();
}
