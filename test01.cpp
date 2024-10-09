#include "Color.h"
#include "vec.h"

int main(){
	RGB a1 = 0x7d663a_rgb;
	RGB a2 = orange;
	RGB a3 = lerp(0.2, a1, a2);
	vec3 u = {1, 2, 4};

	print("a1", toVec(a1));
	print("a2", toVec(a2));
	print("a3", toVec(a3));
}