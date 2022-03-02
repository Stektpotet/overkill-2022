#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace OK
{

struct Transform 
{
	glm::vec3 position = {0, 0, 0};
	glm::vec3 rotation = {0, 0, 0};
	glm::vec3 scale = {1, 1, 1};
};

struct GameObject 
{
	Transform transform;
	glm::mat4 trs;

	std::vector<GameObject*> children;
	std::vector<Component>;
};

}