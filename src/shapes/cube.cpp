#include "cube.h"

Cube::Cube(const double height_, const Matrix4x4 &t_, const Vector3D p0World_, const Vector3D &normal_,
	Material *mat_) :
	Shape(t_, mat_),
	p0World(p0World_),nWorld(normal_.normalized()), height(height_)
{ }

Vector3D Cube::getNormalWorld() const
{
	return nWorld;
}

bool Cube::rayIntersectP(const Ray &rayWorld) const
{

	Ray r_local = worldToObject.transformRay(rayWorld);
	Vector3D n_local = worldToObject.transformPoint(nWorld);

	// Compute the denominator of the tHit formula
	double denominator = dot(rayWorld.d, nWorld);

	// If the denominator is very close to zero, then the ray and the
	// plane are almost parallel
	if (std::abs(denominator) < Epsilon)
		return false;

	// Effectivelly compute the intersection point
	double tHit = dot(p0World-rayWorld.o, nWorld)/denominator;
	// Is tHit outside the bounds of the ray segment we want to test intersecion with?
	if ((tHit) < rayWorld.minT || (tHit) > rayWorld.maxT) 
		return false;

	Vector3D pHit = r_local.o + r_local.d*(tHit); 

	//Define the maximum limits of the coord. 
	double xRight =  height;
	double xLeft = - height;

	double yUp = height;
	double yBottom = - height;

	double zFront = height;
	double zBack = - height;

	//Define the size of the plane 
	if (pHit.x < xLeft || pHit.x > xRight)
		return false;
	if (pHit.y < yBottom || pHit.y > yUp)
		return false;
	if (pHit.z < zBack || pHit.z > zFront)
		return false;

	rayWorld.maxT = tHit;

	// Arriving here, we know that the ray segment intersects the plan
	return true;
}

bool Cube::rayIntersect(const Ray &rayWorld, Intersection &its) const
{
	Ray r_local = worldToObject.transformRay(rayWorld);
	Vector3D n_local = worldToObject.transformPoint(nWorld);

	// Compute the denominator of the tHit formula
	double denominator = dot(rayWorld.d, nWorld);

	// If the denominator is very close to zero, then the ray and the
	// plane are almost parallel
	if (std::abs(denominator) < Epsilon)
		return false;

	// Effectivelly compute the intersection point
	double tHit = dot(p0World.operator-(rayWorld.o), nWorld) / denominator;

	// Is tHit outside the bounds of the ray segment we want to test intersecion with?
	if (tHit < rayWorld.minT || tHit > rayWorld.maxT)
		return false;

	Vector3D pHit = r_local.o + r_local.d*tHit;

	double xRight = height;
	double xLeft = - height;

	double yRight = height;
	double yLeft = - height;

	double zRight = height;
	double zLeft = - height;

	if (pHit.x < xLeft || pHit.x > xRight)
		return false;
	if (pHit.y < yLeft || pHit.y > yRight)
		return false;
	if (pHit.z < zLeft || pHit.z > zRight)
		return false;

	// Fill the intersection details
	its.itsPoint = objectToWorld.transformPoint(pHit);;
	its.normal = nWorld;
	// Store the shape the intersection point lies in
	its.shape = this;
	// Update the ray maxT
	rayWorld.maxT = tHit;

	return true;
}


std::string Cube::toString() const
{
	std::stringstream s;
	s << "\tInfinitePlan attributes: \n";

	s << "\t p0 (world coordinates): " << p0World << "\n";
	s << "\t normal (world coordinates): " << nWorld << "\n" << std::endl;

	return s.str();
}

std::ostream &operator<<(std::ostream &out, const Cube &t)
{
	out << t.toString();
	return out;
}
