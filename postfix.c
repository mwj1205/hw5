/*
 * postfix.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

 /* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum {
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);
char postfixPop();

void evalPush(int x);
int evalPop();

void getInfix(); // 수식 입력
precedence getToken(char symbol); // 연산자 우선순위
precedence getPriority(char x);
void charCat(char* c); // 문자 삽입
void toPostfix();  // infix를 postfix로 변환
void debug();      // 디버그
void reset();      // 초기화
void evaluation(); // postfix로 변환된 수식을 계산

int main()
{
	char command;

	printf("[----- [한민우] [2018038047] -----]\n");

	do {
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
			/* 수식 입력 */
		case 'i': case 'I':
			getInfix();
			break;
			/* postfix notation으로 변경 */
		case 'p': case 'P':
			toPostfix();
			break;
			/* 계산 */
		case 'e': case 'E':
			evaluation();
			break;
			/* 디버그 */
		case 'd': case 'D':
			debug();
			break;
			/* 수식, 스택 초기화 */
		case 'r': case 'R':
			reset();
			break;
			/* 프로그램 종료 */
		case 'q': case 'Q':
			break;
			/* 오류 */
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

// for post fix push
void postfixPush(char x)
{
	postfixStack[++postfixStackTop] = x; 
}

// for post fix pop
char postfixPop()
{
	char x;
	if (postfixStackTop == -1)
		return '\0';
	else {
		x = postfixStack[postfixStackTop--];
	}
	return x;
}

// for evaluation push
void evalPush(int x)
{
	evalStack[++evalStackTop] = x;
}

// for evaluation pop
int evalPop()
{
	if (evalStackTop == -1)
		return -1;
	else
		return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);
}
// 연산자 우선순위 설정
precedence getToken(char symbol)
{
	switch (symbol) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	default: return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0') // 문자열이 없으면 c를 postfixExp에 c복사해서 붙여넣음
		strncpy(postfixExp, c, 1);
	else                    // 문자열이 있으면 c를 뒤에 붙임
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char* exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	 /* check pre conditions */
	if(infixExp[0] == '\0'){
	printf("please enter the infix expression\n");
	return;
	}

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while (*exp != '\0')
	{
		x = *exp;
		if (getPriority(x) == operand) charCat(&x);  // 피 연산자는 postfixExp에 바로 저장
		else if (x == '(') postfixPush(x); // 문자가 '(' 면 push
		else if (x == ')') {           // 읽은 문자가 ')' 면 postfixstack에서 '(' 가 나올 때 까지 pop
			while (postfixStack[postfixStackTop] != '(') {
				x = postfixPop();
				charCat(&x);
			}
			postfixPop();               // '('는 그냥 버린다
		}
		/* 읽고있는 연산자가 postfixStack의 top에 들어있는 연산자보다 우선순위가 낮으면
		우선순위가 낮은 연산자 만날 때 까지 postfixStack의 원소들을 pop한 뒤 자신이 push */
		else {	
			while ((getPriority(postfixStack[postfixStackTop])==minus && getPriority(x) == plus)||getPriority(postfixStack[postfixStackTop]) >= getPriority(x)\
			|| (getPriority(postfixStack[postfixStackTop])==divide && getPriority(x) == times)) {
				x = postfixPop();
				charCat(&x);
			}
			postfixPush(*exp);
		}
		exp++;
	}
	/* postfixStack에 남은 연산자들 pop */
	while (postfixStack[postfixStackTop] != '\0') {
		x = postfixPop();
		charCat(&x);
	}

}
/* debug */
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}
/* reset */
void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for (int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}
/* 계산 */
void evaluation()
{
		 /* check pre conditions */
	if(postfixExp[0] == '\0'){
	printf("please make Postfix\n");
	return;
	}
/* postfixExp의 문자 하나씩을 읽기위한 포인터 */
	char* exp = postfixExp;
	int num1,num2;

	while(*exp != '\0'){
		if(getPriority(*exp) == operand) evalPush(*exp - '0'); // 숫자는 push
		else{ // 숫자 외의 연산자
			num2 = evalPop(); // 피연산자 2개 pop
			num1 = evalPop();
			switch(getPriority(*exp)){ // 연산자에 따른 계산 후 push
				case plus: evalPush(num1+num2);
				break;
				case minus: evalPush(num1-num2);
				break;
				case times: evalPush(num1*num2);
				break;
				case divide: evalPush(num1/num2);
				break;
			}
		}
		exp++;
	}
	evalResult = evalStack[0]; // 결과 저장
}

