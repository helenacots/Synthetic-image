#ifndef TRANSMISSIVE
#define TRANSMISSIVE

#include "material.h"
#include "../core/vector3d.h"
#include "../core/utils.h"


class Transmissive : public Material
{
public:
	Transmissive() = delete;

	Transmissive(double indexRefr, Vector3D specularVector);

	Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
		const Vector3D &wi) const;


	bool hasSpecular() const;
	bool hasTransmission() const;
	bool hasDiffuseOrGlossy() const;
	double getIndexOfRefraction() const;
	Vector3D getDiffuseCoefficient() const;


private:
	Vector3D kd;
	Vector3D ks;
	Vector3D specular;
	double indexRefr;

};

#endif // Transmissive