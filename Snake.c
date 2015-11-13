#include <stdio.h>
#include <curses.h>
#include "map.h"


void StageOneDraw()
{
	int i=0;
	int j=0;

	//화면 클리어
	clear();
	//맵 출력
	for(i=0;i<STAGE1ROW;i++)
	{
		for(j=0;j<STAGE1COL;j++)
		{
			// 맵이 벽일 경우
			if(FirstMap[i][j] == WALL)
			{
				//printf("%d,%d \n",i,j);
				move(i,j);
				addstr("-");
			}
			// 빈 공간일 경우
			else if(FirstMap[i][j] == EMPTY)
			{
				continue;
			}
		}
	}

	refresh();

}

int main()
{
	initscr();
	StageOneDraw();

	getch();
	endwin();

	return 0;
}
