#include "stdafx.h"
#include "Connection.h"

Connection::Connection(Node* connectedNode, float weight) {
	this->connectedNode = connectedNode;
	this->weight = weight;
}

Connection::Connection(Node* connectedNode) {
	this->connectedNode = connectedNode;
	this->weight = Connection::DEFAULT_WEIGHT;
}

Node* Connection::getConnectedNode() {
	return connectedNode;
}