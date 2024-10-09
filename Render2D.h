#pragma once

#include <vector>
#include "geometry.h"
#include "Image.h"
#include "VertexUtils.h"
#include "Primitives.h"
#include "rasterization.h"
#include "Clip2D.h"

struct Render2dPipeline{
	ImageRGB& image;

	template<class Vertices, class Prims>
	void run(const Vertices& V, const Prims& P){
		ClipRectangle R = {-0.5f, -0.5f, image.width()-0.5f, image.height()-0.5f};
		for(auto primitive: clip(assemble(P, V), R))
			draw(primitive);
	}

	void paint(Pixel p, RGB c){
		if(p.x >= 0 && p.y >= 0 && p.x < image.width() && p.y < image.height())
			image(p.x,p.y) = c;
	}

	template<class Vertex>
	void draw(Line<Vertex> line){
		vec2 L[] = { get2DPosition(line[0]), get2DPosition(line[1]) };
		RGB  C[] = { line[0].color,    line[1].color    };

		for(Pixel p: rasterizeLine(L)){
			float t = find_mix_param(toVec2(p), L[0], L[1]);
			RGB color = lerp(t, C[0], C[1]);
			paint(p, color);
		};
	}

	template<class Vertex>
	void draw(Triangle<Vertex> tri){ // aula 09

		vec2 positions[] = {tri[0].position, tri[1].position, tri[2].position};
        vec3 colors[] = {toVec(tri[0].color), toVec(tri[1].color), toVec(tri[2].color)};

        for(Pixel p: rasterizeTriangle(positions)){ // cada pixel da rasterização
            vec3 alpha = barycentric_coords(toVec2(p),positions); // calc coord baricêntricas 
            vec3 colorVec = alpha[0]*colors[0] + alpha[1]*colors[1] + alpha[2]*colors[2]; //interp
            RGB color = toColor(colorVec);
            paint(p,color);
        }
	}
};

template<class Vertices, class Prims>
void render2d(const Vertices& V, const Prims& P, ImageRGB& image){
	Render2dPipeline pipeline{image};
	pipeline.run(V, P);
}

template<class Vertices, class Prims>
void render2d(const Vertices& V, const Prims& P, RGB color, ImageRGB& image){
	using Vertex = std::remove_const_t<std::remove_reference_t<decltype(V[0])>>;
	std::vector<PosCol<Vertex>> VC;
	for(Vertex v: V)
		VC.push_back({v, color});
	render2d(VC, P, image);
}