#include "stdafx.h"
#include "Connection.h"

Connection::Connection(Node* baseVertex, Node* connectedVertex, float weight) {
	this->startNode = baseVertex;
	this->endNode = connectedVertex;
	this->weight = weight;
}
