#ifndef PERSON
#define PERSON

#define NumPlayer 10
typedef struct
{
	char name[30];
	int stage;
}Person;
Person Player[NumPlayer+1];

// 파일로 부터 값을 읽어 Player 메모리로 세팅
void SettingPerson();
/* stage를 받아 top10에 속하는지 판별
/ stage: 끝난 stage
/ return : top10에 속한다면 0, 그렇지 않다면 1을 return
*/
int CheckTopTen(int stage);

// qsort를 위한 것
int StageComp(const void *a , const void *b);
#endif