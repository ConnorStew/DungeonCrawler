#include "stdafx.h"
#include "Node.h"

Node::Node(int gridX, int gridY, bool filled) {
	this->filled = filled;
	this->gridX = gridX;
	this->gridY = gridY;
	this->g = 0;
	this->h = 0;
	this->f = std::numeric_limits<float>::max();
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

int Node::getGridX() {
	return gridX;
}

int Node::getGridY() {
	return gridY;
}

int Node::getParentX() {
	return parentX;
}

int Node::getParentY() {
	return parentY;
}

void Node::setFilled(bool set)
{
	filled = set;
}

void Node::updateScore(int parentX, int parentY, float f, float g, float h) {
	this->f = f;
	this->g = g;
	this->h = h;
	this->parentX = parentX;
	this->parentY = parentY;
}

void Node::setParent(int parentX, int parentY) {
	this->parentX = parentX;
	this->parentY = parentY;
}

bool Node::getFilled() {
	return filled;
}