#include "stdafx.h"
#include "Node.h"
#include <iostream>
#include <String>

Node::Node(float x, float y, int index) {
	this->x = x;
	this->y = y;
	this->index = index;
}

int Node::getIndex() {
	return index;
}

void Node::printDebug() {
	std::cout << "Index: " << std::to_string(index) << ", x: " << std::to_string(x) << ", y: " << std::to_string(y) << std::endl;
}
