#pragma once

#include "MeshUtility.hpp"

namespace OK
{
    Mesh make_cube(const glm::vec3 scale, const glm::vec3 relative_position, const bool invert)
    {
        auto indices = std::vector<std::uint16_t> {
            0,  1,  2,      2,  3,  0,
            4,  5,  6,      6,  7,  4,
            8,  9,  10,     10, 11, 8,
            12, 13, 14,     14, 15, 12,
            16, 17, 18,     18, 19, 16,
            20, 21, 22,     22, 23, 20,
        };

        auto vertices = std::vector<glm::vec3> {
			{ -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
			{  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
			{  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
			{ -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },

            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },

            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z },

            { -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z },
            {  0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
            { -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z },
		};

        auto normals = std::vector<glm::vec3> {
            { 0.0f, 0.0f, -1.0f },
            { 0.0f, 0.0f, -1.0f },
            { 0.0f, 0.0f, -1.0f },
            { 0.0f, 0.0f, -1.0f },

            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },

            { -1.0f, 0.0f, 0.0f },
            { -1.0f, 0.0f, 0.0f },
            { -1.0f, 0.0f, 0.0f },
            { -1.0f, 0.0f, 0.0f },

            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },

            { 0.0f, -1.0f, 0.0f },
            { 0.0f, -1.0f, 0.0f },
            { 0.0f, -1.0f, 0.0f },
            { 0.0f, -1.0f, 0.0f },

            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
        };

		auto uvs = std::vector<glm::vec2> {
			 { 0.0f,  0.0f, },
			 { 1.0f,  0.0f, },
			 { 1.0f,  1.0f, },
			 { 0.0f,  1.0f, },

             { 0.0f,  0.0f, },
             { 1.0f,  0.0f, },
             { 1.0f,  1.0f, },
             { 0.0f,  1.0f, },

             { 0.0f,  0.0f, },
             { 1.0f,  0.0f, },
             { 1.0f,  1.0f, },
             { 0.0f,  1.0f, },
			 
			 { 0.0f,  0.0f, },
			 { 1.0f,  0.0f, },
			 { 1.0f,  1.0f, },
			 { 0.0f,  1.0f, },
			 
             { 0.0f,  0.0f, },
             { 1.0f,  0.0f, },
             { 1.0f,  1.0f, },
             { 0.0f,  1.0f, },
			 
             { 0.0f,  0.0f, },
             { 1.0f,  0.0f, },
             { 1.0f,  1.0f, },
             { 0.0f,  1.0f, },
		};

        Mesh mesh = Mesh{
            indices,
            vertices,
        };

        return mesh;
    }
}
