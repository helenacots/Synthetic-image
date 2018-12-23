#include "mirror.h"

//Material *mirror = new Mirror(Vector3D(1, 0.9, 0.85));

Mirror::Mirror(Vector3D specularColor)
	: Material(), ks(specularColor)
{
	this->coefN;
	this->kd = Vector3D(0,0,255);
}


double Mirror::getIndexOfRefraction() const
{
	return 0.0; //because perfect specular reflection
}

Vector3D Mirror::getDiffuseCoefficient() const
{
	return Vector3D();
}
Vector3D Mirror::getReflectance(const Vector3D &n, const Vector3D & wo, const Vector3D & wi)const
{
	Vector3D wr = n.operator*(dot(n, wi))*2 - wi;
	//kd: light color, ks: sphere color
	Vector3D reflectance = kd * dot(wi, n) + ks * std::pow(dot(wo, wr), coefN);

	//Vector3D reflectance = ks * std::pow(dot(wo, wr), coefN);

	return reflectance;
}

bool Mirror::hasSpecular()const
{
	return true;
}

bool Mirror::hasTransmission()const
{
	return false;
}

bool Mirror::hasDiffuseOrGlossy()const
{
	// function of type boolean. Returns true if the
	//material has glossy or diffuse reflections at the surface, and false otherwise.

	return false;
}
