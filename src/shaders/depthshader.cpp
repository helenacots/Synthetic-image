#include "depthshader.h"
#include "../core/utils.h"


DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_) :
	Shader(bgColor)
{
	this->maxDist = maxDist_;
	this->color = color_;
}



Vector3D DepthShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const {

	Intersection its;
	Vector3D distance;
	//if ray does not intersect --> background color

	if (Utils::getClosestIntersection(r, objList, its)) {

		//distancia entre origen rayo i closest intersection
		distance = its.itsPoint.operator-(r.o);
		double dist = distance.length();
		if (dist >= maxDist) {
			return bgColor;
		}

		if (dist < maxDist) {
			return color*(1.0 - dist / maxDist);
		}

	} else return bgColor;

}