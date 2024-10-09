#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#include <algorithm>
#include <cmath>
#include "vec.h"
#include "geometry.h"

//////////////////////////////////////////////////////////////////////////////

struct Pixel{
	int x, y;
};

inline Pixel toPixel(vec2 u){
	return { (int)round(u[0]), (int)round(u[1]) };
}

inline vec2 toVec2(Pixel p){
	return {(float)p.x, (float)p.y};
}

//////////////////////////////////////////////////////////////////////////////

template<class Line>
std::vector<Pixel> rasterizeLine(const Line& P){
	//return simple(P[0], P[1]);

	return dda(P[0], P[1]);

	//return bresenham(toPixel(P[0]), toPixel(P[1]));
}

//////////////////////////////////////////////////////////////////////////////

inline std::vector<Pixel> simple(vec2 A, vec2 B){
	std::vector<Pixel> out;
	vec2 d = B - A;
	float m = d[1]/d[0];
	float b = A[1] - m*A[0];

	int x0 = (int)roundf(A[0]);
	int x1 = (int)roundf(B[0]);

	for(int x = x0; x <= x1; x++){
		int y = (int)roundf(m*x + b);
		out.push_back({x, y});
	}
	return out;
}

//////////////////////////////////////////////////////////////////////////////

inline std::vector<Pixel> dda(vec2 A, vec2 B){
	vec2 dif = B - A;
	float delta = std::max(fabs(dif[0]), fabs(dif[1]));

	vec2 d = (1/delta)*dif;
	vec2 p = A;

	std::vector<Pixel> out;
	for(int i = 0; i <= delta; i++){
		out.push_back(toPixel(p));
		p = p + d;
	}
	return out;
}

//////////////////////////////////////////////////////////////////////////////

inline std::vector<Pixel> bresenham_base(int dx, int dy){
	std::vector<Pixel> out;

	int D = 2*dy - dx; 
	int y = 0;
	for(int x = 0; x <= dx; x++){
		out.push_back({x, y});
		if(D > 0){
			y++;
			D -= 2*dx;
		}
		D += 2*dy;
	}
	return out;
}

inline std::vector<Pixel> bresenham(int dx, int dy){
	std::vector<Pixel> out;

	if(dx >= dy){
		out = bresenham_base(dx, dy);
	}else{
		out = bresenham_base(dy, dx);
		for(Pixel& p: out)
			p = {p.y, p.x};
	}
	return out;
}

inline std::vector<Pixel> bresenham(Pixel p0, Pixel p1){
	if(p0.x > p1.x)
		std::swap(p0, p1);

	std::vector<Pixel> out = bresenham(p1.x - p0.x, abs(p1.y - p0.y));

	int s = (p0.y <= p1.y)? 1: -1;

	for(Pixel& p: out)
		p = {p0.x + p.x, p0.y + s*p.y};

	return out;
}

//////////////////////////////////////////////////////////////////////////////

template<class Tri>
std::vector<Pixel> rasterizeTriangle(const Tri& P){
	//return simple_rasterize_triangle(P);
	return scanline(P);
}

template<class Tri>
std::vector<Pixel> simple_rasterize_triangle(const Tri& P){
	vec2 A = P[0];
	vec2 B = P[1];
	vec2 C = P[2];

	int xmin =  ceil(std::min({A[0], B[0], C[0]}));
	int xmax = floor(std::max({A[0], B[0], C[0]}));
	int ymin =  ceil(std::min({A[1], B[1], C[1]}));
	int ymax = floor(std::max({A[1], B[1], C[1]}));

	std::vector<Pixel> out;
	Pixel p;
	for(p.y = ymin; p.y <= ymax; p.y++)
		for(p.x = xmin; p.x <= xmax; p.x++)
			if(is_inside(toVec2(p), P))
				out.push_back(p);
	return out;
}

template<class Tri>
std::vector<Pixel> scanline(const Tri& P){ //Aula06

    vec2 A = P[0];
	vec2 B = P[1];
	vec2 C = P[2];

    int ymin =  ceil(std::min({A[1], B[1], C[1]}));
	int ymax = floor(std::max({A[1], B[1], C[1]}));
    int xmin, xmax;

	std::vector<Pixel> out;
    Pixel p,it1,it2;

    for(int ys = ymin; ys<=ymax; ys++){
        it1.x=0;
        it1.y=ys;
        it2.x=0;
        it2.y=ys;

        if(ys == A[1] && B[1]==A[1] && ys!=C[1]){ //Horizontal AB
            it1.x = A[0];
            it2.x = B[0];
        }
        else if(ys == A[1] && C[1]==A[1] && ys!=B[1]){ //Horizontal AC
            it1.x = A[0];
            it2.x = C[0];
        }
        else if(ys == B[1] && B[1]==C[1] && ys!=A[1]){ //Horizontal BC
            it1.x = C[0];
            it2.x = B[0];
        }
        else if(A[1]==B[1] && A[1]==C[1] && B[1]==C[1]){ //Horizontal com 3 pontos
            xmin =  ceil(std::min({A[0],B[0],C[0]}));
            xmax = floor(std::max({A[0],B[0],C[0]}));
            p.y = ys;

            for(p.x = xmin; p.x <= xmax; p.x++){
                if(is_inside(toVec2(p), P)){
                        out.push_back(p);
                }
            }
            break;
        }
        else if(A[0]==B[0] && A[0]==C[0] && B[0]==C[0]){ //Vertical com 3 pontos
            p.y=ys;
            p.x=A[0];
            
            if(is_inside(toVec2(p),P))
                out.push_back(p);
            continue;
        }
        else{
            float temp;
            bool c1 = ys >= A[1] ? true : false; //Ys acima de A
            bool c2 = ys >= B[1] ? true : false; //Ys acima de B
            bool c3 = ys >= C[1] ? true : false; //Ys acima de C

            if(c1 != c2){    //Ys entre A e B
                if(B[0]>A[0]){
                    temp = (ys - A[1])/(B[1]-A[1]);
                    it1.x = A[0] + temp*(B[0]-A[0]);
                }
                else if(A[0]>B[0]){
                    temp = (ys - B[1])/(A[1]-B[1]);
                    it1.x = B[0] + temp*(A[0]-B[0]);
                }
                else it1.x = A[0];
            }
            if(c1 != c3){    //Ys entre A e C
                if(C[0]>A[0]){
                    temp = (ys - A[1])/(C[1]-A[1]);
                    if(it1.x==0){
                        it1.x = A[0] + temp*(C[0]-A[0]);
                    } else it2.x = A[0] + temp*(C[0]-A[0]);
                }
                else if(A[0]>C[0]){
                    temp = (ys - C[1])/(A[1]-C[1]);
                    if(it1.x==0){
                        it1.x = C[0] + temp*(A[0]-C[0]);
                    } else it2.x = C[0] + temp*(A[0]-C[0]);
                }
                else it1.x==0 ? it1.x=C[0] : it2.x=C[0];
            }
            if(c2 != c3){    //Ys entre B e C
                if(C[0]>B[0]){
                    temp = (ys - B[1])/(C[1]-B[1]);
                    if(it1.x==0){
                        it1.x = B[0] + temp*(C[0]-B[0]);
                    } else it2.x = B[0] + temp*(C[0]-B[0]);
                }
                else if(B[0]>C[0]){
                    temp = (ys - C[1])/(B[1]-C[1]);
                    if(it1.x==0){
                        it1.x = C[0] + temp*(B[0]-C[0]);
                    } else it2.x = C[0] + temp*(B[0]-C[0]);
                }
                else it1.x==0 ? it1.x=C[0] : it2.x=C[0];
            }
        }

        if(it1.x==0){ // último ponto
            if(ys==A[1]){
                it1.x=A[0]; 
                it2.x=A[0];
            }
            else if(ys==B[1]){
                it1.x=B[0]; 
                it2.x=B[0];
            }
            else {
                it1.x=C[0]; 
                it2.x=C[0];
            }
        }

        xmin =  ceil(std::min({it1.x,it2.x}));
        xmax = floor(std::max({it1.x,it2.x}));
        p.y = ys;

        for(p.x = xmin; p.x <= xmax; p.x++){
            if(is_inside(toVec2(p), P)){
                    out.push_back(p);
            }
        }
    }

	return out;
}

#endif
