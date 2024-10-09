#include "acutest.h"
#include "Clip3D.h"

template<size_t N>
bool operator==(Vec<N> u, Vec<N> v){
    return norm2(u - v) < 1e-5;
}

struct Position{
    vec4 position;

    bool operator==(const Position& p) const{
        return position == p.position;
    }
};

void test_clip_triangle_inside(){
    std::vector<Position> points = {
        {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {0, 0, 1, 1},
        {-0.3, 0, 0, 1}, {0, 0.8, 0, 1}, {0, 0, 0.2, 1} ,
        {0.1, 0.2, 0.2, 1}, {-0.2, -0.27, 0.37, 1}, {-0.7, -0.3, 0.3, 1} ,
        {0.7, 0.24, -0.18, 1}, {-0.21, -0.8, 0.7, 1}, {0.2, 0.38, 0.28, 1} ,
    };

    int N = points.size();

    for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++)
    for(int k = 0; k < N; k++){
        std::vector<Position> P = {points[i], points[j], points[k]};
        auto clipped = clip(P);

        TEST_CHECK(clipped == P);    
    }
}

void test_clip_triangle_outside(){
    std::vector< std::vector<Position> > triangles = {
        {{10, 0, 0, 1}, {0, 10, 0, 1}, {0, 0, 10, 1} },
        {{-10, 0, 0, 1}, {0, -10, 0, 1}, {0, 0, -10, 1} },
        {{2, 0.1, -0.7, 1}, {3, -0.7, -0.4, 1}, {1.2, 0.4, 1, 1} },
        {{0, 1.5, 0.2, 1}, {-1.2, 2, 0.4, 1}, {0.5, 3, 0.7, 1} },
        {{0.5, 0.4, 1.2, 1}, {0.8, -0.7, 2, 1}, {-0.7, 0, 1.7, 1} },
        {{-2, 0.1, -0.7, 1}, {-3, -0.7, -0.4, 1}, {-1.2, 0.4, 1, 1} },
        {{0, -1.5, 0.2, 1}, {-1.2, -2, 0.4, 1}, {0.5, -3, 0.7, 1} },
        {{0.5, 0.4, -1.2, 1}, {0.8, -0.7, -2, 1}, {-0.7, 0, -1.7, 1} },
    };

    for(auto P: triangles){
        auto clipped = clip(P);

        TEST_CHECK(clipped.empty());
    }
}

void test_clip_triangle(){
    std::vector<Position> P = {
        {1.3, 0, 0, 1},
        {0, 1.78, 0, 1},
        {0, 0, 1.27, 1},
    };

    auto clipped = clip(P);

    std::vector<Position> expected = {
        {0.569663, 1, 0, 1},
        {0, 1, 0.556517, 1},
        {0, 0.378425, 1, 1},
        {0.276378, 0, 1, 1},
        {1, 0, 0.293077, 1},
        {1, 0.410769, 0, 1},
    };

    TEST_CHECK(clipped == expected);    
}

struct Vertex{
    vec4 position;
    vec3 color;

    Vertex operator+(Vertex v) const{
        return {
            position + v.position, 
            color    + v.color
        };
    }

    friend Vertex operator*(float t, Vertex v){
        return {
            t*v.position, 
            t*v.color
        };
    }

    bool operator==(Vertex v) const{
        return 
            position == v.position && 
            color    == v.color;
    }
};

void test_clip_triangle_color(){
    std::vector<Vertex> P = {
        {{1.19, 0.2, 0.1, 1}, {0, 0, 0}},
        {{-0.2, 0.14, 1.33, 1}, {.3, .5, 1}},
        {{0.1, 1.23, 0.2, 1}, {.7, .9, .7}},
    };

    auto clipped = clip(P);

    std::vector<Vertex> expected = {
        {{+0.17293, +0.15610, +1.00000, 1}, {+0.21951, +0.36585, +0.73171}},
        {{-0.11239, +0.45832, +1.00000, 1}, {+0.41681, +0.61681, +0.91239}},
        {{+0.03670, +1.00000, +0.43844, 1}, {+0.61560, +0.81560, +0.76330}},
        {{+0.34340, +1.00000, +0.17767, 1}, {+0.54369, +0.69903, +0.54369}},
        {{+1.00000, +0.37954, +0.11743, 1}, {+0.12202, +0.15688, +0.12202}},
        {{+1.00000, +0.19180, +0.26813, 1}, {+0.04101, +0.06835, +0.13669}},
    };

    TEST_CHECK(clipped == expected);
}

struct VertexPosTexNor{
    vec4 position;
    vec2 texCoords;
    vec3 normal;

    VertexPosTexNor operator+(VertexPosTexNor v) const{
        return {
            position  + v.position, 
            texCoords + v.texCoords, 
            normal    + v.normal
        };
    }

    friend VertexPosTexNor operator*(float t, VertexPosTexNor v){
        return {
            t*v.position, 
            t*v.texCoords, 
            t*v.normal
        };
    }

    bool operator==(VertexPosTexNor v) const{
        return position  == v.position  && 
               texCoords == v.texCoords &&
               normal    == v.normal;
    }
};

void test_clip_triangle_tex_normal(){
    std::vector<VertexPosTexNor> P = {
        {{ 1.31, 0.22, 0.17, 1}, {0, 0}, {0.0, 1.0, 0.0}},
        {{-0.33, 0.21, 1.35, 1}, {0, 1}, {0.0, 0.5, 0.8}},
        {{ 0.11, 1.31, 0.22, 1}, {1, 1}, {0.7, 0.0, 0.1}},
    };

    auto clipped = clip(P);

    std::vector<VertexPosTexNor> expected = {
        {{+0.15644, +0.21297, +1.00000, 1}, {+0.00000, +0.70339}, {+0.00000, +0.64831, +0.56271}},
        {{-0.19372, +0.55071, +1.00000, 1}, {+0.30973, +1.00000}, {+0.21681, +0.34513, +0.58319}},
        {{-0.01400, +1.00000, +0.53845, 1}, {+0.71818, +1.00000}, {+0.50273, +0.14091, +0.29727}},
        {{+0.45128, +1.00000, +0.20578, 1}, {+0.71560, +0.71560}, {+0.50092, +0.28440, +0.07156}},
        {{+1.00000, +0.50158, +0.18292, 1}, {+0.25833, +0.25833}, {+0.18083, +0.74167, +0.02583}},
        {{+1.00000, +0.21811, +0.39305, 1}, {+0.00000, +0.18902}, {+0.00000, +0.90549, +0.15122}},
    };

    TEST_CHECK(clipped == expected);
}

TEST_LIST = {
    {"clip triangle - vec3 - inside", test_clip_triangle_inside},
    {"clip triangle - vec3 - outside", test_clip_triangle_outside},
    {"clip triangle - vec3", test_clip_triangle},
    {"clip triangle - position color", test_clip_triangle_color},
    {"clip triangle - pos texcoords normal", test_clip_triangle_tex_normal},
    {NULL, NULL}
};