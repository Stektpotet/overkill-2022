#include <iostream>
#include "glfont.h"

Mesh generateTextGeometryBuffer(std::string text, float characterHeightOverWidth, float totalTextWidth) {
    float characterWidth = totalTextWidth / float(text.length());
    float characterHeight = characterHeightOverWidth * characterWidth;

    float stepSize = 1.0f / 128;

    unsigned int vertexCount = 4 * text.length();
    unsigned int indexCount = 6 * text.length();

    Mesh mesh;

    mesh.vertices.resize(vertexCount);
    mesh.normals.resize(vertexCount);
    mesh.textureCoordinates.resize(vertexCount);
    mesh.indices.resize(indexCount);
    
    char offset = 'a';
    for(unsigned int i = 0; i < text.length(); i++)
    {
        float baseXCoordinate = float(i) * characterWidth;
        float baseU = text[i] * stepSize;

        mesh.vertices.at(4 * i + 0) = {baseXCoordinate, 0, 0};
        mesh.vertices.at(4 * i + 1) = {baseXCoordinate + characterWidth, 0, 0};
        mesh.vertices.at(4 * i + 2) = {baseXCoordinate + characterWidth, characterHeight, 0};

        mesh.vertices.at(4 * i + 0) = {baseXCoordinate, 0, 0};
        mesh.vertices.at(4 * i + 2) = {baseXCoordinate + characterWidth, characterHeight, 0};
        mesh.vertices.at(4 * i + 3) = {baseXCoordinate, characterHeight, 0};

        mesh.textureCoordinates.at(4 * i + 0) = { baseU, 0 };
        mesh.textureCoordinates.at(4 * i + 1) = { baseU + stepSize, 0 };
        mesh.textureCoordinates.at(4 * i + 2) = { baseU + stepSize, 1 };
        mesh.textureCoordinates.at(4 * i + 3) = { baseU, 1 };

        mesh.indices.at(6 * i + 0) = 4 * i + 0;
        mesh.indices.at(6 * i + 1) = 4 * i + 1;
        mesh.indices.at(6 * i + 2) = 4 * i + 2;
        mesh.indices.at(6 * i + 3) = 4 * i + 0;
        mesh.indices.at(6 * i + 4) = 4 * i + 2;
        mesh.indices.at(6 * i + 5) = 4 * i + 3;
    }
    std::fill(mesh.normals.begin(), mesh.normals.end(), glm::vec3{ 0, 0, -1 });

    return mesh;
}