#include <ncurses.h>
#include "map.h"

#define MENU_HEIGHT 5
#define MENU_WIDTH  10

enum Menu
{	
	START=1,
	RANK,
	EXIT,
};

// 선택된 메뉴 하이라이트 
void Highlight_Menu(WINDOW* menu_win, int highlight, char** choices, int choice_num )
{
	int x, y, i;                                                            
	x = 1;                                                                                                          
	y = 1;                                                                                                          
	box(menu_win, 0, 0);                                                                                            
	for(i = 0; i < choice_num; ++i)                                                                                  
	{       
		if(highlight == i + 1) /* High light the present choice */                                              
		{       
			wattron(menu_win, A_REVERSE);                                                                   
			mvwprintw(menu_win, y, x, "%s", choices[i]);                                                    
			wattroff(menu_win, A_REVERSE);                                                                  
		}                                                                                                       
		else                                                                                                    
			mvwprintw(menu_win, y, x, "%s", choices[i]);                                                    
		++y;                                                                                                    
	}                                                                                                               
	wrefresh(menu_win);
}

// 메뉴 선택창 출력
void DisplayMenu(int x,int y)
{
	WINDOW* menu_win;
	int highlight = 1;
	int choice = 0;
	int input;
	char *choices[] = {
			"1. Start",
			"2. Rank",
			"3. Exit",
					};
	int choice_num = sizeof(choices) / sizeof(char*);

	// non echo, 버퍼링 제거
	noecho();
	cbreak();

	// 게임 이름 출력
	attron(A_BOLD);
	move(x-6,y-5);
	addstr("***Snake Game***");
	attroff(A_BOLD);
	refresh();

	// 메뉴 윈도우 생성
	menu_win = newwin(MENU_HEIGHT,MENU_WIDTH, x, y-2);
	keypad(menu_win, TRUE);
	Highlight_Menu(menu_win, highlight, choices, choice_num);
	while(1)
	{
		input = wgetch(menu_win);
		switch(input)                                                                                               
		{       
			case KEY_UP:                                                                                    
				if(highlight == 1)                                                                      
				        highlight = choice_num;                                                          
				else                                                                                    
				        --highlight;                                                                    
				break;                                                                                  
			case KEY_DOWN:                                                                                  
				if(highlight == choice_num)                                                              
				        highlight = 1;                                                                  
				else                                                                                    
				        ++highlight;                                                                    
				break;                                                                                  
			case '\n':                                                                                        
				choice = highlight;                                                                     
				break;                                                                                  
			default:                                                                                        
				mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", input, input); 
				refresh();                                                                              
				break;                                                                                  
		}                                                                                                       
		Highlight_Menu(menu_win, highlight, choices, choice_num);                                                                        
		if(choice != 0) /* User did a choice come out of the infinite loop */  
		{
			// debug 용도
			// mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
			// refresh();
			
			// START 선택 시
			if( choice == START)
			{
				//메인 선택 메뉴 clear
				endwin();
				//stage1 시작
				StageOneDraw(FirstMap);
				break;
			}
			// RANK 선택 시
			else if(choice == RANK)
			{

			}
			// EXIT 선택 시
			else if(choice == EXIT)
			{
				endwin();
				break;
			}
		}
	}
}

// 게임 시작 창
void StartMenu()
{
	int x=0;
	int y=0;

	//화면 클리어
	initscr();
	clear();

	//시작 화면 출력
	for(x=0;x<STAGE1ROW;x++)
	{
		for(y=0;y<STAGE1COL;y++)
		{
			// 벽 일 경우
			if(StartMap[x][y] == WALL)
			{
				move(x,y);
				addstr("*");
			}
			// 빈 공간일 경우
			else if(StartMap[x][y] == EMPTY)
			{
				continue;
			}
		}
	}
	//clrtoeol();
	refresh();

	// 메뉴 출력
	DisplayMenu(STAGE1ROW/2+2, STAGE1COL/2-4);
}


//stage 1 그리기
void StageOneDraw(int StageMap[STAGE1ROW][STAGE1COL])
{
	int x=0;
	int y=0;

	//화면 클리어
	initscr();
	clear();
	//맵 출력
	for(x=0;x<STAGE1ROW;x++)
	{
		for(y=0;y<STAGE1COL;y++)
		{
			// 벽 일 경우
			if(StageMap[x][y] == WALL)
			{
				move(x,y);
				addstr("*");
			}
			// snake 시작지점 일 경우
			else if(StageMap[x][y] == SNAKE_START)
			{
				move(x,y);
				addstr("[]");
			}
			// 빈 공간일 경우
			else if(StageMap[x][y] == EMPTY)
			{
				continue;
			}
		}
	}
	refresh();
	getch();
	endwin();

	//두번째 판 시작
	StageTwoDraw(SecondMap);
}

//두번째 판 그리기
void StageTwoDraw(int StageMap[STAGE1ROW][STAGE1COL])
{
	int x=0;
	int y=0;

	//화면 클리어
	initscr();
	clear();
	//맵 출력
	for(x=0;x<STAGE1ROW;x++)
	{
		for(y=0;y<STAGE1COL;y++)
		{
			// 벽 일 경우
			if(StageMap[x][y] == WALL)
			{
				move(x,y);
				addstr("*");
			}
			// snake 시작지점 일 경우
			else if(StageMap[x][y] == SNAKE_START)
			{
				move(x,y);
				addstr("[]");
			}
			// 빈 공간일 경우
			else if(StageMap[x][y] == EMPTY)
			{
				continue;
			}
		}
	}
	refresh();
	getch();
	endwin();
}