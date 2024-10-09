#include "Quaternion.h"

int main(){
	Quaternion Q1 = {2, 1, 3, 5};
	print(Q1);
	printf("norm(Q1) = %g\n", norm(Q1));

	vec3 u = {5, 4, 8};
	Quaternion Q2 = 3 + u;
	print(Q2);
	
	Quaternion Q3 = 4*Q1 + 2*Q2;
	print(Q3);
	
	print(Q1*Q2);
}
