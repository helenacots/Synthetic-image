#ifndef ELIPSOIDE_H
#define ELIPSOIDE_H

#include <iostream>
#include <string>

#include "shape.h"
#include "../core/eqsolver.h"

class Elipsoide : public Shape
{
public:
	Elipsoide() = delete;
	Elipsoide(const double a_, const double b_, const double c_, const Matrix4x4 &t_, Material *material_);

	Vector3D getNormalWorld(const Vector3D &pt_world) const;

	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

	std::string toString() const;

private:

	double a;
	double b;
	double c;

};

std::ostream& operator<<(std::ostream &out, const Elipsoide &e);

#endif // ELIPSOIDE_H

