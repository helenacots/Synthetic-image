#include "transmissive.h"

//Material *transmissive = new Transmissive(1.1, Vector3D(1));

Transmissive::Transmissive(double n2, Vector3D specularVector)
	: Material()
{
	this->indexRefr = n2;
	this->specular = specularVector;
}


double Transmissive::getIndexOfRefraction() const
{
	return indexRefr;
}

Vector3D Transmissive::getDiffuseCoefficient() const
{
	return Vector3D();
}

Vector3D Transmissive::getReflectance(const Vector3D &n, const Vector3D & wo, const Vector3D & wi)const
{

	//Vector3D wr = n * 2 * dot(n, wi) - wi;
	Vector3D wr = n.operator*(dot(n, wo)) * 2 - wo;

	//kd: light color, ks: sphere color
	//Vector3D reflectance = kd * dot(wi, n) + ks * std::pow(dot(wo, wr), coefN);

	return wr;
}

bool Transmissive::hasSpecular()const
{
	return false;
}

bool Transmissive::hasTransmission()const
{
	return true;
}

bool Transmissive::hasDiffuseOrGlossy()const
{
	return false;
}
