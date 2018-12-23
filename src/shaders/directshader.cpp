#include "directshader.h"
#include "../materials/phong.h"
#include "../materials/material.h"
#include "../shapes/cone.h"


DirectShader::DirectShader(Vector3D bgColor_) :
	Shader(bgColor)
{}



Vector3D DirectShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const {

	if (r.depth > 10)
		return Vector3D();

	Intersection its = Intersection();
	Vector3D Lo = Vector3D(); 
	Vector3D wo = -r.d;
	size_t nL = lsList.size(); // number of point light sources
	Vector3D color(0, 0, 0);

	if (Utils::getClosestIntersection(r, objList, its)) {
		const Shape* sh = its.shape; // take the shape of the object to take the material to get the reflectance

		const Material& material = sh->getMaterial();
		Vector3D p = its.itsPoint; // point p
		Vector3D n = its.normal;


		//PHONG
		if (material.hasDiffuseOrGlossy()) {

			for (int s = 0; s < nL; s++) { // for each of the point light sources
				const PointLightSource ls = lsList.at(s);  // take a point light source
				Vector3D lsPos = ls.getPosition();	// save its position
				Vector3D Li = ls.getIntensity(p);	// get the amount of intensity from the light source that arrives to p
				Vector3D wi = (lsPos - p).normalized(); //calculate the w incident
				Ray rwi(p, wi);
				rwi.maxT = (lsPos - p).length();
				Vector3D reflect = material.getReflectance(n, wo, wi);

				//check if the point light source ls contributes to p
				bool Vi = !Utils::hasIntersection(rwi, objList);
				Lo = Lo.operator+(Vector3D(Utils::multiplyPerCanal(reflect, Li)*Vi));
			}

			return Lo;
		}


		//MIRROR
		else if (material.hasSpecular()){

			Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
			Ray reflectionRay(p,wr,r.depth+1);
			return computeColor(reflectionRay, objList, lsList);

		}

		//TRANSMISSIVE
		else if (material.hasTransmission()) {

			double n1 = 1;
			double nt = material.getIndexOfRefraction() / n1;
			double cosAlpha = dot(wo, n); //testejar el cosinus de aplha
			if (cosAlpha < 0) {
				n = -its.normal;
				nt = 1 / nt;
				cosAlpha = dot(wo.normalized(), n.normalized()); //testejar el cosinus de aplha
			}
			
			double cosBetha;
			if (!Utils::isTotalInternalReflection(nt,cosAlpha,cosBetha)) {
				Vector3D wt = r.d*nt + n*(nt*cosAlpha - cosBetha);
			
				Ray refractionRay(p, wt.normalized(), r.depth + 1);
				return computeColor(refractionRay, objList, lsList);
			//	return (wt + Vector3D(1.0)) / 2.0;
				//return Vector3D(1, 0, 0);
			}
			else {
				//calcular specular reflection
				Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
				Ray reflectionRay(p, wr, r.depth + 1);
				return computeColor(reflectionRay, objList, lsList);
				//return Vector3D(0,1,0);

			}

		}

	}
	else 
		return bgColor;
}
