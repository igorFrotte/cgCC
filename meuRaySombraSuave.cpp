#include "Image.h"
#include "raytracing.h"
#include "transforms.h"
#include "Phong.h"
#include <random>

#define USE_OCTREE
#include "RTMesh.h"

static float random_value(float a, float b){
    static std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(a, b);
    return distribution(generator);
}
            
vec3 get_random_position(vec3 position, float r){
    vec3 p = {random_value(-r, r), random_value(-r, r), random_value(-r, r)};
    return position + p;
}

void show_progress(int current_value, int max_value){
    float percent = (float)current_value/(float)max_value*100;
    fprintf(stderr, "\r%.1f%%", percent);
}

struct Scene{
    ImageRGB image;
    Camera camera;
    Light light;
    std::vector<RTMesh> meshes;

    void render(){
        #pragma omp parallel for schedule(dynamic, 1)  // OpenMP
        for(int y = 0; y < image.height(); y++){
            show_progress(y, image.height()-1);
            for(int x = 0; x < image.width(); x++)
                image(x, y) = color_at(camera.ray(x,y));
        }
    }

    RGB color_at(Ray ray) const{
        MatTriIntersection I = min_intersection(ray, meshes);
        if(I.t == HUGE_VALF)
            return white;

        sample_textures(I.material, I.texCoords);

        float shadow = 0;
        int ns = 50;
        for(int i = 0; i < ns; i++){
            vec3 lpos = get_random_position(toVec3(light.position), 0.5);
            Ray light_ray{I.position, normalize(lpos - I.position)};
            MatTriIntersection Ilight = min_intersection(light_ray, meshes);
            if(Ilight.t == HUGE_VALF)
                shadow++;
        }
        shadow /= ns;
        
        I.material.Kd = shadow*I.material.Kd;
        I.material.Ks = shadow*I.material.Ks;

        return illumination(I.position, I.normal, light, I.material);
    }
};
    
MaterialInfo get_material(RGB base_color){
    MaterialInfo mat = standard_material();
    vec3 color = toVec(base_color);
    mat.Ka = color;
    mat.Kd = color;
    mat.Ks = color;
    return mat;
}

std::vector<RTMesh> get_meshes(){
    std::vector<RTMesh> meshes;
    
    meshes.emplace_back("modelos/wall.obj", scale(20, 20, 20), get_material(blue));
    meshes.emplace_back(  
            "modelos/fred/fred.obj", 
            translate(-5, 0, -1)*rotate_y(0.2)*scale(.0006, .0006, .0006));

    meshes.emplace_back(  
            "modelos/perna/perna.obj", 
            translate(5, 0, 3)*scale(0.6, 0.6, 0.6));

    meshes.emplace_back(  
		"modelos/ellie/ellie.obj", 
		translate(1, 3.3, 5)*rotate_y(1.3)*scale(2.6, 2.6, 2.6));

    meshes.emplace_back(
		"modelos/metroid/DolBarriersuit.obj", 
		translate(1, 0, -4)*rotate_y(0.2)*scale(.4, .4, .4));

    return meshes;
}

int main(){
    int w = 800;
    int h = 600;

    Scene scene{
        ImageRGB{w, h},
        Camera{0.1, 40*M_PI/180, w, h},
        Light{
            vec4{0.0, 13, 15, 1.0}, // position
	        vec3{0.2, 0.2, 0.2}, // Ia
	        vec3{1.0, 1.0, 1.0}, // Id
	        vec3{1.0, 1.0, 1.0}  // Is
        },
        get_meshes()
    };
	scene.camera.lookAt({10, 7, 20}, {0, 4, 0}, {0, 1, 0});
    scene.render();
    scene.image.savePNG("output.png");
}