#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define S 5
#define WIDTH 40
#define HEIGHT 20
#define B 5
enum direction {UP, DOWN, RIGHT, LEFT };
typedef struct 
{
	int x;
	int y;
}snakedir;
typedef struct
{
	int x;
	int y;
}food;
char *choices[] = { 
			"With barriers",
			"Without barriers",
		  };
typedef struct
{
	int x;
	int y;
}barr;

WINDOW *create_win(int height, int width, int starty, int startx)
{
	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0, 0);
	wrefresh(local_win);
	return local_win;
}

void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;	

	x = 2;
	y = 5;
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	wbkgd(menu_win, COLOR_PAIR(2));
	box(menu_win, 0, 0);
	for(i = 0; i < 2; ++i)
	{	if(highlight == i + 1) /* Highlight the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

food gen_food(snakedir *snake, int SNAKE_LENGTH, barr b[])
{
	int fx, fy, i, ok = 1;
	food f;

	while(ok)
	{	
		ok = 0;
		fx = rand() % (WIDTH - 1) + 1;
		fy = rand() % (HEIGHT - 1) + 1;
		for(i = 0; i < SNAKE_LENGTH; i++)/*Checks if the food was generates on the snake*/
		{
			if(snake[i].x == fx && snake[i].y == fy)
				ok = 1; 
		}
		if (fx == (WIDTH - 1) || fy == (HEIGHT - 1))/*Checks if the food was generated on the box*/
			ok = 1;
		for(i = 0; i < B; i++)/*Checks if the food was generated on the barriers*/
			if((fy == b[i].y) && fabs(fx - b[i].x) < 5)
				ok = 1;
		f.x = fx;
		f.y = fy;
	}
	
	return f;
}

int print_snake(WINDOW *win, snakedir snake[], int SNAKE_LENGTH)
{
	int i;
	
 	wclear(win);
	for (i = 0; i < SNAKE_LENGTH-1 ; i++)
		mvwaddch(win, snake[i].y, snake[i].x, '@' );
	mvwaddch(win, snake[SNAKE_LENGTH - 1].y, snake[SNAKE_LENGTH - 1].x, 'O' );
	box(win, 0, 0);
	
	return 0;
} 

int move_snake( int direction, snakedir snake[], int SNAKE_LENGTH, char * str, int score)
{
	int i,row,col;
	char mesg[]="The game is over! Press any key to exit.";
	char mesgn[]="Well played, ";
	char mesgs[]="You scored ";
	int x = snake[SNAKE_LENGTH - 1].x;
   	int y = snake[SNAKE_LENGTH - 1].y;

	getmaxyx(stdscr,row,col);		
 
	switch (direction) 
	{
	        case UP:
			{
		        if(y - 1 == 0)   
				{
					mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
					mvprintw(row/2 +1,(col-strlen(mesgn))/2,"%s %s !",mesgn, str);
					mvprintw(row/2 +2,(col-strlen(mesgs))/2,"%s %d.",mesgs, score);
					return -1;
			   	} 
			
			    else 
					y--;
			}
	        break;

	        case DOWN:
			{
	            if(y + 1 == HEIGHT - 1)
				{
					
					mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
					mvprintw(row/2 +1,(col-strlen(mesgn))/2,"%s %s !",mesgn, str);
					mvprintw(row/2 +2,(col-strlen(mesgs))/2,"%s %d.",mesgs, score);
					return -1;
			   	} 
				else
					y++;
			}
	            break;

	        case RIGHT:
			{
	            if(x + 1 == WIDTH - 1) 
				{
					
					mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
					mvprintw(row/2 +1,(col-strlen(mesgn))/2,"%s %s !",mesgn, str);
					mvprintw(row/2 +2,(col-strlen(mesgs))/2,"%s %d.",mesgs, score);
					return -1;
			   	} 
				else
					x++;
			}
	            break;

	        case LEFT:
			{
		        if(x - 1 == 0 )
				{
					
					mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
					mvprintw(row/2 +1,(col-strlen(mesgn))/2,"%s %s !",mesgn, str);
					mvprintw(row/2 +2,(col-strlen(mesgs))/2,"%s %d.",mesgs, score);
					return -1;
			   	} 
				else
					x--;
			}
	            break;

	        default:
	            break;
	}
		
 	
	for (i = 0; i < SNAKE_LENGTH - 1; i++)
		snake[i] = snake[i + 1];
	snake[SNAKE_LENGTH - 1].x = x;
    snake[SNAKE_LENGTH - 1].y = y;
	
	return 0;
} 

void print_barr(WINDOW *win, barr b[])
{
	int i;

	for(i = 0; i < B; i++) 
		mvwprintw(win,b[i].y,b[i].x,"____");	

}
void gen_barr(barr b[],snakedir *snake, int SNAKE_LENGTH)
{
	int ok = 1, i, j;
	for(i = 0; i < B; i++) /*Generates barriers*/
	{
		while(ok)
		{
			ok = 0;
			b[i].x = rand() % (WIDTH-5) + 1;
			b[i].y = rand() % (HEIGHT-2) + 1;
			for( j= 0; j < SNAKE_LENGTH; j++)/*Barriers don't appear on the snake*/
				if(snake[j].y== b[i].y && fabs(snake[j].x-b[i].x) < 5)
					ok = 1;
			for(j = 0; j <  i; j++)/*Barriers don't appear on other barriers*/
				if((b[i].y == b[j].y) && fabs(b[i].x - b[j].x) < 5)
					ok = 1;
		}
		ok = 1;
	}
}

int main()
{
	WINDOW *my_win,*menu_win, *name_win;
	int SNAKE_LENGTH = S;
	int startx, starty, row, col, startxx, startyy;
	int cur_dir, ch, i, j, n = 0, ok2 = 1, r;
	int speed = 300, scor = 0, level = 1;
	int highlight = 1, choice = 0;
	int c;
	char mesg[]="The game is over! Press any key to exit.";
	char mesgn[]="Well played,";
	char mesgs[]="You scored ";
	char str[20];
	snakedir snake[WIDTH * HEIGHT - 21], old;
	food fo;
	barr b[B], obs[4 * B];
	
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);
	getmaxyx(stdscr,row,col);
	srand(time(NULL));

	starty = (LINES - HEIGHT) / 2;/*Gets dates about the terminal*/
	startx = (COLS - WIDTH) / 2;
	startyy = (row -HEIGHT/2)/2 ;
	startxx = (col-WIDTH/2)/2;

	name_win = create_win(5,30,startyy,startxx);/*Creates the name window*/

	box(name_win, 0, 0);
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	wbkgd(name_win,COLOR_PAIR(1));
	mvwprintw(name_win,1,1,"Enter your name: ");
	wrefresh(name_win);
	wgetstr(name_win,str);
	delwin(name_win);
	refresh();
	
	noecho();

	menu_win = create_win(HEIGHT/2,WIDTH/2,startyy,startxx);/*Creates the menu window*/

	keypad(menu_win, TRUE);
	print_menu(menu_win, highlight);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = 2;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == 2)
					highlight = 1;
				else 
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
				refresh();
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)	/* come out of the menu */
			break;
	}	
	clrtoeol();
	refresh();
	delwin(menu_win);
	
	my_win=create_win(HEIGHT, WIDTH, starty, startx);

	int sbegx = (WIDTH - SNAKE_LENGTH) / 2;
    int sbegy = (HEIGHT - 1) / 2;
	for (i = 0; i < SNAKE_LENGTH; i++) 
	{
        snake[i].x = sbegx + i;
        snake[i].y = sbegy;
   	 }
 	print_snake(my_win,snake,SNAKE_LENGTH);

    cur_dir = RIGHT;
	gen_barr(b,snake,SNAKE_LENGTH);
	if (choice == 1)
		print_barr(my_win,b);

	fo = gen_food(snake, SNAKE_LENGTH, b);
	mvwaddch(my_win,fo.y,fo.x,'*');
	wrefresh(my_win);
 
	while ((ch = getch()) != 'q' && ok2 == 1)
	{
		timeout(speed);

		if(ch != ERR)
		{
	        switch(ch) 
			{
	            case 'w':
	            	cur_dir = UP;
	            break;

	            case 's':
	                cur_dir = DOWN;
	            break;

	            case 'd':
	                cur_dir = RIGHT;
	            break;

	            case 'a':
	                cur_dir = LEFT;
	            break;

	            default:
	                break;
	        }
	 
	    }

    	old = snake[0];
		r = move_snake(cur_dir, snake, SNAKE_LENGTH,str,scor);
		print_snake(my_win,snake,SNAKE_LENGTH);
		if( r )
			ok2 = 0;

		mvprintw(1,1,"Your level is : %d", level);
		
		for(j = 0; j < SNAKE_LENGTH-2; j++)/*Ends the game if the snake hits itself*/
		{
			if(snake[SNAKE_LENGTH-1].x==snake[j].x && snake[SNAKE_LENGTH-1].y==snake[j].y)
			{
				mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
				mvprintw(row/2 +1,(col-strlen(mesgn))/2,"%s %s !",mesgn, str);
				mvprintw(row/2 +2,(col-strlen(mesgs))/2,"%s %d.",mesgs, scor);
				ok2 = 2;
			}	
		}
		if(choice == 1)
		{
      		for(i = 0; i < B; i++)/*Generates barriers*/
				mvwprintw(my_win,b[i].y,b[i].x,"____");	
		
			for(j = 0; j < 4 * B; j++)
			{
				obs[j].x = b[j/4].x + j%4;
				obs[j].y = b[j/4].y;
			}
	
			for(j = 0; j < 4 * B; j++)/*Ends the game if the snake hits the barriers*/
				if(snake[SNAKE_LENGTH -1].x == obs[j].x && snake[SNAKE_LENGTH -1].y == obs[j].y)
				{
					mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
					mvprintw(row/2 +1,(col-strlen(mesgn))/2,"%s %s !",mesgn, str);
					mvprintw(row/2 +2,(col-strlen(mesgs))/2,"%s %d.",mesgs, scor);
					ok2 = 0;
				}

		}
		
		mvwaddch(my_win,fo.y,fo.x,'*');
		wrefresh(my_win);
		
		if(snake[SNAKE_LENGTH-1].x == fo.x && snake[SNAKE_LENGTH-1].y == fo.y)
		{
			scor+=10; /*Calculates the score*/
			n++;
			if(n == 5)
			{
				level++;/*Calculates the level*/
				mvprintw(1,1,"Your level is : %d", level);
				if(choice == 1)/*Generates new barriers*/
				{
					gen_barr(b,snake,SNAKE_LENGTH);
				}
				speed-=30;
				n = 0;
			}
			SNAKE_LENGTH++;

			for(i = SNAKE_LENGTH-1; i > 0; i--)  /*Makes the snake grow*/
			{
				snake[i] = snake[i - 1];
			}
			snake[0] = old;
			
			print_snake(my_win,snake,SNAKE_LENGTH);
			fo = gen_food(snake,SNAKE_LENGTH, b);
			mvwaddch(my_win,fo.y,fo.x,'*');
			if(choice == 1)
				print_barr(my_win,b);
			wrefresh(my_win);

		}
        
	}
	if (ch == 'q')
	{
		delwin(my_win);
		endwin();
	}
	else
		getchar();
	delwin(my_win);
	endwin();

	return 0;
}
