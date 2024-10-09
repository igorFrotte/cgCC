#ifndef SCENE_H
#define SCENE_H

#include "GLMesh.h"
#include "transforms.h"

class Scene{
    public:
    std::vector<GLMesh> meshes;

    void init(){

        meshes.emplace_back(  
            "modelos/fred/fred.obj", 
            translate(-2, 0, -1)*rotate_y(0.2)*scale(.0002, .0002, .0002));

        meshes.emplace_back(
            "modelos/House Complex/House Complex.obj", 
            translate(4, 0, 0)*rotate_y(0.5*M_PI)*scale(.15, .15, .15)); 

        meshes.emplace_back(  
            "modelos/perna/perna.obj", 
            translate(1, 0, -3)*scale(0.2, 0.2, 0.2));

        MaterialInfo brickwall_material = standard_material("../brickwall.jpg");
        brickwall_material.map_Bump = "../brickwall_normal.jpg";

        meshes.emplace_back(
            "modelos/floor.obj", 
            scale(20, 20, 20), 
            brickwall_material
        );
    }

    void draw() const{
        for(const GLMesh& mesh: meshes)
            mesh.draw();
    }

    private:
    SurfaceMesh flag_mesh(int m, int n) const{
        int N = m*n;

        float u0 = -5, u1 = 5, du = (u1-u0)/(m-1),
            v0 = -5, v1 = 5, dv = (v1-v0)/(n-1);

        SurfaceMesh res;
        res.vertices.resize(N);
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                float u = u0 + i*du;
                float v = v0 + j*dv;

                Vertex& V = res.vertices[i + j*m];
                V.position = {u, v, sinf(u*v/4)};

                float s = i/(m-1.0);
                float t = j/(n-1.0);

                V.texCoords = {s, t};

                vec3 du = {1, 0, cosf(u*v/4)*v/4};
                vec3 dv = {0, 1, cosf(u*v/4)*u/4};
                V.normal = normalize(cross(du, dv));
            }
        }

        for(int i = 0; i < m-1; i++){
            for(int j = 0; j < n-1; j++){	
                unsigned int ij = i + j*m;
                res.indices.insert(res.indices.end(), {
                        ij, ij+1, ij+m,
                        ij+m+1, ij+m, ij+1
                        });
            }
        }

        return res;
    }
};

#endif