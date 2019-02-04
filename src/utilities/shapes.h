#pragma once
#include "mesh.h"

Mesh generateBox(float width, float height, float depth, bool flipFaces = false);
Mesh generateSphere(float radius, int slices, int layers);