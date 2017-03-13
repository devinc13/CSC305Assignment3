#ifndef PLANEH
#define PLANEH

#include "hitable.h"

class plane : public hitable {
public:
	plane() {}
	plane(vec3 n, vec3 p, material *m) : normal(n), point(p), mat_ptr(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 normal;
	vec3 point;
	material *mat_ptr;
};

bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	float temp = dot(normal, (point - r.origin())) / dot(normal, r.direction());
	if (temp > 0 && temp < t_max && temp > t_min) {
		rec.t = temp;
		rec.p = r.point_at_parameter(temp);
		rec.normal = normal;
		rec.mat_ptr = mat_ptr;
		return true;
	}

	return false;
}

#endif
