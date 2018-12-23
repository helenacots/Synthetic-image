#include "cone.h"

Cone::Cone(const double a_, const double b_, const double c_, const double height_ ,const Matrix4x4 &t_, Material *material_)
	: Shape(t_, material_), a(a_), b(b_), c(c_), height(height_)
{ }

Vector3D Cone::getNormalWorld(const Vector3D &pt_world) const
{
	// Transform the point to local coordinates
	//Point3D pt_local = worldToObject.applyTransform(pt_world);
	Vector3D pt_local = worldToObject.transformPoint(pt_world);

	// Normal in local coordinates --> normal in point p
	double Vx = pt_local.x / sqrt(std::pow((pt_local.x), 2) + std::pow(pt_local.z, 2));
	double Vz = pt_local.z / sqrt(std::pow(pt_local.x, 2) + std::pow(pt_local.z, 2));
	Vector3D n(Vx*height / a, a / height, Vz*height / a);

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

bool Cone::rayIntersect(const Ray &ray, Intersection &its) const
{
	// Pass the ray to local coordinates
	//Ray r = worldToObject.applyTransform(ray);
	Ray r = worldToObject.transformRay(ray);

	// The ray-sphere intersection equation can be expressed in the
	// form A*t^2 + B*t + C = 0, where:

	double A = std::pow(b*c*r.d.x, 2) + std::pow(a*b*r.d.z, 2) - std::pow(a*c*r.d.y, 2);
	double B = 2 * (std::pow(b*c,2)*r.d.x*r.o.x + std::pow(a*b, 2)*r.d.z*r.o.z - std::pow(a*c, 2)*r.d.y*r.o.y);
	double C = std::pow(b*c*r.o.x, 2) + std::pow(a*b*r.o.z, 2) - std::pow(a*c*r.o.y, 2); //correct

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

	// We initialize the tHit for case 1
	double tHit = roots.values[0];
	double tHit2 = roots.values[1];

	// We check where we could possibly be in case 2 or 3
	if (tHit < ray.minT) {
		// If so, then we set tHit to t1 (hoping we are in case 2!)
		tHit = roots.values[1];
		// If we are in case 3, then return false (meaning there is no intersection detected
		if (tHit > r.maxT)
			return false;
	}

	// Compute Intersection Point (in local coordinates)
	Vector3D itsPoint = r.o + r.d*tHit;

	if (itsPoint.y < -height || itsPoint.y>0) {
		if (tHit2 > ray.minT) {
			// If so, then we set tHit to t1 (hoping we are in case 2!)
			itsPoint = r.o + r.d*tHit2;
			if (itsPoint.y < -height || itsPoint.y>0)
				return false;
			// Transform to world coordinates
			its.itsPoint = objectToWorld.transformPoint(itsPoint);
			// Compute the normal at the intersection point (in world coordinates)
			its.normal = getNormalWorld(its.itsPoint);
			its.normal = -its.normal;
			// Store the shape the intersection point lies in
			its.shape = this;
			// If we are in case 3, then return false (meaning there is no intersection detected
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

bool Cone::rayIntersectP(const Ray &ray) const
{
	// Pass the ray to local coordinates
	Ray r = worldToObject.transformRay(ray);

	// The ray-sphere intersection equation can be expressed in the
	// form A*t^2 + B*t + C = 0, where:
	double A = std::pow(b*c*r.d.x, 2) + std::pow(a*b*r.d.z, 2) - std::pow(a*c*r.d.y, 2);
	double B = 2 * (std::pow(b*c, 2)*r.d.x*r.o.x + std::pow(a*b, 2)*r.d.z*r.o.z - std::pow(a*c, 2)*r.d.y*r.o.y);
	double C = std::pow(b*c*r.o.x, 2) + +std::pow(a*b*r.o.z, 2) - +std::pow(a*c*r.o.y, 2);
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

	// We initialize the tHit for case 1
	double tHit = roots.values[0];
	double tHit2 = roots.values[1];
	// We check where we could possibly be in case 2 or 3
	if (roots.values[0] < ray.minT) {
		// If so, then we set tHit to t1 (hoping we are in case 2!)
		tHit = roots.values[1];
		// If we are in case 3, then return false (meaning there is no intersection detected
		if (tHit > r.maxT)
			return false;
	}

	Vector3D itsPoint = r.o + r.d*tHit;

	if (itsPoint.y < -height || itsPoint.y>0) {
		if (tHit2 > ray.minT) {
			// If so, then we set tHit to t1 (hoping we are in case 2!)
			tHit2 = roots.values[1];
			Vector3D itsPoint = r.o + r.d*tHit2;
			if (itsPoint.y < -height || itsPoint.y>0)
				return false;
			// If we are in case 3, then return false (meaning there is no intersection detected
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

std::string Cone::toString() const
{
	std::stringstream c;
	c << "[ " << std::endl
		<< " Center (World) = " << objectToWorld.transformPoint(Vector3D(0, 0, 0)) << std::endl
		<< "]" << std::endl;

	return c.str();
}

std::ostream& operator<<(std::ostream &out, const Cone &c)
{
	out << c.toString();
	return out;
}
