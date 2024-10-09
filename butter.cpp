#include <iostream>
#include "Render2D.h"
#include "bezier.h"
#include "matrix.h"
#include "polygon_triangulation.h"
#include "Color.h"

using namespace std;

int main()
{
    vector<vec2> CP = loadCurve("borboleta.txt");
    vector<vec2> P = sample_bezier_spline<3>(CP, 30);
    vector<unsigned int> tri;

    vec2 v = {350,350};

    mat3 T = {
        1.0, 0.0, v[0],
        0.0, 1.0, v[1],
        0.0, 0.0, 1.0
    };

    mat3 Ti = {
        1.0, 0.0, -v[0],
        0.0, 1.0, -v[1],
        0.0, 0.0, 1.0
    };

    LineStrip L{P.size()};
    float t,u;
    RGB r = red;
    RGB y = yellow;

    ImageRGB G(700, 700);

    G.fill(white);

    for(t = 0, u = 0; t <= 2*3.14 ; t+=0.5233333333, u+=0.08333333333){

        mat3 R = {
            cos(t), -sin(t), 0.0,
            sin(t),  cos(t), 0.0,
            0.0,     0.0,    1.0
        };

        
        //tri = triangulate_polygon(B);
        //Elements<Triangles> Te{tri};
        //TriLines<Elements<Triangles>> C{tri};

        //RGB c = lerp(u,r,y);

        //render2d(B, Te, c, G);
        //render2d(B, C, c, G);
        render2d((T*R*Ti)*P, L, black, G);
    }


    G.savePNG("output.png");

}