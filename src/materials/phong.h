#ifndef PHONG
#define PHONG

#include "material.h"
#include "../core/vector3d.h"
#include "../core/utils.h"

class Phong : public Material
{
public:
	Phong() = delete;

	Phong(Vector3D kd_, Vector3D ks_, double coefN_);

	Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
		const Vector3D &wi) const;
	Vector3D getDiffuseCoefficient() const;


	bool hasSpecular() const;
	bool hasTransmission() const;
	bool hasDiffuseOrGlossy() const;
	double getIndexOfRefraction() const;

private:
	Vector3D kd;
	Vector3D ks;
	double coefN;
	//Vector3D at; 

}; 

#endif // PHONG