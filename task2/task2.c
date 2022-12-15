#include <stdio.h>
#include <math.h>

int main(void)
{
	int m, n;
	scanf("%d %d", &m, &n);
	double Z1 =((m-1)*sqrt(m)-(n-1)*sqrt(n))/(sqrt(pow(m,3)*n)+n*m+pow(m,2)-m);
	double Z2 =(sqrt(m)-sqrt(n))/m;
	printf("%lf %lf\n", Z1,Z2);
	return 0;
}
