#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H

#include "shader.h"
#include "../core/hemisphericalsampler.h"

class GlobalShader : public Shader
{
public:
	GlobalShader() = delete;
	GlobalShader(Vector3D bgColor_, int maxDepth);

	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;
	

private:
	int maxDepth; // Used to store the value of the maximum distance which can be visualized
	Vector3D color; // Used to store the visualization color
	HemisphericalSampler sampler;
	
};

#endif // GLOBALSHADER_H

