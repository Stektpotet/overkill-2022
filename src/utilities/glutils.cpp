#include <glad/glad.h>
#include <program.hpp>
#include "glutils.h"
#include <vector>
#include <cassert>

std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>>
computeTB(const Mesh& mesh)
{
    assert(mesh.textureCoordinates.size() > 0 && "Mesh needs to have UV coordinates to sensibly generate tangents and bitangents!");
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    tangents.reserve(mesh.normals.size());
    bitangents.reserve(mesh.normals.size());

    for (size_t i = 0; i < mesh.vertices.size(); i += 3)
    {
        glm::vec3 d1 = mesh.vertices[i + 1] - mesh.vertices[i + 0];
        glm::vec3 d2 = mesh.vertices[i + 2] - mesh.vertices[i + 0];

        glm::vec2 dUV1 = mesh.textureCoordinates[i + 1] - mesh.textureCoordinates[i + 0];
        glm::vec2 dUV2 = mesh.textureCoordinates[i + 2] - mesh.textureCoordinates[i + 0];

        float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);
        glm::vec3 tangent = (d1 * dUV2.y - d2 * dUV1.y) * r;
        glm::vec3 bitangent = (d2 * dUV1.x - d1 * dUV2.x) * r;
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }
    return { tangents, bitangents };
}

template <class T>
unsigned int generateAttribute(int id, int elementsPerEntry, std::vector<T> data, bool normalize) {
    unsigned int bufferID;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(id, elementsPerEntry, GL_FLOAT, normalize ? GL_TRUE : GL_FALSE, sizeof(T), 0);
    glEnableVertexAttribArray(id);
    return bufferID;
}

unsigned int generateBuffer(Mesh &mesh) {
    unsigned int vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    generateAttribute(0, 3, mesh.vertices, false);
    generateAttribute(1, 3, mesh.normals, true);
    if (mesh.textureCoordinates.size() > 0) {
        auto TB = computeTB(mesh);

        generateAttribute(2, 2, mesh.textureCoordinates, false);
        generateAttribute(3, 3, TB.first, true);
        generateAttribute(4, 3, TB.second, true);
    }

    unsigned int indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

    return vaoID;
}
