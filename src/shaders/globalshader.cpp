#include "globalshader.h"
#include "../materials/phong.h"
#include "../materials/material.h"
#include "../core/hemisphericalsampler.h"
#include <math.h>

GlobalShader::GlobalShader(Vector3D bgColor_, int maxDepth_) :
	Shader(bgColor), maxDepth(maxDepth_)
{
	HemisphericalSampler sampler = HemisphericalSampler();
}



Vector3D GlobalShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const {

	double nSamples = 3; //number of rays from a point 
	Intersection its;
	Vector3D Lodir;
	Vector3D Loind;
	Vector3D at(0.1);
	Vector3D wo = -r.d;
	size_t nL = lsList.size(); // number of point light sources
	//Vector3D color(0, 0, 0);
	Vector3D reflect;
	
	if (Utils::getClosestIntersection(r, objList, its)) {
		const Shape* sh = its.shape; // take the shape of the object to take the material to get the reflectance
		const Material& material = sh->getMaterial();
		Vector3D p = its.itsPoint; // point p
		Vector3D n = its.normal;


		//PHONG
		if (material.hasDiffuseOrGlossy()) {
		
			// Compute Lo_dir
			for (int s = 0; s < nL; s++) { // for each of the point light sources
				const PointLightSource ls = lsList.at(s);  // take a point light source
				Vector3D lsPos = ls.getPosition();	// save its position
				Vector3D Li = ls.getIntensity(p);	// get the amount of intensity from the light source that arrives to p
				Vector3D wi = (lsPos - p).normalized(); //calculate the w incident
				Ray rwi(p, wi);
				rwi.maxT = (lsPos - p).length();
				reflect = material.getReflectance(n, wo, wi);

				//check if the point light source ls contributes to p
				bool Vi = !Utils::hasIntersection(rwi, objList);
				Lodir += Vector3D(Utils::multiplyPerCanal(reflect, Li)*Vi);
			}

			// Compute Lo_ind
			if (r.depth == 0) {
				//then we are in the first intersection with a Phong material
				Vector3D Li_ind;
				for (int s = 0; s < nSamples; s++) {
					
					Vector3D wj = sampler.getSample(n); //random direction
					Ray secondaryRay(p, wj, r.depth + 1);
					Li_ind = computeColor(secondaryRay, objList, lsList);	// get the amount of intensity from the light source that arrives to p
					reflect = material.getReflectance(n, wj, wo);
					Loind += Utils::multiplyPerCanal(reflect, Li_ind);

				} 
				Loind = Loind / (2.0*nSamples*M_PI);
			}
			else if (r.depth == maxDepth) {
				//Loind = kd*at
				Loind = Utils::multiplyPerCanal(material.getDiffuseCoefficient(), at);
			}
			else if (r.depth!=0 && r.depth<maxDepth){
				// compute the light arriving from the normal at p
				Ray wn(p,n.normalized(),r.depth+1);
				// compute the light arriving from wr (perfect reflection)
				Vector3D wr = Utils::computeReflectionDirection(r.d, n);
				Ray rayRefl(p,wr,r.depth+1);
				// average
				Vector3D refl1 = material.getReflectance(n,n,wo);
				Vector3D Li_ind1 = computeColor(wn, objList, lsList);
				Vector3D Loind1 = Utils::multiplyPerCanal(refl1, Li_ind1);

				Vector3D refl2 = material.getReflectance(n, wr, wo);
				Vector3D Li_ind2 = computeColor(rayRefl, objList, lsList);
				Vector3D Loind2 = Utils::multiplyPerCanal(refl2, Li_ind2);

				Loind = (Loind1 + Loind2)/(4*M_PI);

			}
		
			return Lodir + Loind;
		}


		//MIRROR
		else if (material.hasSpecular()) {

			Vector3D wr = Utils::computeReflectionDirection(r.d, n);
			Ray reflectionRay(p, wr, r.depth);
			//reflectionRay.depth = r.depth; //why??
			return computeColor(reflectionRay, objList, lsList);

		}

		//TRANSMISSIVE
		else if (material.hasTransmission()) {

			double n1 = 1;
			double nt = material.getIndexOfRefraction() / n1;
			if (dot(wo, n) < 0) {
				n = -n;
				nt = 1 / nt;
			}
			double cosAlpha = dot(wo.normalized(), n.normalized()); //testejar el cosinus de aplha
			double cosBetha;
			if (!Utils::isTotalInternalReflection(nt, cosAlpha, cosBetha)) {
				Vector3D wt = -wo*nt + n*(nt*cosAlpha - cosBetha);
				Ray refractionRay(p, wt, r.depth);
				return computeColor(refractionRay, objList, lsList);
			}
			else {
				//calcular specular reflection
				Vector3D wr = Utils::computeReflectionDirection(r.d, its.normal);
				Ray reflectionRay(p, wr, r.depth);
				return computeColor(reflectionRay, objList, lsList);

			}

		}

	}
	else
		return bgColor;
}
