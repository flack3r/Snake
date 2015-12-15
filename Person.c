#include "Person.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void SettingPerson()
{
	int i =0;
	Person tmp = {0,};
	FILE* f = fopen("rank.dat","rb");
	if(f != NULL)
		return;

	fread(&tmp,sizeof(Person),1,f);
	Player[i++] = tmp;
	while(!feof(f))
	{
		fread(&tmp,sizeof(Person),1,f);
		Player[i++] = tmp;
		if(i >= NumPlayer)
			break;
	}

	fclose(f);
}

/* stage를 받아 top10에 속하는지 판별
/ stage: 끝난 stage
/ return : top10에 속한다면 0, 그렇지 않다면 1을 return
*/
int CheckTopTen(int stage)
{
	int i =0;

	SettingPerson();
	for(i=0; i<NumPlayer; i++)
	{
		if(Player[i].stage <= stage)
		{
			return;
		}
	}
	return (i<NumPlayer)? 0 : 1; 
}

int StageComp(const void *a , const void *b)
{
	Person* a1 = (Person*)a;
	Person* a2 = (Person*)b;

	return a2->stage - a1->stage;
}