#include "Quaternion.h"

Quaternion quaternion_rotation(vec3 n, float theta){
   return {cosf(theta/2), sinf(theta/2)*normalize(n)};
}

vec3 rotate(Quaternion q, vec3 v){
   Quaternion qi = q.a - q.v;
   Quaternion vr = q*v*qi;
   return vr.v;
}

int main(){
   Quaternion q = quaternion_rotation(vec3{2, 3, 5}, 0.25);
   vec3 v = {2, 1, 4};
   print("v", v);
   print("v'", rotate(q, v));
}

