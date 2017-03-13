#include <iostream>
#include <random>
#include "sphere.h"
#include "plane.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "rectangle.h"
#include "flip_normals.h"

// Get the color of the passed in ray
vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return emitted + attenuation * color(scattered, world, depth + 1);
		} else {
			return emitted;
		}
	} else {
		return vec3(0, 0, 0);
	}
}

int main() {
	int nx = 800;
	int ny = 800;
	int ns = 20;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	material *light = new diffuse_light(new constant_texture(vec3(1, 1, 1)));
	material *checkered = new lambertian(new checker_texture(new constant_texture(vec3(0.2, 0.2, 0.4)), new constant_texture(vec3(0.9, 0.9, 0.9))));

	hitable *list[7];
	float R = cos(M_PI / 4);
	list[0] = new plane(vec3(0, 1, 0), vec3(0, 0, 0), checkered);
	list[1] = new sphere(vec3(-2, 1, -1), 1, new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5))));
	list[2] = new sphere(vec3(0, 1, 0), 1, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(2, 1, 1), 1, new dielectric(1.5));
	list[4] = new sphere(vec3(2, 1, 1), -0.95, new dielectric(1.5));
	list[5] = new sphere(vec3(-2, 8, 0), 3, light);
	hitable *world = new hitable_list(list, 6);

	//hitable **list = new hitable*[13];
	//int i = 0;
	//material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	//material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	//material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	//material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	//material *checkered = new lambertian(new checker_texture(new constant_texture(vec3(0.2, 0.2, 0.4)), new constant_texture(vec3(0.9, 0.9, 0.9))));

	//list[i++] = new flip_normals(new yz_rectangle(0, 555, 0, 555, 555, green));
	//list[i++] = new yz_rectangle(0, 555, 0, 555, 0, red);
	//list[i++] = new xz_rectangle(213, 343, 227, 332, 554, light);
	//list[i++] = new flip_normals(new xz_rectangle(0, 555, 0, 555, 555, white));
	//list[i++] = new xz_rectangle(0, 555, 0, 555, 0, checkered);
	//list[i++] = new flip_normals(new xy_rectangle(0, 555, 0, 555, 555, white));

	//list[i++] = new sphere(vec3(75, 25, 300), 50, new lambertian(new constant_texture(vec3(0.8, 0.3, 0.3))));
	//list[i++] = new sphere(vec3(278, 200, 400), 100, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	//list[i++] = new sphere(vec3(200, 50, 500), 100, new metal(vec3(0.8, 0.6, 0.5), 1.0));
	//list[i++] = new sphere(vec3(150, 400, 400), 50, new metal(vec3(0.8, 0.6, 0.5), 0.1));
	//list[i++] = new sphere(vec3(300, 50, 200), 100, new dielectric(1.6));
	//list[i++] = new sphere(vec3(500, 50, 500), 100, new dielectric(1.5));
	//list[i++] = new sphere(vec3(500, 50, 500), -90, new dielectric(1.5));
	//
	//hitable *world = new hitable_list(list, i);

	//vec3 lookfrom(278, 278, -800);
	//vec3 lookat(278, 278, 0);
	//float dist_to_focus = 10.0;
	//float aperture = 0.0;
	//float vfov = 40.0;

	//camera camera(vfov, float(nx) / float(ny), lookfrom, lookat, vec3(0,1,0), aperture, dist_to_focus);

	vec3 lookfrom(2, 2, 10);
	vec3 lookat(0, 1, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;
	float vfov = 35.0;

	camera camera(vfov, float(nx) / float(ny), lookfrom, lookat, vec3(0, 1, 0), aperture, dist_to_focus);


	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> random(0, 1);

	#pragma omp parallel for
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random(gen)) / float(nx);
				float v = float(j + random(gen)) / float(ny);
				ray r = camera.get_ray(u, v);
				col += color(r, world, 0);
			}

			// Get the average color
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
 }