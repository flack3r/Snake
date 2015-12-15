#include <stdio.h>
#include <ncurses.h>
#include "map.h"


int main()
{
	int select;
	select = StartMenu();
	switch(select)
	{
		case START:
			StageOneDraw(FirstMap);
			break;
		case RANK:
			break;
		case EXIT:
			break;
	}
	
	return 0;
}
