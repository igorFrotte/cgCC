#ifndef VEC_H
#define VEC_H

#include "matrix.h"

template<size_t M>
using Vec = Mat<M, 1>;

using vec2 = Vec<2>;
using vec3 = Vec<3>;
using vec4 = Vec<4>;

template<size_t M>
Vec<M> operator*(const Vec<M>& U, const Vec<M>& V){
    Vec<M> R;
    for(size_t i = 0; i < M; i++)
        R[i] = U[i]*V[i];
    return R;
}

template<size_t M>
Vec<M> operator/(const Vec<M>& U, const Vec<M>& V){
    Vec<M> R;
    for(size_t i = 0; i < M; i++)
        R[i] = U[i]/V[i];
    return R;
}

template<size_t M>
float dot(const Vec<M>& U, const Vec<M>& V){
	return dot<M, Vec<M>, Vec<M>>(U, V);
}

template<size_t M>
Vec<M> normalize(const Vec<M>& u){
	return (1.0/norm(u))*u;
}

inline vec3 reflect(vec3 I, vec3 N){
    return I - 2*dot(N, I)*N;
}

template<size_t M, size_t N>
std::vector<Vec<M>> operator*(const Mat<M, N>& A, const std::vector<Vec<N>>& P){
	int n = P.size();
	std::vector<Vec<M>> Q(n);
	for(int i = 0; i < n; i++)
		Q[i] = A*P[i];
	return Q;
}

inline vec2 toVec2(vec2 u){
    return u;
}

inline vec2 toVec2(vec3 u){
    return {u[0], u[1]};
}

inline vec2 toVec2(vec4 u){
    return {u[0], u[1]};
}

inline vec3 toVec3(vec2 u, float z=0){
    return {u[0], u[1], z};
}

inline vec3 toVec3(vec3 u){
    return u;
}

inline vec3 toVec3(vec4 u){
    return {u[0], u[1], u[2]};
}

inline vec4 toVec4(vec2 u, float z=0, float w=1){
    return {u[0], u[1], z, w};
}

inline vec4 toVec4(vec3 u, float w=1){
    return {u[0], u[1], u[2], w};
}

inline vec4 toVec4(vec4 u){
    return u;
}

inline std::vector<vec2> loadCurve(std::string filename){
	std::ifstream in(filename);
	int n = 0;
	in >> n;
	std::vector<vec2> P(n);
	for(vec2& v: P)
		in >> v[0] >> v[1];
	return P;
}

template<size_t M>
Vec<M> lerp(float t, Vec<M> A, Vec<M> B){
	//return (1-t)*A + t*B;
	return A + t*(B-A);
}

template<size_t M>
Vec<M> bilerp(float s, float t, Vec<M> A, Vec<M> B, Vec<M> C, Vec<M> D){
	Vec<M> E = lerp(s, A, B);
	Vec<M> F = lerp(s, C, D);
	return lerp(t, E, F);
}

inline float find_mix_param(vec2 v, vec2 v0, vec2 v1){
	vec2 d = v1 - v0;
	return dot(d, v-v0)/dot(d,d);
}

inline vec3 cross(vec3 u, vec3 v){
	return {
		 u[1]*v[2] - u[2]*v[1],
		 u[2]*v[0] - u[0]*v[2],
		 u[0]*v[1] - u[1]*v[0]
	};
}

template<size_t M, class...V, size_t N = 1+sizeof...(V)>
Mat<M, N> toMat(const Vec<M>& C, V... tail){
	std::array<Vec<M>, N> columns = {C, tail...};
	Mat<M, N> R;
	for(size_t j = 0; j < N; j++)
		R.column(j) = columns[j];
    return R;
}

template<class T, int N = sizeof(T)/sizeof(float)>
Vec<N>& asVec(T& t){
    return reinterpret_cast<Vec<N>&>(t);
}

inline vec2 get2DPosition(vec2 v){
	return v;
}

/*****************************************************************************/
/* TRANSFORMAÇÕES EM COORDENADAS HOMOGÊNEAS */
inline vec2 get2DPosition(vec4 v){
	return {v[0]/v[3], v[1]/v[3]};
}

inline vec2 get2DPosition(vec3 v){
	return {v[0]/v[2], v[1]/v[2]};
}
inline vec3 operator*(const mat3& A, vec2 P){
	return A*toVec3(P, 1);
}

inline std::vector<vec3> operator*(const mat3& A, const std::vector<vec2>& Ps){
	std::vector<vec3> Q;

	for(vec2 P: Ps)
		Q.push_back( A*P );

	return Q;
}
/*****************************************************************************/


#endif