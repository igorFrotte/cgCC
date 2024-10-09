#include "Image.h"
#include "raytracing.h"
#include "transforms.h"
#include <random>
#include <chrono>

#define USE_BOUNDING_BOX
#define USE_OCTREE
#include "RTMesh.h"

static double random_value(double a, double b){
    static std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(a, b);
    return distribution(generator);
}

// Direção aleatória ponderada pelo cosseno
vec3 cos_random_dir(vec3 n){
    double r1 = random_value(0, 2*M_PI); 
    double r2 = random_value(0, 1); 

    // base ortonormal: n, u, v
    vec3 u = normalize( cross((fabsf(n[0]) > .1 ? vec3{0, 1, 0} : vec3{1, 0, 0}), n) );
    vec3 v = cross(n, u);

    // direção aleatória dentro do hemisfério
    return normalize(sqrt(r2)*(cos(r1)*u + sin(r1)*v) + sqrt(1 - r2)*n);
}

vec3 sky_color(){
    return vec3{1, 1, 1}; // cor do céu
}

void show_progress(int current_value, int max_value){
    float percent = (float)current_value/(float)max_value*100;
    fprintf(stderr, "\r%.1f%%", percent);
}

enum IlluminationModel{
    EMISSION, DIFFUSE, SPECULAR
};

IlluminationModel get_illumination_model(MaterialInfo material){
    if(material.illum == 0)
        return EMISSION;
    
    if(material.illum == 1)
        return DIFFUSE;

    // Choose randomly between diffuse and specular
    return (random_value(0, 1) < 0.5)? DIFFUSE : SPECULAR;
}

struct Scene{
    ImageRGB image;
    Camera camera;
    std::vector<RTMesh> meshes;

    void render(int nsamples, int nsave=1){
        Image<vec3> sum{image.width(), image.height()};
        sum.fill(vec3{0, 0, 0});
        
        for(int n = 1; n <= nsamples; n++){
            #pragma omp parallel for schedule(dynamic, 1)  // OpenMP
            for(int y = 0; y < image.height(); y++){
                for(int x = 0; x < image.width(); x++){
                    sum(x, y) = sum(x,y) + color_at(x,y);
                    if(n%nsave == 0)
                        image(x,y) = toColor(1.0/n*sum(x,y));
                }
            }
            if(n%nsave == 0)
	            image.saveBMP("output.bmp");
            show_progress(n, nsamples);
        }
    }

    vec3 color_at(int x, int y) const{
        float rx = x + random_value(-0.1, 0.1);
        float ry = y + random_value(-0.1, 0.1);
        Ray ray = camera.ray(rx, ry);
        return trace_path(ray, 0);
    }

    vec3 trace_path(Ray ray, int depth) const{
        if(depth > 10)
            return vec3{0, 0, 0};

        MatTriIntersection I = min_intersection(ray, meshes);
        if(I.t == HUGE_VALF)
            return sky_color();
        
        vec3 n = normalize(I.normal); 
        sample_textures(I.material, I.texCoords);

        IlluminationModel model = get_illumination_model(I.material);

        if(model == EMISSION)
            return I.material.Kd;

        if(model == DIFFUSE){
            vec3 dir = cos_random_dir(n);
            return I.material.Kd*trace_path(Ray{I.position, dir}, depth+1);
        }

        if(model == SPECULAR){
            vec3 dir = reflect(ray.dir, n);
            return I.material.Ks*trace_path(Ray{I.position, dir}, depth+1);
        }

        return vec3{0, 0, 0};
    }
};

MaterialInfo get_material(int illum, vec3 Kd, vec3 Ks=vec3{0,0,0}){
    MaterialInfo mat = standard_material();
    mat.illum = illum;
    mat.Kd = Kd;
    mat.Ks = Ks;
    return mat;
}

std::vector<RTMesh> get_meshes(){
    std::vector<RTMesh> meshes;
    meshes.emplace_back("modelos/wall.obj", scale(20, 20, 20), get_material(1, {0.4, 0.4, 1.0}));
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
    int nsamples  = 64;

    Scene scene{
        ImageRGB{w, h},
        Camera{0.1, 45*M_PI/180, w, h},
        get_meshes()
    };
    scene.camera.lookAt({10, 7, 15}, {0, 4, 0}, {0, 1, 0});

	auto start = std::chrono::high_resolution_clock::now();
	scene.render(nsamples, 4);
	scene.image.saveBMP("output.bmp");
	auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
	std::cout << '\n' << nsamples << " samples - " << diff.count() << "s\n";
}
