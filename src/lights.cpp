#include "lights.h"

PointLight::PointLight(float radius) :
	intensities{ glm::vec3{1.0f, 1.0f, 1.0f} },
	constant{ 1.0f }
{
	linear = 2.0f / radius;
	quadratic = 1.0f / (radius * radius);
}

void PointLight::setRadius(float radius) {
	linear = 2.0f / radius;
	quadratic = 1.0f / (radius * radius);
}


SpotLight::SpotLight(float reach, float cutoffAngle) : PointLight(reach)
{ this->cutoffAngle = cutoffAngle; }
