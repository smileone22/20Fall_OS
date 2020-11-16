#include <stdio.h>

void main()
{
	const float PI = 3.14;
	float pi=3.14; 
	int radius;// integer variable

	PI=3.1; // this will not change the value of PI
	pi=3.1; // this will change 

	scanf("%d",&radius); //radius 변수에 키보드로 입력받을 숫자를 저장
	printf("%d*3.14=%f\n", radius,radius*PI);

}