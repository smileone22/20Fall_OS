//사용자정의 상수
#include <stdio.h>
//define 상수명 상수값 
#define CHARGE 2000

void main()
{
	int time, rate; //declaring two integer variables 

	printf("사용시간을 입력:\n");
	scanf("%d",&time);

	rate = time* CHARGE;
	CHARGE=1000; //THIS WON'T CHANGE THE VALUE OF CHARGE
	printf("사용요금은 %d 원 입니다\n",rate);
	//&rate = rate variable 에 저장된 값의 메모리의 위치 

}

/*WHAT IS THE DIFFERENCE BETWEEN CONST AND #define? 
- const 는 실행이 될때 변수가 고정
- #define 는 실행 전에 처리 

키워드란? 프로그램에서 이미 뜻이 정해져서 사용하고 있는 단어
int, unsigned, switch, continue, break, short, int, if, case ...*/


