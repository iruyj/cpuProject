#include <stdio.h>	//표준입출력헤더
#include <stdlib.h>	//malloc, free 등
#include <Windows.h>//windows console api 헤더
#include <time.h>	//랜덤 시드용 time 함수 헤더
#include <conio.h>	//gotoxyD에서 사용할 api 헤더
#include "dalgona.h"

#define MAGIC_KEY 224
#define FIELD_WIDTH 65	//필드가로길이
#define FIELD_HEIGHT 25	//필드세로길이
#define LEFT 75			//키보드 좌 화살표의 char값
#define RIGHT 77		//키보드 우
#define UP 72			//키보드 위
#define DOWN 80			//지보드 아래
#define ITEM_MAX 2		//화면에 표시되는 아이템개수
#define ITEM_GOLD 101	//골드 아이템 인디케이터
#define ITEM_EXP 102	//경험치 아이템 인디케이터
#define LEFT_MARGIN 25	//화면왼쪽마진(공백)
#define TOP_MARGIN 3	//화면 상단마진(공백)
#define DELAYTIME 200	//Sleep함수에 들어갈 x/1000 초

int inputkey = 0;
int mode = 1;	// 달고나모양 (1=동그라미/ 2=세모 / 3=네모)

void printstart();
void playgame();
void selectShape();

enum MENU
{
	GAMESTART = 0,
	EXIT
};

//지렁이를 구현할 이중연결리스트 구조체
#pragma pack(push,1)
typedef struct _WORM
{
	int x;	//x좌표
	int y;	//y좌표
	char direction;	//진행방향
	struct _WORM* next;	//다음노드주소
	struct _WORM* before;//이전노드주소
}WORM, * pWORM;

//아이템을 구현할 단일연결리스트 구조체
typedef struct _ITEM
{
	int x;
	int y;
	int itemType;
	int itemNo;
	struct _ITEM* next;
}ITEM, * pITEM;
#pragma pack(pop)

//커서를 일정 좌표로 이동
void gotoxyD(int x, int y)
{
	COORD Pos;
	Pos.X = x + LEFT_MARGIN;//가로세로 비율을 맞추기 위해서 가로의 좌표는 2배
	Pos.Y = y + TOP_MARGIN;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
// 첫화면 시작종료 선택 커서 함수
enum MENU gamemenu() {
	int y = 0;
	while (1) {
		printstart();

		if (y <= 0)
		{
			y = 0;
		}
		else if (y >= 4)
		{
			y = 2;
		}

		gotoxy(53, 18 + y);
		printf(">");

		inputkey = _getch();

		if (inputkey == MAGIC_KEY)
		{
			switch (_getch())
			{
			case UP:
				gotoxy(53, 18 + y);
				printf("  ");
				y = y - 2;
				break;
			case DOWN:
				gotoxy(53, 18 + y);
				printf("  ");
				y = y + 2;
				break;
			}
		}
		else if (inputkey == 13)
		{
			switch (y)
			{
			case 0:
				return GAMESTART;
			case 2:
				return EXIT;
			}
		}
	}
}

// 게임 시작화면 
void printstart() {
	int x = 40;
	int y = 11;
	system("cls");
	gotoxy(34, 5);
	printf("△￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣○");
	gotoxy(34, 7);
	printf("|                   설탕 뽑기 게임                   |");
	gotoxy(34, 9);
	printf("|    각자 자신이 원하는 모양 중 하나를 선택합니다    |");
	gotoxy(34, 11);
	printf("|     먹이를 먹으며 제한시간 안에 살아가면 됩니다    |");
	gotoxy(34, 13);
	printf("|      먹이를 다 먹지못하면 끝나는 게임입니다        |");
	gotoxy(34, 15);
	printf("□＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿☆");

	x = 100;
	y = 2;
	print_auto_y(&x, &y, "|□|");
	print_auto_y(&x, &y, "|□|");
	print_auto_y(&x, &y, "|□|_∧");
	print_auto_y(&x, &y, "|□|ㅁ0` )");
	print_auto_y(&x, &y, "|□|⊂ ）");
	print_auto_y(&x, &y, "|□|L");
	print_auto_y(&x, &y, "|□|");
	print_auto_y(&x, &y, "|□|");
	print_auto_y(&x, &y, "|□|");
	print_auto_y(&x, &y, "|□|");
	print_auto_y(&x, &y, "|□|");
	print_auto_y(&x, &y, "|□|");
	print_auto_y(&x, &y, "|□|");


	x = 45;
	y = 21;
	print_auto_y(&x, &y, "　　ｏ");
	print_auto_y(&x, &y, "　°");
	print_auto_y(&x, &y, "　┳┳   ∩∩");
	print_auto_y(&x, &y, "　┃┃ (0 ㅁ 0)");
	print_auto_y(&x, &y, "┏┻┻┷━Ｏ┏┷┓┏┷┓");
	print_auto_y(&x, &y, "┃ 　　  　 ┠┨○┠┨○┃");
	print_auto_y(&x, &y, "┗◎━━◎┛┗◎┛┗◎┛");
	print_auto_y(&x, &y, "-----------------------------------------------------");

	x = 1;
	y = 20;
	print_auto_y(&x, &y, "            ,===,==, ");
	print_auto_y(&x, &y, "           ||__|＿|| ");
	print_auto_y(&x, &y, "	    l|＿＼ ＼＼ ");
	print_auto_y(&x, &y, "	   |□|＼＼ 　∧--∧ ");
	print_auto_y(&x, &y, "	   |口|　＼＼(   0ㅁ0) ");
	print_auto_y(&x, &y, "	   |口|　　＼⊂ 　 つ ");
	print_auto_y(&x, &y, "	   |口|　 　 ＼＼つつ＼ ");
	print_auto_y(&x, &y, "	   |口|　　　　＼| 　＼| ");
	print_auto_y(&x, &y, "     ------------------------------------------------");

	gotoxy(56, 18);
	printf("게임 시작");

	gotoxy(56, 20);
	printf("게임 종료");

	print_by_name("고에스더 김민주 전유리");
}

// 게임시작 시작종료처리 함수
void dalgona() {
	system("cls");
	//PlaySound(TEXT("push.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	while (1) {
		switch (gamemenu()) {
		case GAMESTART:
			selectShape();
			//break;

		case EXIT:
			//PlaySound(NULL, 0, 0);
			main();
			break;
		}
	}
}

// 게임 모양선택 함수
void printShape() {
	int x = 10;
	int y = 20;
	system("cls");
	gotoxy(34, 5);
	printf("△￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣  ￣○");
	gotoxy(34, 7);
	printf("|     지금 즉시 하나를 선택해 이동하시기바랍니다.    |");
	//gotoxy(45, 9);
	//printf("지금부터 게임을 시작하겠습니다");
	gotoxy(34, 9);
	printf("|    엔터를 누르면 타이머와 함께 게임을 시작합니다.  |");
	gotoxy(34, 11);
	printf("□＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿  ＿☆");


	print_auto_y(&x, &y, "            ○○○○    ");
	print_auto_y(&x, &y, "          ○        ○");
	print_auto_y(&x, &y, "	   ○          ○");
	print_auto_y(&x, &y, "	    ○        ○");
	print_auto_y(&x, &y, "	      ○○○○   ");
	print_auto_y(&x, &y, "	             ");
	print_auto_y(&x, &y, "	    ");
	print_auto_y(&x, &y, "	  ");
	print_auto_y(&x, &y, "     ");

	x = 40;
	y = 20;
	print_auto_y(&x, &y, "             △     ");
	print_auto_y(&x, &y, "           △   △");
	print_auto_y(&x, &y, "	  △      △");
	print_auto_y(&x, &y, "	 △         △");
	print_auto_y(&x, &y, "	△ △ △ △  △   ");
	print_auto_y(&x, &y, "	            ");
	print_auto_y(&x, &y, "	    ");
	print_auto_y(&x, &y, "	  ");
	print_auto_y(&x, &y, "     ");

	x = 70;
	y = 20;
	print_auto_y(&x, &y, "         □□□□□□□");
	print_auto_y(&x, &y, "         □          □");
	print_auto_y(&x, &y, "	       □          □");
	print_auto_y(&x, &y, "	       □          □");
	print_auto_y(&x, &y, "	       □          □   ");
	print_auto_y(&x, &y, "	       □□□□□□□    ");
	print_auto_y(&x, &y, "	  ");
	print_auto_y(&x, &y, "     ");
}
void selectShape() {
	system("cls");
	int kx = 24;
	int ky = 15;
	int gap = 30;

	while(1){
		printShape();
		if (kx > 24 + gap * 2) {
			kx = 24 + gap * 2;
		}
		else if (kx < 20 ) {
			kx = 20;
		}
		gotoxy(kx, ky);
		printf("　∧,,∧");
		gotoxy(kx, ky+1);
		printf("(-＞ㅁ0-)");
		gotoxy(kx, ky+2);
		printf("　(O O)");
		gotoxy(kx, ky + 3);
		printf("　 T-T");

		inputkey = _getch();
		if (inputkey == MAGIC_KEY) // 방향키
		{
			switch (_getch())
			{
			case LEFT:
				gotoxy(kx, ky);
				printf("  ");
				gotoxy(kx, ky + 1);
				printf("    ");
				gotoxy(kx, ky + 2);
				printf("   ");
				kx -= gap;
				break;
			case RIGHT:
				gotoxy(kx, ky);
				printf("  ");
				gotoxy(kx, ky + 1);
				printf("    ");
				gotoxy(kx, ky + 2);
				printf("   ");
				kx += gap;
				break;
			}
		}
		else if (inputkey == 13)	// 엔터
		{
			switch (kx)
			{
			case 24:
				mode = 1; break;
			case 24+30:
				mode = 2; break;
			case 24+30*2:
				mode = 3; break;
			}
			playgame();
			return 0;
		}
	}
}


//게임영역출력
void PrintField()
{
	for (int i = 1; i < FIELD_WIDTH; i++)
	{
		gotoxyD(i, 0);
		printf("─");
		gotoxyD(i, FIELD_HEIGHT);
		printf("─");
	}

	for (int i = 1; i < FIELD_HEIGHT; i++)
	{
		gotoxyD(0, i);
		printf("│");
		gotoxyD(FIELD_WIDTH, i);
		printf("│");
	}
	gotoxyD(0, 0);
	printf("┌");
	gotoxyD(0, FIELD_HEIGHT);
	printf("└");
	gotoxyD(FIELD_WIDTH, 0);
	printf("┐");
	gotoxyD(FIELD_WIDTH, FIELD_HEIGHT);
	printf("┘");
}

//지렁이를 늘리는 함수(이중연결리스트의 테일쪽에 노드 추가)
void AddWorm(pWORM wormTailNode)
{
	pWORM newNode = malloc(sizeof(WORM));
	pWORM temp = wormTailNode->next;

	//새 노드의 주솟값 이어주기;
	newNode->before = wormTailNode;
	newNode->next = wormTailNode->next;
	wormTailNode->next = newNode;
	newNode->next->before = newNode;
	newNode->direction = temp->direction;
	newNode->x = temp->x;
	newNode->y = temp->y;
	switch (temp->direction)
	{
	case LEFT:
		newNode->x = newNode->x + 1;
		break;
	case RIGHT:
		newNode->x = newNode->x - 1;
		break;
	case UP:
		newNode->y = newNode->y + 1;
		break;
	case DOWN:
		newNode->y = newNode->y - 1;
		break;
	default:
		break;
	}


}

//연결리스트를 받아서 지렁이 출력
void PrintWorm(pWORM wormTailNode, pWORM wormHeadNode)
{
	pWORM curr = wormTailNode->next;
	while (curr != wormHeadNode)
	{
		gotoxyD(curr->x, curr->y);
		printf("o");
		curr = curr->next;
	}
}

//게임점수 출력
void PrintScore(int score)
{
	gotoxyD(FIELD_WIDTH + 3, 3);
	printf("점수 : %d점", score);
	gotoxyD(FIELD_WIDTH + 3, 5);
	printf("종료하려면 Q를 누르세요");
	gotoxyD(FIELD_WIDTH + 3, 7);
	printf("조작은 화살표키로");
	gotoxyD(FIELD_WIDTH + 3, 9);
	printf("mode : %d ",mode);
}

//웜이 지나간 자리 지우기
void ClearWorm(int x, int y)
{
	gotoxyD(x, y);
	printf(" ");
}

//웜 움직이기 
void MoveWorm(pWORM wormTailNode, pWORM wormHeadNode)
{
	pWORM curr;

	curr = wormTailNode->next;
	while (curr != NULL && curr != wormHeadNode->before)
	{
		curr->x = curr->next->x;
		curr->y = curr->next->y;
		curr = curr->next;
	}
	curr = wormHeadNode->before;
	if (curr->direction == LEFT)
	{
		curr->x--;
	}
	else if (curr->direction == RIGHT)
	{
		curr->x++;
	}
	else if (curr->direction == UP)
	{
		curr->y--;
	}
	else if (curr->direction == DOWN)
	{
		curr->y++;
	}
}


//웜 리스트에 할당된 메모리를 순차로 해제
void FreeWormList(pWORM wormTailNode)
{
	pWORM curr;
	pWORM temp;
	curr = wormTailNode;
	while (curr != NULL)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
}

//item 발생시키기
void CreateItem(pITEM itemNode, int* itemNo) {

	pITEM newItem = malloc(sizeof(ITEM));

	newItem->next = itemNode->next;
	newItem->itemNo = (*itemNo)++;
	itemNode->next = newItem;
	newItem->x = 3 + rand() % (FIELD_WIDTH - 3);
	newItem->y = 3 + rand() % (FIELD_HEIGHT - 3);
	newItem->itemType = ITEM_EXP;
}

//아이템 화면에 출력
void PrintItem(pITEM itemNode)
{
	pITEM curr = itemNode->next;
	while (curr != NULL && mode == 1)
	{
		gotoxyD(curr->x, curr->y);
		printf("○");
		curr = curr->next;
	}
	while (curr != NULL && mode == 2)
	{
		gotoxyD(curr->x, curr->y);
		printf("△");
		curr = curr->next;
	}
	while (curr != NULL && mode == 3)
	{
		gotoxyD(curr->x, curr->y);
		printf("□");
		curr = curr->next;
	}
}

//리스트에서 itemNo의 위치를 찾아서 카운터를 리턴
int findItemNoInList(pITEM itemNode, int itemNo)
{
	int numberOfNode = 0;	//노드번호를 저장할 변수

	pITEM curr;		//노드사이를 움직일 포인터
	curr = itemNode->next;		//첫번째 데이터를 가지키는 주소를 curr에 할당

	while (curr != NULL)	//반복문을 돌리면서 찾는 데이터를 발견하면 노드번호를 리턴
	{
		numberOfNode++;
		if (itemNo == curr->itemNo)
		{
			return numberOfNode;
		}
		curr = curr->next;
	}

	return numberOfNode;
}



//아이템의 노드번호를 받아 그 번호의 노드를 삭제
void delItemFromList(pITEM itemNode, int targetNodeNumber)
{
	pITEM beforeTarget;		//제거할 노드의 앞노드
	pITEM target;		//제거할 노드

	int counter = 0;

	beforeTarget = itemNode;

	if (targetNodeNumber < 0)   // 0이 입력되면 FirstNode를 삭제하기 때문에 실행하지 않고 리턴
		return 0;

	//노드를 targetNodeNumber-1 만큼 이동시켜서 beforeTarget을 제거할 노드 앞으로 이동시킴
	while (counter < targetNodeNumber - 1)
	{
		beforeTarget = beforeTarget->next;
		counter = counter + 1;
	}

	//노드를 리스트에서 제거
	target = beforeTarget->next;
	beforeTarget->next = target->next;
	free(target);
}

//아이템(골뱅이)와 웜의 헤드가 만났는지 검사, 
//delItemNo는 먹은 아이템을 지우는 함수로 넘겨줄 변수
int CheckItemHit(pWORM wormHeadPointer, pITEM itemNode, int* delItemNo)
{
	pITEM curr;
	int nodeNo = 0;
	curr = itemNode->next;
	while (curr != NULL)
	{
		if (wormHeadPointer->x == curr->x && wormHeadPointer->y == curr->y)
		{
			if (curr->itemType == ITEM_EXP)
			{
				*delItemNo = curr->itemNo;
				return 1; //아이템 먹으면 1리턴
			}
		}
		nodeNo++;
		curr = curr->next;
	}
	return 0;//아이템을 안만나면 0
}

//아이템의 링크드 리스트 메모리 해제
void FreeItemList(pITEM itemNode)
{
	pITEM curr;
	pITEM temp;
	curr = itemNode;
	while (curr != NULL)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
}

void playgame()
{
	pWORM wormHeadNode = malloc(sizeof(WORM));//이중연결리스트 헤드노드
	pWORM wormTailNode = malloc(sizeof(WORM));//이중연결리스트 테일노드
	pWORM addWorm = malloc(sizeof(WORM));//첫번째 웜몸통 
	pITEM itemNode = malloc(sizeof(ITEM));//아이템용 단일 연결리스트

	wormHeadNode->next = NULL;
	wormHeadNode->before = addWorm;
	addWorm->next = wormHeadNode;
	addWorm->before = wormTailNode;
	addWorm->x = FIELD_WIDTH / 2;
	addWorm->y = FIELD_HEIGHT / 2;
	addWorm->direction = RIGHT;
	wormTailNode->next = addWorm;
	wormTailNode->before = NULL;

	itemNode->next = NULL;
	itemNode->itemNo = -1;

	//지렁이 게임시작 지렁이 생성
	for (int i = 9; i > 0; i--)
		AddWorm(wormTailNode);

	//웜의 머리를 가리키는 포인터
	pWORM wormHeadPointer = addWorm;

	int score = 0;			//최초점수
	int itemCounter = 0;	//아이템 생성 한도 카운터
	char key;				//키입력받을 변수
	int delItemNo = 0;		//지울아이템넘버를 받을 변수초기화
	int itemNo = 10000;//아이템의 최초번호

	//아이템 생성 위치 난수 시드
	srand((unsigned int)time(NULL));

	system("cls");	//화면지우고
	PrintField();	//필드 출력


	while (1)
	{
		//테스트용 출력부분
		//gotoxyD(-LEFT_MARGIN, 0);
		//printf("먹은 아이템 : %d\n",delItemNo);
		//PrintItemList(itemNode);

		if (_kbhit() != 0)
		{

			key = _getch();
			if (key == 'q' || key == 'Q')
			{
				printf("%c", key);
				break;
			}
			if (key == LEFT && wormHeadPointer->direction != RIGHT)
			{
				wormHeadPointer->direction = LEFT;
			}
			else if (key == RIGHT && wormHeadPointer->direction != LEFT)
			{
				wormHeadPointer->direction = RIGHT;
			}
			else if (key == UP && wormHeadPointer->direction != DOWN)
			{
				wormHeadPointer->direction = UP;
			}
			else if (key == DOWN && wormHeadPointer->direction != UP)
			{
				wormHeadPointer->direction = DOWN;
			}
		}

		//웜 지나간 자리 지우기
		ClearWorm(wormTailNode->next->x, wormTailNode->next->y);

		//웜 한칸씩 움직이기
		MoveWorm(wormTailNode, wormHeadNode);

		//벽에 부딛히면 게임오버
		if (wormHeadPointer->x == 0 || wormHeadPointer->x == FIELD_WIDTH || wormHeadPointer->y == 0 || wormHeadPointer->y == FIELD_HEIGHT)
		{
			system("cls");
			gotoxyD(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2);
			printf("벽에 부딛혔습니다. GAME OVER");
			FreeWormList(wormTailNode);
			FreeItemList(itemNode);
		}

		//아이템을 생성
		while (itemCounter < ITEM_MAX)
		{
			CreateItem(itemNode, &itemNo);
			itemCounter++;
		}

		//아이템 먹었는지 확인
		if (CheckItemHit(wormHeadPointer, itemNode, &delItemNo))
		{
			AddWorm(wormTailNode);
			delItemFromList(itemNode, findItemNoInList(itemNode, delItemNo));
			score += 100;
			itemCounter--;
		}
		PrintItem(itemNode);
		PrintWorm(wormTailNode, wormHeadNode);
		PrintScore(score);
		Sleep(DELAYTIME);
	}

	FreeWormList(wormTailNode);
	FreeItemList(itemNode);
}