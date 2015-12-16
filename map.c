#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "Bem.h"
#include "Person.h"

#define MENU_HEIGHT 5
#define MENU_WIDTH  10

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
int DisplayMenu(int x,int y)
{
	int result;
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
				//mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", input, input); 
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
				result = START;
				break;
			}
			// RANK 선택 시
			else if(choice == RANK)
			{
				endwin();
				result = RANK;
				break;
			}
			// EXIT 선택 시
			else if(choice == EXIT)
			{
				endwin();
				result = EXIT;
				break;
			}
		}
	}

	return result;
}

// 게임 시작 창
int StartMenu()
{
	int x=0;
	int y=0;

	//화면 클리어
	initscr();
	clear();

	//시작 화면 출력
	for(x=0;x<STAGE_ROW;x++)
	{
		for(y=0;y<STAGE_COL;y++)
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
	return DisplayMenu(STAGE_ROW/2+2, STAGE_COL/2-4);
}


//stage 그리기
int StageDraw(int StageMap[STAGE_ROW][STAGE_COL])
{
	int x=0;
	int y=0;
	int Bem_start_x = 0;
	int Bem_start_y = 0;
	int result;

	//화면 클리어
	initscr();
	clear();
	//맵 출력
	for(y=0;y<STAGE_ROW;y++)
	{
		for(x=0;x<STAGE_COL;x++)
		{
			// 벽 일 경우
			if(StageMap[y][x] == WALL)
			{
				move(y,x);
				addstr("*");
			}
			// snake 시작지점 일 경우
			else if(StageMap[y][x] == SNAKE_START)
			{
				Bem_start_x = x;
				Bem_start_y = y;
				// move(x,y);
				// addstr("[]");
			}
			// 먹이 일 경우
			else if(StageMap[y][x] == FOOD)
			{
				FoodNum++;
				move(y,x);
				addstr("O");
			}
			//Reverse일 경우
			else if(StageMap[y][x] == REVERSE)
			{
				move(y,x);
				addstr("?");
			}
			//SLOW 아이템일 경우
			else if(StageMap[y][x] == SLOW)
			{
				move(y,x);
				addstr("?");
			}
			// FAST 아이템일 경우
			else if(StageMap[y][x] == FAST)
			{
				move(y,x);
				addstr("?");
			}
			// ADD Bem length
			else if(StageMap[y][x] == ADDBODY)
			{
				move(y,x);
				addstr("?");
			}
			// 빈 공간일 경우
			else if(StageMap[y][x] == EMPTY)
			{
				continue;
			}
		}
	}
	refresh();
	// 뱀 그리기
	result = DrawBem(Bem_start_x,Bem_start_y,StageMap);
	endwin();

	return result;
}

void EndingDraw(int StageMap[STAGE_ROW][STAGE_COL],int stage)
{
	Person tmp = {0,};
	FILE* f_result;
	char name[30] = {0,};
	int x,y,i=0;
	char c;
	int idx;

	//화면 클리어
	initscr();
	clear();
	//맵 출력
	for(y=0;y<STAGE_ROW;y++)
	{
		for(x=0;x<STAGE_COL;x++)
		{
			// 벽 일 경우
			if(StageMap[y][x] == WALL)
			{
				move(y,x);
				addstr("@");
			}
			else if(StageMap[y][x] == EMPTY)
			{
				continue;
			}
		}
	}

	mvprintw(10,24,"Game is End");
	mvprintw(11,24,"End stage: %d",stage);

	// 만약 stage가 ranking에 든다면
	if( !CheckTopTen(stage) )
	{
		mvprintw(12,24,"Input Name");
		mvprintw(13,24,">");
		refresh();
		while( (c = getch()) !='\n')
		{
			name[i] = c;
			i++;
			move(13,24+i);
			addch(c);
		}
		tmp.stage = stage;
		strncpy(tmp.name,name,sizeof(tmp.name));

		// stage 순으로 sorting
		Player[NumPlayer] = tmp;
		qsort(Player,NumPlayer+1,sizeof(Person),StageComp);

		f_result = fopen("rank.dat","r+");
		// rank.dat 파일이 없을 때
		if(f_result==NULL)
			f_result = fopen("rank.dat","w");
		//파일 쓰기
		fflush(f_result);
		fwrite(Player,sizeof(Person),11,f_result);
		mvprintw(15,15,"Rank register success");
		fclose(f_result);
	}
	else
	{
		mvprintw(12,15,"You are not Top10 Try hard");
	}
	refresh();
	getch();
	endwin();
}

void RankDraw()
{
	int i;
	int x=0;
	int y=0;

	//화면 클리어
	initscr();
	clear();

	//시작 화면 출력
	for(x=0;x<STAGE_ROW;x++)
	{
		for(y=0;y<STAGE_COL;y++)
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

	SettingPerson();
	for(i=0;i<NumPlayer;i++)
	{
		mvprintw(3+i,5,"[%d] name: %s, stage: %d",i+1,Player[i].name,Player[i].stage);
	}
	refresh();
	getch();
	endwin();
}