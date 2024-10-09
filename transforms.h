#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include "matrix.h"
#include "vec.h"
#include "math.h"

inline mat4 loadIdentity(){
    return Id<4>();
}

inline mat4 translate(vec3 u){
	return {
		1, 0, 0, u[0],
		0, 1, 0, u[1],
		0, 0, 1, u[2],
		0, 0, 0,    1
	};
}

inline mat4 translate(float x, float y, float z){
    return translate(vec3{x, y, z});
}

inline mat4 scale(float x, float y, float z){
	return {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	};
}

inline mat4 rotate_x(float angle){
	float c = cos(angle);
	float s = sin(angle);

	return {
		1, 0,  0, 0,
		0, c, -s, 0,
		0, s,  c, 0,
		0, 0,  0, 1
	};
}

inline mat4 rotate_y(float angle){
	float c = cos(angle);
	float s = sin(angle);

	return {
		 c, 0, s, 0,
		 0, 1, 0, 0,
		-s, 0, c, 0,
		 0, 0, 0, 1
	};
}

inline mat4 rotate_z(float angle){
	float c = cos(angle);
	float s = sin(angle);

	return {
		c, -s, 0, 0,
		s,  c, 0, 0,
		0,  0, 1, 0,
		0,  0, 0, 1
	};
}

/* TAREFA - aula 10 */
inline mat4 rotate(vec3 n, float theta){ // ok
	float cosseno = cos(theta);
	float seno = sin(theta);

    vec3 norm = normalize(n);
    vec3 v1 = {1,0,0};
    vec3 v2 = {0,1,0};
    vec3 v3 = {0,0,1};
    vec3 Rot1 = (1-cosseno)*(dot(v1,norm)*norm)+(cosseno*v1)+(seno*(cross(norm,v1)));
    vec3 Rot2 = (1-cosseno)*(dot(v2,norm)*norm)+(cosseno*v2)+(seno*(cross(norm,v2)));
    vec3 Rot3 = (1-cosseno)*(dot(v3,norm)*norm)+(cosseno*v3)+(seno*(cross(norm,v3)));

	return {
		Rot1[0], Rot2[0], Rot3[0], 0,
		Rot1[1], Rot2[1], Rot3[1], 0,
		Rot1[2], Rot2[2], Rot3[2], 0,
		    0,     0,     0, 	   1
	};
}

inline mat4 orthogonal(float l, float r, float b, float t, float n, float f){
	return {
		2/(r-l),      0,     0,      (l+r)/(l-r),
			0,  2/(t-b),     0,      (b+t)/(b-t),
			0,        0, -2/(f-n),   (n+f)/(n-f),
			0,        0,      0,               1
	};	
}

inline mat4 frustum(float l, float r, float b, float t, float n, float f){
	mat4 Persp = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0,-(n+f)/n, -f,
		0, 0, -1/n, 0
	};
	mat4 Volume = {
		2/(r-l),      0,     0,      (l+r)/(l-r),
			0,  2/(t-b),     0,      (b+t)/(b-t),
			0,        0, 2/(f-n),    (n+f)/(n-f),
			0,        0,      0,               1
	};	
	return Volume*Persp;
}

/* TAREFA  - AULA 12 */
inline mat4 lookAt(vec3 eye, vec3 center, vec3 up){
	vec3 normUp = normalize(up);
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f,normUp));
    vec3 u = cross(s,f);

    mat4 ort = {
        s[0],s[1],s[2],0,
        u[0],u[1],u[2],0,
        -f[0],-f[1],-f[2],0,
        0,    0,    0,    1
    };

    mat4 trans = {
        1,0,0,-eye[0],
        0,1,0,-eye[1],
        0,0,1,-eye[2],
        0,0,0,1
    };

	return ort*trans;
}

/* TAREFA  - AULA 13 angulo em graus, razão aspecto da tela, perto, longe */
inline mat4 perspective(float fovy, float aspect, float Near, float Far){
	
    double pi = 3.14159265;
    float tg = tan((fovy/2)*pi/180);
	float t = Near*tg;
	float b = -t;
	float r = t*aspect;
	float l = -r;

	return frustum(l,r,b,t,Near,Far);
}


#endif
