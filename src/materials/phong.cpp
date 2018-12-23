#include "phong.h"


Phong::Phong(Vector3D kd_, Vector3D ks_, double coefN_)
	: Material(), ks(ks_), coefN(coefN_) 
{
	this->kd = kd_;
}


double Phong::getIndexOfRefraction() const
{
	return 0.0;
}


Vector3D Phong::getDiffuseCoefficient() const
{
	return kd;
}

Vector3D Phong::getReflectance(const Vector3D &n, const Vector3D & wo, const Vector3D & wi)const
{

	double v1 = dot(wi, n);
	Vector3D wr = n * 2 * v1 - wi;
	//kd: light color, ks: sphere color
	double v2 = dot(wo,n);

	if (v1 < 0)
		v1 = 0;
	if (v2 < 0)
		v2 = 0;

	Vector3D reflectance = kd * v1 + ks * std::pow(v2, coefN);
	
	return reflectance;
}

bool Phong::hasSpecular()const
{
	return false;
}

bool Phong::hasTransmission()const
{
	return false;
}

bool Phong::hasDiffuseOrGlossy()const
{
	return true;
}
