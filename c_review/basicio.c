#include<stdio.h>

void main()
{	

	int a, b;
	printf("a="); 
	scanf("%d",&a);

	// %d = -1, 1, 0
	// %f ,%lf = 123.45
	// %c = c (character) 
	// %s = "Programming" (string)

	printf("b=");
	scanf("%d",&b);

	printf("a=%d,b=%d\n",a,b);
}