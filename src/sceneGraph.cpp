#include "sceneGraph.hpp"
#include <iostream>

SceneNode* createSceneNode() {
	return new SceneNode();
}

// Add a child node to its parent's list of children
void addChild(SceneNode* parent, SceneNode* child) {
	parent->children.push_back(child);
}

int totalChildren(SceneNode* parent) {
	int count = parent->children.size();
	for (SceneNode* child : parent->children) {
		count += totalChildren(child);
	}
	return count;
}

// Pretty prints the current values of a SceneNode instance to stdout
void printNode(SceneNode* node) {
	printf(
		"SceneNode {\n"
		"    Child count: %i\n"
		"    Rotation: (%f, %f, %f)\n"
		"    Location: (%f, %f, %f)\n"
		"    Reference point: (%f, %f, %f)\n"
		"    VAO ID: %i\n"
		"}\n",
		int(node->children.size()),
		node->rotation.x, node->rotation.y, node->rotation.z,
		node->position.x, node->position.y, node->position.z,
		node->referencePoint.x, node->referencePoint.y, node->referencePoint.z, 
		node->vertexArrayObjectID);
}

