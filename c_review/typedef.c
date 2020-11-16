#include <stdio.h>

typedef unsigned int uint; //자료형 재정의로 typedef 기존자료형이름 새로운자료형이름
typedef int integer;

void main()
{
	uint number1;
	integer number2;

	number1=100;
	number2=-10;

	printf("number1=%d\n,number2=%d\n",number1,number2);
}