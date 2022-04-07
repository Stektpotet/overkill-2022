#pragma once

#include "MeshUtility.hpp"

namespace OK
{
    Mesh make_cube(const glm::vec3 scale, const glm::vec3 relative_position, const bool invert)
    {
        std::uint16_t indices[36];
        glm::vec3 vertices[36] {
			{ -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
			{  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
			{  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
			{  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
			{ -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
			{ -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },

            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },

            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },

            { -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
		};
		/*GLfloat normal[] = {
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};
		GLfloat uvCoordinates[] = {
			 0.0f,  0.0f,
			 1.0f,  0.0f,
			 1.0f,  1.0f,
			 1.0f,  1.0f,
			 0.0f,  1.0f,
			 0.0f,  0.0f,

			 0.0f,  0.0f,
			 1.0f,  0.0f,
			 1.0f,  1.0f,
			 1.0f,  1.0f,
			 0.0f,  1.0f,
			 0.0f,  0.0f,

			 0.0f,  0.0f,
			 1.0f,  0.0f,
			 1.0f,  1.0f,
			 1.0f,  1.0f,
			 0.0f,  1.0f,
			 0.0f,  0.0f,
			 
			 0.0f,  0.0f,
			 1.0f,  0.0f,
			 1.0f,  1.0f,
			 1.0f,  1.0f,
			 0.0f,  1.0f,
			 0.0f,  0.0f,
			 
			 0.0f,  0.0f,
			 1.0f,  0.0f,
			 1.0f,  1.0f,
			 1.0f,  1.0f,
			 0.0f,  1.0f,
			 0.0f,  0.0f,
			 
			 0.0f,  0.0f,
			 1.0f,  0.0f,
			 1.0f,  1.0f,
			 1.0f,  1.0f,
			 0.0f,  1.0f,
			 0.0f,  0.0f,
		};
        Mesh*/
        return Mesh();
    }
}
