#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include "Bem.h"
#include "map.h"

int reverse = 0;	//0이면 앞뒤반전된적 없음 1이면 앞뒤 반전됨 앞뒤반전 아이템을 먹었을때 값을 바꿔주면 됨
int delay = 200;
int dir_x=1;
int dir_y=0;
int game_result = -1;
int (*CurrentMap)[STAGE_COL];
int count = 0;
Bem* Head;
Bem* Tail;

int DrawBem(int x,int y,int StageMap[STAGE_ROW][STAGE_COL]){	//start game
	char c;
	CurrentMap = StageMap;

	signal(SIGALRM,whileBem);
	set_ticker(delay);
	setup(x,y);
	
	while(1){
		if((c=getchar()) == 'Q') break;
		else if( c == 'w')	{dir_x=0; dir_y=-1;}
		else if( c == 'a')	{dir_x=-1; dir_y=0;}
		else if( c == 's')	{dir_x=0; dir_y=1;}
		else if( c == 'd')	{dir_x=1; dir_y=0;}
		else if(c == 'x' && game_result == LOSE){ //게임 패배
			break;
		}
		else if(c == 'z' && game_result == WIN) // 게임 클리어
		{
			break;
		}
	}

	return game_result;	//이겼을 경우 0인가 1인가에 따라 승리화면과 패배화면을 출력하도록값을 넘겨줌 
}

// 충돌 이벤트 발생 처리 (이기고 지느냐에 따라 game_result값 결정)
void CheckEvent()
{
	int y,x;
	// 머리 부분 좌표
	int Bem_x = 0;
	int Bem_y = 0;

	if(reverse == 0)
	{
		Bem_x = Head->pos_x;
		Bem_y = Head->pos_y;
	}
	else
	{
		Bem_x = Tail->pos_x;
		Bem_y = Tail->pos_y;
	}

	for(y=0;y<STAGE_ROW;y++)
	{
		for(x=0;x<STAGE_COL;x++)
		{
			//벽인 경우 벰이 부딛혔는지 체크
			if(CurrentMap[y][x] == WALL)
			{
				if(CheckTouchWall(x,y) == 1)
				{
					// mvprintw(10,55,"Bem head x %2d, head y %2d",Bem_x,Bem_y);
					// mvprintw(23,50,"Wall !! %d",count++);
					mvprintw(10,55,"You die. Enter 'x' key");
					refresh();
					game_result = LOSE;
					GameOver();
				}
			}
			//먹이 먹은 경우
			else if(CurrentMap[y][x] == FOOD)
			{
				//뱀의 머리 부분이 닿을 경우
				if(Bem_x == x && Bem_y == y)
				{
					FoodNum--;
					CurrentMap[y][x] = EMPTY;
					move(y,x);
					addstr(" ");
					addBem();

					//mvprintw(10,55,"%d",FoodNum);
					//음식 다 먹은 경우 승리
					if(FoodNum == 0)
					{
						mvprintw(10,55,"Clear. Enter 'z' key");
						refresh();
						game_result = WIN;
						GameOver();
					}
				}
			}
			//Reverse Item 먹을 경우
			else if(CurrentMap[y][x] == REVERSE)
			{
				if(Bem_x == x && Bem_y == y)
				{
					CurrentMap[y][x] = EMPTY;
					move(y,x);
					addstr(" ");

					ReverseBem();
				}
			}
		}
	}
}

// 벽에 몸체가 부딛혔는지 체크. 부딛혔을 시 1, 아니면 0을 리턴
int CheckTouchWall(int x,int y)
{
	Bem* temp = Head;
	int tmp_x,tmp_y;

	refresh();
	while(temp != NULL)
	{
		tmp_x = temp->pos_x;
		
		tmp_y = temp->pos_y;
		if( (tmp_x == x) && (tmp_y == y) )
			return 1;
		temp = temp->next;
	}

	return 0;
}

void GameOver()
{
	signal(SIGALRM,SIG_IGN);
	if(reverse == 0)
		del(Tail);
	else
		del(Head);
}

void ReverseBem()
{
	if(reverse == 0){
		reverse = 1;
		dir_x = Tail->pos_x - Tail->prev->pos_x;
		dir_y = Tail->pos_y - Tail->prev->pos_y;
	}
	else{
		reverse = 0;
		dir_x = Head->pos_x - Head->next->pos_x;
		dir_y = Head->pos_y - Head->next->pos_y;
	}
}

void wrap_up(){
	set_ticker(0);
	endwin();
}

void setup(int x,int y){				//뱀의 기본위치를 세팅하고 딜레이를 주는 부분
	Head = (Bem*)malloc(sizeof(Bem));
	Tail = (Bem*)malloc(sizeof(Bem));
	
	Head->pos_x = x;
	Head->pos_y = y;
	Tail->pos_x = Head->pos_x - dir_x;
	Tail->pos_y = Head->pos_y - dir_y;
	
	Head->next = Tail;
	Head->prev = NULL;
	Tail->prev = Head;
	Tail->next = NULL;	
}

void whileBem(int signum){
	signal(SIGALRM,SIG_IGN);
	moveBem();
	//signal(SIGALRM,whileBem);
}

void moveBem(){		//뱀의 맨 앞부분부터 자신의 값을 뒤로 넘기고 머리는 맨 마지막에 값을 변경함
	int i=1;
	int x,y;
	Bem* Temp;
	signal(SIGALRM,SIG_IGN);
	
	if(reverse==0){
		mvaddch(Tail->pos_y,Tail->pos_x,BLANK);
		Temp = Tail;
		do{
			x=Temp->prev->pos_x;
			y=Temp->prev->pos_y;
			Temp->pos_x = x;
			Temp->pos_y = y;

			mvaddch(Temp->pos_y,Temp->pos_x,BODYSYMBOL);
			Temp = Temp->prev;

			i++;
		}while(Temp->prev);

		Head->pos_x += dir_x;
		Head->pos_y += dir_y;
		mvaddch(Head->pos_y,Head->pos_x,HEADSYMBOL);
	}
	else{						//이곳을 기점으로 아래가 반대로 가는것 위가 똑바로 가는것(뱀의 머리와 꼬리가 바뀌는 아이템을 먹었을때)
		mvaddch(Head->pos_y,Head->pos_x,BLANK);
		Temp = Head;
		do{
			x=Temp->next->pos_x;
			y=Temp->next->pos_y;
			Temp->pos_x = x;
			Temp->pos_y = y;
			
			mvaddch(Temp->pos_y,Temp->pos_x,BODYSYMBOL);
			Temp = Temp->next;

			i++;
		}while(Temp->next);
		
		Tail->pos_x += dir_x;
		Tail->pos_y += dir_y;
		mvaddch(Tail->pos_y,Tail->pos_x,HEADSYMBOL);
	}
	move(23,79);
	refresh();
	//본인 꼬리 부딛힐 경우
	if((Head->pos_x == Tail->pos_x) &&(Head->pos_y == Tail->pos_y) )
	{
		mvprintw(10,55,"You die. Enter 'x' key");
		refresh();
		game_result = LOSE;
		GameOver();
	}
	else
	{
		// 시그널 재 등록
		signal(SIGALRM,whileBem);
		//충돌 이벤트 발생 여부 확인
		CheckEvent();
	}
}

void addBem(){	
	signal(SIGALRM,SIG_IGN);
	
	if(reverse==0)
		insertBody(Tail->prev,Tail);
	else
		insertBody(Head,Head->next);
	signal(SIGALRM,whileBem);
}

void del(Bem* Temp){
	while(Temp != NULL){
		if(reverse == 0){
			mvaddch(Temp->pos_y,Temp->pos_x,BLANK);
			Temp = Temp->next;
		}
		else{
			mvaddch(Temp->pos_y,Temp->pos_x,BLANK);
			Temp = Temp->prev;
		}
		refresh();	
	}
}

void FollowBam()//그냥 오류 나는지 검사용 구문
{
	Bem* tmp = Head;
	int i =0;

	while(tmp)
	{
		mvprintw(i,0,"Bem Body (%d,%d)",tmp->pos_x,tmp->pos_y);
		tmp = tmp->next;
		i++;
	}
	refresh();
}

Bem *insertBody(Bem *left,Bem *right){
		
	Bem *New;
	int DX=0;
	int DY=0;

	New=(Bem*)malloc(sizeof(Bem));
	
	
	New->prev = left;
	New->next = right;
	left->next = New;
	right->prev = New;

	New->pos_x = right->pos_x;
	New->pos_y = right->pos_y;
	return New;
}

int set_ticker(int n_msecs){			//아실꺼라 믿는 타이머
	struct itimerval new_timeset;
	long n_sec, n_usecs;
	
	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000)*1000;
	
	new_timeset.it_interval.tv_sec	= n_sec;
	new_timeset.it_interval.tv_usec	= n_usecs;
	new_timeset.it_value.tv_sec		= n_sec;
	new_timeset.it_value.tv_usec	= n_usecs;
	
	return setitimer(ITIMER_REAL, &new_timeset,NULL);
}
		
void set_cr_noecho_mode(){					//아실꺼라 믿는 에코모드랑 케노니컬 모드 끄는것
	struct termios ttystate;
	
	tcgetattr(0,&ttystate);
	ttystate.c_lflag	&=	~ICANON;
	ttystate.c_lflag	&=	~ECHO;
	ttystate.c_cc[VMIN]	=	1;
	tcsetattr(0,TCSANOW,&ttystate);
}