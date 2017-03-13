#ifndef RECTANGLEH
#define RECTANGLEH

#include "hitable.h"

class xy_rectangle : public hitable {
public:
	xy_rectangle() {}
	xy_rectangle(float _x0, float _x1, float _y0, float _y1, float _k, material *mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	float x0, x1, y0, y1, k;
	material *mp;
};

bool xy_rectangle::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	float t = (k - r.origin().z()) / r.direction().z();

	if (t < tmin || t > tmax) {
		return false;
	}

	float x = r.origin().x() + t * r.direction().x();
	float y = r.origin().y() + t * r.direction().y();

	if (x < x0 || x > x1 || y < y0 || y > y1) {
		return false;
	}

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0, 0, 1);
	return true;
}


class xz_rectangle : public hitable {
public:
	xz_rectangle() {}
	xz_rectangle(float _x0, float _x1, float _z0, float _z1, float _k, material *mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	float x0, x1, z0, z1, k;
	material *mp;
};

bool xz_rectangle::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	float t = (k - r.origin().y()) / r.direction().y();

	if (t < tmin || t > tmax) {
		return false;
	}

	float x = r.origin().x() + t * r.direction().x();
	float z = r.origin().z() + t * r.direction().z();

	if (x < x0 || x > x1 || z < z0 || z > z1) {
		return false;
	}

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0, 1, 0);
	return true;
}


class yz_rectangle : public hitable {
public:
	yz_rectangle() {}
	yz_rectangle(float _y0, float _y1, float _z0, float _z1, float _k, material *mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	float y0, y1, z0, z1, k;
	material *mp;
};

bool yz_rectangle::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	float t = (k - r.origin().x()) / r.direction().x();

	if (t < tmin || t > tmax) {
		return false;
	}

	float y = r.origin().y() + t * r.direction().y();
	float z = r.origin().z() + t * r.direction().z();

	if (y < y0 || y > y1 || z < z0 || z > z1) {
		return false;
	}

	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(1, 0, 0);
	return true;
}

#endif
