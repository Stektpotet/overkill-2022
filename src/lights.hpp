#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>


struct PointLight {
	PointLight() :
		intensities{ glm::vec3{1.0f, 1.0f, 1.0f} },
		constant{ 1.0f }, 
		linear{ 0.014f }, 
		quadratic { 0.0007f }
	{}

	glm::vec3 intensities;
	float constant;
	float linear;
	float quadratic;
};

struct PointLightData {
	glm::vec3 position;		// 0   +  3*4 = 12
	glm::vec3 intensities;	// 12  +  3*4 = 24
	// falloff rates		
	float constant;			// 24 + 4	  = 28 
	float linear;			// 24 + 4	  = 28 
	float quadratic;		// 24 + 4	  = 28 
};