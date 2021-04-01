/*
 * circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4

typedef char element;
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;


QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
	QueueType *cQ = createQueue();
	element data;
	char command;

    printf("[----- [한민우] [2018038047] -----]\n");

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
			/* 원소 삽입 */
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
			/* 원소 삭제 */
		case 'd': case 'D':
			deQueue(cQ, &data);
			break;
			/* 원소 출력 */
		case 'p': case 'P':
			printQ(cQ);
			break;
			/* 디버그 */
		case 'b': case 'B':
			debugQ(cQ);
			break;
			/* 프로그램 종료 */
		case 'q': case 'Q':
   	        freeQueue(cQ);
			break;
			/* 오류 */
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}
/* 원형 큐 생성 및 초기화 */
QueueType *createQueue()
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	cQ->front = 0;
	cQ->rear = 0;
	for(int i=0;i<MAX_QUEUE_SIZE;i++) cQ->queue[i]=0;
	return cQ;
}
/* 할당된 메모리 해제 */
int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;
    free(cQ);
    return 1;
}
/* 원소 입력 */
element getElement()
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}


/* 원형 큐가 공백 상태인지 확인 */
int isEmpty(QueueType *cQ)
{
	if(cQ->front==cQ->rear) return 1;
    else return 0;
}

/* 원형 큐가 포화 상태인지 확인 */
int isFull(QueueType *cQ)
{
	if(((cQ->rear+1) % MAX_QUEUE_SIZE) == cQ->front) return 1;
	else return 0;
}


/* 원형 큐에 원소 삽입 */
void enQueue(QueueType *cQ, element item)
{
	/* 원형 큐가 포화 상태면 삽입하지 않고 종료 */
	if(isFull(cQ)){
		printf("Circular Queue is full!\n");
		return;
	}
	cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear 한칸 앞으로 이동 후
	cQ->queue[cQ->rear] = item;                 // 원소 삽입
	return;
}

/* 원형 큐에 원소 삭제 */
void deQueue(QueueType *cQ, element *item)
{
	/* 원형 큐가 공백상태면 함수 종료 */
	if(isEmpty(cQ)){
		printf("Circular Queue is empty!\n");
		return;
	}
	cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // front 한칸 앞으로 이동
    return;
}

/* 큐 원소 출력 */
void printQ(QueueType *cQ)
{
	int i, first, last;

	first = (cQ->front + 1) % MAX_QUEUE_SIZE;
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE;

	printf("Circular Queue : [");

	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}

/* 큐의 원소와 front, rear 출력 */
void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}

