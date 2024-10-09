#ifndef VERTEXUTILS_H
#define VERTEXUTILS_H

#include "vec.h"
#include "Color.h"

template<size_t N>
vec4 getPosition(Vec<N> v){	return toVec4(v); }

template<typename Vertex>
vec4 getPosition(Vertex in){ return toVec4(in.position); }


template <class Vec>
struct PosCol{
	Vec position;
	RGB color;

	friend PosCol lerp(float t, PosCol u, PosCol v){
		return {lerp(t, u.position, v.position), lerp(t, u.color, v.color)};
	}

	friend vec2 get2DPosition(PosCol in){ 
		return get2DPosition(in.position); 
	}

	friend vec3 getColor(PosCol in){
		return toVec(in.color);
	}
};

using Vec2Col = PosCol<vec2>;
using Vec3Col = PosCol<vec3>;
using Vec4Col = PosCol<vec4>;

#endif
