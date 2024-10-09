#include "colors.inc"
// Inclui o arquivo "colors.inc" que contém definições de cores.

camera {
    location <0, 4, -10>
    look_at <0, 0, 0>
}
// Configurações da câmera: Define a localização da câmera e o ponto para o qual ela está apontando.

light_source {
    <0, 10, -10>
    color rgb <1, 1, 1>
}
// Configurações da fonte de luz: Define a posição da fonte de luz e a cor da luz.

#declare Material = texture {
    pigment { color rgb <0.5, 0.5, 0.5> }
    finish { ambient 0.2 diffuse 0.8 }
}
// Define um material básico com uma textura. A cor do material é um cinza médio e possui um acabamento com 20% de ambiente e 80% de difusão.

sphere {
    <0, -2.5, 0>, 1
    texture { Material }
}
// Cria uma esfera que representa a cabeça do objeto. A esfera está localizada em <0, -2.5, 0> e possui um raio de 1. A textura aplicada é o material definido anteriormente.

cylinder {
    <0, -2, 0>, <0, 1, 0>, 1
    texture { Material }
}
// Cria um cilindro que representa o corpo do objeto. O cilindro tem uma base em <0, -2, 0>, outra base em <0, 1, 0> e um raio de 1. A textura aplicada é o material definido anteriormente.

union {
    cylinder {
        <1.5,-0.5, 0>, <3, -2, 0>, 0.5
        texture { Material }
    }
    cylinder {
        <-1.5,-0.5, 0>, <-3, -2, 0>, 0.5
        texture { Material }
    }
}
// Cria uma união dos braços do objeto. É uma composição de dois cilindros: um para o braço esquerdo e outro para o braço direito. Ambos os cilindros têm tamanhos e posições específicas e são texturizados com o material definido anteriormente.

union {
    cylinder {
        <0.5, -3, 0>, <1, -5, 0>, 0.5
        texture { Material }
    }
    cylinder {
        <-0.5, -3, 0>, <-1, -5, 0>, 0.5
        texture { Material }
    }
}
// Cria uma união das pernas do objeto. É uma composição de dois cilindros: um para a perna esquerda e outro para a perna direita. Ambos os cilindros têm tamanhos e posições específicas e são texturizados com o material definido anteriormente.

union {
    box {
        <-1, 3.2, -1>, <1, 4.2, 1>
        texture { Material }
    }
    torus {
        0.2, 0.1
        rotate <90, 0, 0>
        translate <0, 4.5, 0>
        texture { Material }
    }
}
// Cria uma união das partes do chapéu do objeto. É uma composição de duas formas: uma caixa que representa a base do chapéu e um torus que representa a parte superior do chapéu. Ambas as formas são texturizadas com o material definido anteriormente.

intersection {
    sphere {
        <0.3, 2.7, -0.8>, 0.2
        texture { pigment{color rgb <1, 0, 0>} }
    }
    box {
        <0.3, 2.7, -0.8>, <0.5, 2.9, -0.6>
        texture { pigment{color rgb <1, 0, 0>} }
    }
}
// Cria o olho esquerdo do objeto. É uma interseção entre uma esfera que representa a íris e uma caixa que representa a pálpebra. Ambas as formas são texturizadas com uma cor vermelha.

intersection {
    sphere {
        <-0.3, 2.7, -0.8>, 0.2
        texture { pigment{color rgb <1, 0, 0>} }
    }
    box {
        <-0.3, 2.7, -0.8>, <-0.5, 2.9, -0.6>
        texture { pigment{color rgb <1, 0, 0>} }
    }
}
// Cria o olho direito do objeto. É uma interseção entre uma esfera que representa a íris e uma caixa que representa a pálpebra. Ambas as formas são texturizadas com uma cor vermelha.

box {
    <-0.6, 2.4, 0.8>, <0.6, 2.6, 1>
    texture { Material }
}
// Cria uma caixa que representa a sobrancelha do objeto. É texturizada com o material definido anteriormente.

difference {
    sphere {
        <0, 2, 0>, 1
        texture { Material }
    }
    sphere {
        <0, 1.8, 0>, 0.6
        texture { Material }
    }
}
// Cria uma diferença entre duas esferas para criar um orifício na cabeça do objeto. A primeira esfera representa a cabeça original, enquanto a segunda esfera representa o orifício.
