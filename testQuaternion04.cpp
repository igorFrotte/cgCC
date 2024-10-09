#include "Quaternion.h"

Quaternion quaternion_rotation(vec3 n, float theta){
   return {cosf(theta/2), sinf(theta/2)*normalize(n)};
}

int main(){
   Quaternion q1 = quaternion_rotation(vec3{2, 3, 5}, 0.25);
   Quaternion q2 = quaternion_rotation(vec3{1, 7, 6}, 0.43);
   Quaternion q = q2*q1;
   float theta = 2*acos(q.a);
   vec3 n = normalize(q.v);
   printf("theta = %f\n", theta);
   print("n", n);
}

