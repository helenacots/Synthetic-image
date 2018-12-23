#include "cylinder.h"

Cylinder::Cylinder(const double radius_, const double maxLength_, const Matrix4x4 &t_, Material *material_)
	: Shape(t_, material_), radius(radius_), maxLength(maxLength_)
{ }

Vector3D Cylinder::getNormalWorld(const Vector3D &pt_world) const
{
	// Transform the point to local coordinates
	//Point3D pt_local = worldToObject.applyTransform(pt_world);
	Vector3D pt_local = worldToObject.transformPoint(pt_world);

	// Normal in local coordinates --> normal in point p
	Vector3D n(pt_local.x, 0, pt_local.z);

	// Transform the normal to world coordinates
	//Normal nWorld = objectToWorld.applyTransform(n);
	// Multiply the normal by the transpose of the inverse
	Matrix4x4 inverseTransposed;
	worldToObject.transpose(inverseTransposed);
	Vector3D nWorld = inverseTransposed.transformVector(n);

	// Check whether applying the transform to a normalized
	// normal allways yields a normalized normal
	return(nWorld.normalized());
}

bool Cylinder::rayIntersect(const Ray &ray, Intersection &its) const
{
	// Pass the ray to local coordinates
	//Ray r = worldToObject.applyTransform(ray);
	Ray r = worldToObject.transformRay(ray);

	// The ray-sphere intersection equation can be expressed in the
	// form A*t^2 + B*t + C = 0, where:
	
	double A = std::pow(r.d.x,2)+ std::pow(r.d.z, 2);
	double B = 2 * (r.d.x*r.o.x + r.d.z*r.o.z);
	double C = std::pow(r.o.x,2) + std::pow(r.o.z,2) - radius*radius;

	// Now we need to solve this quadratic equation for t
	EqSolver solver;
	rootValues roots;
	bool hasRoots = solver.rootQuadEq(A, B, C, roots);

	if (!hasRoots)
	{
		return false;
	}

	// Test whether both root values (i.e., the intersection points)
	// are greater or smaller than r.maxT and r.minT, respectivelly
	if (roots.values[0] > r.maxT || roots.values[1] < r.minT)
		return false;

	double tHit = roots.values[0]; //first intersection
	double tHit2 = roots.values[1]; //second intersection

	if (roots.values[0] < ray.minT) {
		// If so, then we set tHit to t1 (hoping we are in case 2!)
		tHit = roots.values[1];
		// If we are in case 3, then return false (meaning there is no intersection detected
		if (tHit > r.maxT)
			return false;
	}

	// Compute Intersection Point (in local coordinates)
	Vector3D itsPoint = r.o + r.d*tHit;

	if (itsPoint.y < -maxLength || itsPoint.y>maxLength) {
		if (tHit2 > ray.minT) {

			itsPoint = r.o + r.d*tHit2;
			if (itsPoint.y < -maxLength || itsPoint.y>maxLength)
				return false;
			its.itsPoint = objectToWorld.transformPoint(itsPoint);
			its.normal = -its.normal;
			its.shape = this;

			if (tHit2 > r.maxT)
				return false;
			ray.maxT = tHit2;
			return true;
		}
		return false;
	}

	// Transform to world coordinates
	its.itsPoint = objectToWorld.transformPoint(itsPoint);
	// Compute the normal at the intersection point (in world coordinates)
	its.normal = getNormalWorld(its.itsPoint);
	// Store the shape the intersection point lies in
	its.shape = this;
	// Update the maxT in the ray so as to terminate earlier subsequent
	// intersection tests with other shapes
	ray.maxT = tHit;
	return true;
}

bool Cylinder::rayIntersectP(const Ray &ray) const
{
	// Pass the ray to local coordinates
	Ray r = worldToObject.transformRay(ray);

	// The ray-sphere intersection equation can be expressed in the
	// form A*t^2 + B*t + C = 0, where:
	double A = std::pow(r.d.x, 2) + std::pow(r.d.z, 2);
	double B = 2 * (r.d.x*r.o.x + r.d.z*r.o.z);
	double C = std::pow(r.o.x, 2) + std::pow(r.o.z, 2) - radius*radius;
	// Now we need to solve this quadratic equation for t
	EqSolver solver;
	rootValues roots;
	bool hasRoots = solver.rootQuadEq(A, B, C, roots);

	if (!hasRoots)
	{
		return false;
	}

	// Test whether both root values (i.e., the intersection points)
	// are greater or smaller than r.maxT and r.minT, respectivelly
	if (roots.values[0] > r.maxT || roots.values[1] < r.minT)
		return false;

	double tHit = roots.values[0]; //first intersection
	double tHit2 = roots.values[1]; //second intersection

	if (roots.values[0] < ray.minT) {
		// If so, then we set tHit to t1 (hoping we are in case 2!)
		tHit = roots.values[1];
		// If we are in case 3, then return false (meaning there is no intersection detected
		if (tHit > r.maxT)
			return false;
	}

	Vector3D itsPoint = r.o + r.d*tHit;

	if (itsPoint.y < -maxLength || itsPoint.y>maxLength) {
		if (tHit2 > ray.minT) { //If there is no first intersection we test if there is a second one
			itsPoint = r.o + r.d*tHit2;
			if (itsPoint.y < -maxLength || itsPoint.y>maxLength)
				return false;
			if (tHit2 > r.maxT)
				return false;
			ray.maxT = tHit2;
			return true;
		}
		return false;
	}
	// Update the maxT in the ray so as to terminate earlier subsequent
	// intersection tests with other shapes
	ray.maxT = tHit;
	
	return true;
}

std::string Cylinder::toString() const
{
	std::stringstream c;
	c << "[ " << std::endl
		<< " Center (World) = " << objectToWorld.transformPoint(Vector3D(0, 0, 0)) << std::endl
		<< "]" << std::endl;

	return c.str();
}

std::ostream& operator<<(std::ostream &out, const Cylinder &c)
{
	out << c.toString();
	return out;
}
