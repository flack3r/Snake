#include <stdio.h>
#include <ncurses.h>
#include "map.h"
#include "Bem.h"

void GameStart();
int main()
{
	int select;
	int game_result;
	select = StartMenu();
	switch(select)
	{
		case START:
			GameStart();
			break;
		case RANK:
			break;
		case EXIT:
			break;
	}
	
	return 0;
}

void GameStart()
{
	int stage = 1;
	//Stage 1
	if(StageDraw(FirstMap) == WIN)
	{
		stage++;
		//stage 2 start
		if(StageDraw(SecondMap) == WIN)
		{

		}
	}
	// 게임 패배 경우
	else
	{
		EndingDraw(StartMap,stage);
	}
}