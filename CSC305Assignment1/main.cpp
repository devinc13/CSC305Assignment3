#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_DEPRECATE

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
#include "stb_image_write.h"
#include "bbh_node.h"

struct rgba8 {
	unsigned char r, g, b, a;
	rgba8() { }
	rgba8(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
		: r(rr), g(gg), b(bb), a(aa) { }
};

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
	int nx = 600;
	int ny = 600;
	int ns = 200;

	rgba8* pixels = new rgba8[nx * ny];

	// clear to black
	memset(pixels, 0, nx * ny * sizeof(*pixels));

	material *light = new diffuse_light(new constant_texture(vec3(1, 1, 1)));
	material *checkered = new lambertian(new checker_texture(new constant_texture(vec3(0.2, 0.2, 0.4)), new constant_texture(vec3(0.9, 0.9, 0.9))));
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *perlin = new lambertian(new noise_texture(4));

	
	int i = 0;
	hitable **boxlist = new hitable*[20];
	boxlist[i++] = new sphere(vec3(2, 3, 0), 0.5, perlin);
	boxlist[i++] = new sphere(vec3(0, 3, 0), 0.5, perlin);
	boxlist[i++] = new sphere(vec3(-2, 3, 0), 0.5, perlin);
	boxlist[i++] = new sphere(vec3(-2, 1, -1), 1, new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5))));
	boxlist[i++] = new sphere(vec3(0, 1, 0), 1, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	boxlist[i++] = new moving_sphere(vec3(0, 0.4, 4), vec3(0, 1.1, 4), 0.0, 1.0, 0.5, new lambertian(new constant_texture(vec3(0.7, 0.2, 0.3))));
	boxlist[i++] = new sphere(vec3(-2, 8, 5), 3, light);
	boxlist[i++] = new xy_rectangle(-5.5, 2.5, 0, 3, -5, red);
	boxlist[i++] = new sphere(vec3(2, 1, 1), 1, new dielectric(1.5));
	boxlist[i++] = new sphere(vec3(2, 1, 1), -0.95, new dielectric(1.5));

	int y = 0;
	hitable *list[10];
	list[y++] = new plane(vec3(0, 1, 0), vec3(0, 0, 0), perlin);
	list[y++] = new bbh_node(boxlist, i, 0, 1);

	hitable *world = new hitable_list(list, y);

	//hitable **list = new hitable*[13];
	//int i = 0;


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

	camera camera(vfov, float(nx) / float(ny), lookfrom, lookat, vec3(0, 1, 0), aperture, dist_to_focus, 0.0, 1.0);


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

			pixels[(ny - j - 1) * nx + i] = rgba8(ir, ig, ib, 255);
			//std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	// write image to file
	stbi_write_png("raytrace.png", nx, ny, 4, pixels, nx * 4);
	system("raytrace.png");

	delete[] pixels;
 }