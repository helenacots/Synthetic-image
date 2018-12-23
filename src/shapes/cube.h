#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include "../core/eqsolver.h"


class Cube : public Shape
{
public:
	Cube() = delete;
	Cube(const double height_, const Matrix4x4 &t_, const Vector3D p0World_,const Vector3D &normal_,
		Material *mat_);

	// Get the normal at a surface point in world coordinates
	Vector3D getNormalWorld() const;

	// Ray/plane intersection methods
	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

	// Convert triangle to String
	std::string toString() const;

private:
	// A point belonging to the plane, and the normal at the plane
	Vector3D p0World;
	Vector3D nWorld;
	double height;
};

std::ostream &operator<<(std::ostream &out, const Cube &t);

#endif // CUBE_H
