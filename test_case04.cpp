#include "acutest.h"
#include "rasterization.h"
#include "Primitives.h"
#include "Clip2D.h"
#include "VertexUtils.h"
#include "Color.h"
#include <iostream>
/*
TT             : 4
draw(Triangle) : 1 ok 
clip(Line)     : 2 ok
rotate         : 1 ok
quaternion     : 2 
*/

bool close_points(vec2 u, vec2 v){
    return norm2(u - v) < 1e-5;
}

void test_clip_line(){
    ClipRectangle R = {8.67, 4.92, 22.31, 12.59};
    std::vector<Vec2Col> P = {
        {{6, 10},        red},
        {{11.55, 13.2},  green},
        {{10.78, 9.01},  blue},
        {{13.17, 7.52},  yellow},
        {{17.42, 14.06}, purple},
        {{25.55, 15.89}, brown},
        {{28.08, 13.48}, orange},
        {{6.53, 6.9},    cyan},
        {{12.28, 5.56},  black},
        {{15.7, 2.56},   magenta},
        {{17.97, 7.36},  gray},
        {{20.72, 4},     orange},
        {{24.97, 5.22},  white},
        {{25.39, 8.71},  red},
        {{3.05, 8.95},   blue},
        {{21.08, 13.29}, blue},
        {{24.6, 1.92},   brown}
    };

    LineStrip L{P.size()};
    auto clipped = clip(assemble(L, P), R);
    std::vector<Line<Vec2Col>> expected = {
        { Vec2Col{ {8.67,11.5395}, {132,123,0}},  Vec2Col{ {10.492,12.59}, {49,206,0}}   },
        { Vec2Col{ {11.4379,12.59}, {0,218,37}},  Vec2Col{ {10.78,9.01}, {0,0,255}}      },
        { Vec2Col{ {10.78,9.01}, {0,0,255}},      Vec2Col{ {13.17,7.52}, {255,255,0}}    },
        { Vec2Col{ {13.17,7.52}, {255,255,0}},    Vec2Col{ {16.4647,12.59}, {157,57,99}} },
        { Vec2Col{ {22.31,11.7182}, {187,189,68}},Vec2Col{ {8.67,7.55342}, {25,246,230}} },
        { Vec2Col{ {8.67,6.40129}, {0,160,160}},  Vec2Col{ {12.28,5.56}, {0,0,0}}        },
        { Vec2Col{ {12.28,5.56}, {0,0,0}},        Vec2Col{ {13.0096,4.92}, {54,0,54}}    },
        { Vec2Col{ {16.8161,4.92}, {193,63,193}}, Vec2Col{ {17.97,7.36}, {128,128,128}}  },
        { Vec2Col{ {17.97,7.36}, {128,128,128}},  Vec2Col{ {19.967,4.92}, {220,155,35}}  },
        { Vec2Col{ {22.31,8.74309}, {220,0,35}},  Vec2Col{ {8.67,8.88962}, {64,0,191}}   },
        { Vec2Col{ {8.67,10.3028}, {0,0,255}},    Vec2Col{ {18.1719,12.59}, {0,0,255}}   },
        { Vec2Col{ {21.2967,12.59}, {10,8,245}},  Vec2Col{ {22.31,9.31696}, {56,45,199}} }
    };

    for(auto line: clipped){
        std::cout << "(";
        print(transpose(line[0].position));
        print(transpose(line[1].position));
        std::cout << ")\n";
    }

    std::cout << "clipped size " << clipped.size() << '\n';
    std::cout << "expected size " << expected.size() << '\n';

    TEST_ASSERT(clipped.size() == expected.size());    

    for(unsigned int i = 0; i < expected.size(); i++){
        Line<Vec2Col> line = clipped[i];
        Line<Vec2Col> expected_line = expected[i];

        TEST_CHECK(close_points(line[0].position, expected_line[0].position));
        TEST_CHECK(close_points(line[1].position, expected_line[1].position));
        
        TEST_CHECK(line[0].color == expected_line[0].color);
        TEST_CHECK(line[1].color == expected_line[1].color);
    }
}

TEST_LIST = {
    {"clip line", test_clip_line},
    {NULL, NULL}
};