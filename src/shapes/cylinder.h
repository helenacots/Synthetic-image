#ifndef CYLINDER_H
#define CYLINDER_H

#include <iostream>
#include <string>

#include "shape.h"
#include "../core/eqsolver.h"

class Cylinder : public Shape
{
public:
	Cylinder() = delete;
	Cylinder(const double radius_, const double maxLength_, const Matrix4x4 &t_, Material *material_);

	Vector3D getNormalWorld(const Vector3D &pt_world) const;

	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

	std::string toString() const;

private:
	double radius;
	double maxLength;

};

std::ostream& operator<<(std::ostream &out, const Cylinder &e);

#endif // CYLINDER_H
