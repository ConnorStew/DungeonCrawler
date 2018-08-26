#include "stdafx.h"
#include "Node.h"

Node::Node(std::pair<int, int> location, int index) {
	this->location = location;
	this->g = 0;
	this->h = 0;
	this->f = std::numeric_limits<float>::max();
	this->index = index;
}

int Node::getX() {
	return location.first;
}

int Node::getY() {
	return location.second;
}

float Node::getG() {
	return g;
}

float Node::getH() {
	return h;
}

float Node::getF() {
	return f;
}

int Node::getIndex() {
	return index;
}

std::pair<int, int> Node::getLocation() {
	return location;
}

void Node::updateScore(std::shared_ptr<Node> parent, float f, float g, float h) {
	this->f = f;
	this->g = g;
	this->h = h;
	this->parent = parent;
}

void Node::setParent(std::shared_ptr<Node> parent) {
	this->parent = parent;
}

std::shared_ptr<Node> Node::getParent() {
	return parent;
}
