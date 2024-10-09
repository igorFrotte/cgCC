#include "colors.inc"
camera{
    location <4, 4, -5>
    look_at <0, 0, 0>
    angle 36
}
light_source{ <500, 500, -1000> White }
background { color White }

difference{
    intersection{
        box{-1, 1 pigment{Red}}
        sphere{0, 1.5 pigment{Yellow}}
    }
    union{
        cylinder{ <0, 0, 2>, <0, 0, -2>, 0.7}
        cylinder{ <2, 0, 0>, <-2, 0, 0>, 0.7}
        cylinder{ <0, 2, 0>, <0, -2, 0>, 0.7}
    }
    scale <0.8, 0.5, 0.7>
    rotate <-30, 0, 0>
    pigment{ Blue }
}
