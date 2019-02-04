#include <glad/glad.h>
#include <program.hpp>
#include "glutils.h"

unsigned int generateBuffer(Mesh &mesh) {
    unsigned int vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    unsigned int vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(glm::vec3), mesh.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    unsigned int normalBufferID;
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(glm::vec3), mesh.normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    unsigned int indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);


    return vaoID;
}
