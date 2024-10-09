#include "Image.h"
#include "raytracing.h"
#include "Phong.h"

struct Paraboloid{
    float A, B, C;
    
    vec3 normal(vec3 pos) const{
        float x = pos[0];
        float y = pos[1];
        float z = pos[2];
        return {2*x/(A*A), -2*y/(B*B), 1/C};
    }
};

struct ParaboloidIntersection : public Intersection{
    Paraboloid paraboloid;
};

inline ParaboloidIntersection intersection(Ray ray, Paraboloid paraboloid){
    float A = paraboloid.A;
    float B = paraboloid.B;
    float C = paraboloid.C;
    vec3 d = ray.dir;
    float x0 = ray.orig[0];
    float y0 = ray.orig[1];
    float z0 = ray.orig[2];
    
    float a = d[0]*d[0]/(A*A) - d[1]*d[1]/(B*B);
    float b = 2*x0*d[0]/(A*A) - 2*y0*d[1]/(B*B) + d[2]/C;
    float c = x0*x0/(A*A) - y0*y0/(B*B) + z0/C;
    
    float t = smallerT(a, b, c);
    return {t < 1e-50? HUGE_VALF: t, paraboloid};
}
////////////////////////////////////////////////////////////////////

struct Material{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Ns;
};

struct Scene{
    ImageRGB            image;
    Camera              camera;
    Light               light;
    Material            material;
    Paraboloid          paraboloid;

    void render(){
        #pragma omp parallel for schedule(dynamic, 1)  // OpenMP
        for(int y = 0; y < image.height(); y++)
            for(int x = 0; x < image.width(); x++)
                image(x, y) = color_at(camera.ray(x,y));
    }

    RGB color_at(Ray ray) const{
        auto hit = intersection(ray, paraboloid);

        if(hit.t == HUGE_VALF)
            return white;

        vec3 position = ray.at(hit.t);
        vec3 normal = paraboloid.normal(position);
        return illumination(position, normal, light, material);
    }
};

int main(){
    int w = 800;
    int h = 600;
    float R = 1e5;

    Scene scene{
        ImageRGB{w, h},
        Camera{0.1, 45*M_PI/180, w, h},
        Light{
            vec4{0.0, 13, 15, 1.0}, // position
	        vec3{0.2, 0.2, 0.2}, // Ia
	        vec3{1.0, 1.0, 1.0}, // Id
	        vec3{1.0, 1.0, 1.0}  // Is
        },
        Material{
            vec3{1.0, 1.0, 1.0}, // Ka
            vec3{0.5, 0.5, 0.5}, // Kd
            vec3{0.5, 0.5, 0.5}, // Ks
            128 // Ns
        },
        Paraboloid{1, 2, 0.5}
    };
    int N = 60;
    for(int i = 0; i < N; i++){
        float theta = i*2*M_PI/N;
        vec3 pos = {15*cosf(theta), 15*sinf(theta), 5};
    	scene.camera.lookAt(pos, {0, 0, 0}, {0, 0, 1});
        scene.render();
        scene.image.save_frame(i, "anim/output", "png");
    }
}