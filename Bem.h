#include "map.h"
#ifndef BEM
#define BEM
#define BLANK ' '
#define HEADSYMBOL '@'
#define BODYSYMBOL 'O'
#define SET_X 25
#define SET_Y 10
#define WIN 1
#define LOSE 0
	
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
void delite();
void whileBem(int signum);
Bem *insertBody(Bem*,Bem*);
int set_ticker(int);
void startGame();
void del(Bem*);
int DrawBem(int x,int y,int StageMap[STAGE_ROW][STAGE_COL]);
void wrap_up();
void ReverseBem();
void GameOver();
int CheckTouchWall(int x,int y);
#endif