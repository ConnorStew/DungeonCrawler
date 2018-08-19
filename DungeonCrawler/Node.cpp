#include "stdafx.h"
#include "Node.h"

Node::Node(int x, int y) {
	this->x = x;
	this->y = y;
}

int Node::getIndex() {
	return index;
}

int Node::getX() {
	return x;
}

int Node::getY() {
	return y;
}


void Node::printDebug() {
	std::cout << "x: " << std::to_string(x) << ", y: " << std::to_string(y) << std::endl;
}
