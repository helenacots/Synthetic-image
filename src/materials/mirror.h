#ifndef MIRROR
#define MIRROR

#include "material.h"
#include "../core/vector3d.h"
#include "../core/utils.h"


class Mirror : public Material
{
public:
	Mirror() = delete;

	//PERFECT SPECULAR REFLECTION

	Mirror(Vector3D specularColor);

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
	double coefN;

};

#endif // MIRROR