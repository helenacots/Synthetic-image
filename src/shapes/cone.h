#ifndef CONE_H
#define CONE_H

#include <iostream>
#include <string>

#include "shape.h"
#include "../core/eqsolver.h"

class Cone : public Shape
{
public:
	Cone() = delete;
	Cone(const double a_, const double b_, const double c_, const double height_,const Matrix4x4 &t_, Material *material_);

	Vector3D getNormalWorld(const Vector3D &pt_world) const;

	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

	std::string toString() const;

private:
	double a;
	double b;
	double c;
	double height;

};

std::ostream& operator<<(std::ostream &out, const Cone &e);

#endif // CONE_H


