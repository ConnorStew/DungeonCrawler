#include "stdafx.h"
#include "Node.h"

Node::Node(std::pair<int, int> gridLoc, bool filled) {
	this->filled = filled;
	this->gridLocation = gridLoc;
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

std::pair<int, int> Node::getGridLoc() {
	return gridLocation;
}

int Node::gridX()
{
	return gridLocation.first;
}

int Node::gridY()
{
	return gridLocation.second;
}

void Node::setFilled(bool set)
{
	filled = set;
}

void Node::updateScore(std::pair<int, int> parentLoc, float f, float g, float h) {
	this->f = f;
	this->g = g;
	this->h = h;
	this->parentLoc = parentLoc;
}

void Node::setParent(std::pair<int, int> parentLoc) {
	this->parentLoc = parentLoc;
}

std::pair<int, int> Node::getParent() {
	return parentLoc;
}

bool Node::getFilled() {
	return filled;
}
