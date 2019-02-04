#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stack>
#include <vector>
#include <cstdio>
#include <stdbool.h>
#include <cstdlib> 
#include <ctime> 
#include <chrono>
#include <fstream>

enum SceneNodeType {
	GEOMETRY, POINT_LIGHT, SPOT_LIGHT
};

// In case you haven't got much experience with C or C++, let me explain this "typedef" you see below.
// The point of a typedef is that you it, as its name implies, allows you to define arbitrary data types based upon existing ones. For instance, "typedef float typeWhichMightBeAFloat;" allows you to define a variable such as this one: "typeWhichMightBeAFloat variableName = 5.0;". The C/C++ compiler translates this type into a float. 
// What is the point of using it here? A smrt person, while designing the C language, thought it would be a good idea for various reasons to force you to explicitly state that you are using a data structure datatype (struct). So, when defining a variable, you'd have to type "struct SceneNode node = ..." in the case of a SceneNode. Which can get in the way of readability.
// If we just use typedef to define a new type called "SceneNode", which really is the type "struct SceneNode", we can omit the "struct" part when creating an instance of SceneNode. 
struct SceneNode {
	SceneNode() {
		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);

        referencePoint = glm::vec3(0, 0, 0);
        vertexArrayObjectID = -1;
        VAOIndexCount = 0;

        nodeType = GEOMETRY;
	}

	// A list of all children that belong to this node.
	// For instance, in case of the scene graph of a human body shown in the assignment text, the "Upper Torso" node would contain the "Left Arm", "Right Arm", "Head" and "Lower Torso" nodes in its list of children.
	std::vector<SceneNode*> children;
	
	// The node's position and rotation relative to its parent
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	// A transformation matrix representing the transformation of the node's location relative to its parent. This matrix is updated every frame.
	glm::mat4 currentTransformationMatrix;

	// The location of the node's reference point
	glm::vec3 referencePoint;

	// The ID of the VAO containing the "appearance" of this SceneNode.
	int vertexArrayObjectID;
	unsigned int VAOIndexCount;

	// Node type is used to determine how to handle the contents of a node
	SceneNodeType nodeType;
};

// Struct for keeping track of 2D coordinates

SceneNode* createSceneNode();
void addChild(SceneNode* parent, SceneNode* child);
void printNode(SceneNode* node);


// For more details, see SceneGraph.cpp.