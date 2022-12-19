#include <stdio.h>
#include <math.h>

int main(void)
{
	int a;
	
	scanf("%d", &a);
	
	double Z1 = (sin(2 * a) + sin(5*a) - sin(3 * a)) / (cos(a) + 1 - 2 * pow(sin(a),2));
	double Z2 = 2 * sin(a);
	
	printf("%lf %lf\n", Z1, Z2);
	
	return 0;
}
