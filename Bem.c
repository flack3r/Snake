#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
//							L'☐'		L'■'
#define BLANK ' '
#define HEADSYMBOL '@'
#define BODYSYMBOL 'O'
#define SET_X 25
#define SET_Y 10
	
typedef struct Bem{
	struct Bem *prev;
	struct Bem *next;
	int pos_x;
	int pos_y;
	}Bem;

void setup(int,int);
void FollowBam();
void moveBem();
void set_cr_noecho_mode();
void addBem();
void del(Bem*);
void wrap_up();
void whileBem(int signum);
Bem *insertBody(Bem*,Bem*);
int set_ticker(int);

int reverse = 0;						//0이면 앞뒤반전된적 없음 1이면 앞뒤 반전됨 앞뒤반전 아이템을 먹었을때 값을 바꿔주면 됨
int delay = 200;
int dir_x=1;
int dir_y=0;

Bem* Head;
Bem* Tail;

int main(){								//start game
	char c;
//	set_cr_noecho_mode();
	signal(SIGALRM,whileBem);
	set_ticker(delay);

	setup(25,10);
	
	while(1){
		if((c=getchar()) == 'Q') break;
		else if( c == 'w')	{dir_x=0; dir_y=-1;}
		else if( c == 'a')	{dir_x=-1; dir_y=0;}
		else if( c == 's')	{dir_x=0; dir_y=1;}
		else if( c == 'd')	{dir_x=1; dir_y=0;}
		else if( c == 'e')	{addBem();}
		else if( c == 'r'){
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
		else if(c == 'x'){				//게임이 종료되었을 경우 불러오면 됨
			signal(SIGALRM,SIG_IGN);
			if(reverse == 0)
				del(Tail);
			else
				del(Head);
			break;}
	}
	wrap_up();
	return 1;	//이겼을 경우 0인가 1인가에 따라 승리화면과 패배화면을 출력하도록값을 넘겨줌 
//	return 0;
}

void wrap_up(){
	set_ticker(0);
	endwin();
}

void setup(int x,int y){				//뱀의 기본위치를 세팅하고 딜레이를 주는 부분
	initscr();
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

	refresh();	
}

void whileBem(int signum){
	signal(SIGALRM,SIG_IGN);
//	FollowBam();
	moveBem();
	signal(SIGALRM,whileBem);
}

void moveBem(){		//뱀의 맨 앞부분부터 자신의 값을 뒤로 넘기고 머리는 맨 마지막에 값을 변경함
	int i=1;
	int x,y;
	Bem* Temp;
	signal(SIGALRM,SIG_IGN);
	
	if(reverse==0){
		mvaddch(Tail->pos_y,Tail->pos_x,BLANK);

//		mvprintw(0,0,"head (%d,%d)",Head->pos_x,Head->pos_y);
		Temp = Tail;
		do{
			x=Temp->prev->pos_x;
			y=Temp->prev->pos_y;
			Temp->pos_x = x;
			Temp->pos_y = y;

			mvaddch(Temp->pos_y,Temp->pos_x,BODYSYMBOL);
			Temp = Temp->prev;

//			mvprintw(i,0,"body (%d,%d)",Temp->pos_x,Temp->pos_y);
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
	while(Temp == NULL){
		if(reverse == 0){
			mvaddch(Temp->pos_y,Temp->pos_x,BLANK);
			Temp = Temp->next;
		}
		else{
			mvaddch(Temp->pos_y,Temp->pos_x,BLANK);
			Temp = Temp->prev;
		}
		refresh();	
		sleep(0.2);
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
//	mvprintw(10,0,"nwe Body (%d,%d)",New->pos_x,New->pos_y);		테스트용


//	mvprintw(11,0,"right Body (%d,%d)",right->pos_x,right->pos_y);		테스트용

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