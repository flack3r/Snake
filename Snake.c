#include <stdio.h>
#include <ncurses.h>
#include "map.h"
#include "Bem.h"

void GameStart();
void RankView();

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
			RankView();
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
			stage++;
			//stage 3 start
			if(StageDraw(ThirdMap) == WIN)
			{
				stage++;
				if(StageDraw(FourthMap) == WIN)
					stage++;
			}
		}
	}
	// 게임 패배 경우
	EndingDraw(StartMap,stage);
}

void RankView()
{
	RankDraw();
}