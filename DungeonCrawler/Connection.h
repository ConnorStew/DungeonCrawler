#pragma once
#ifndef CONNECTION_H
#define CONNECTION_H

#include "Node.h"

/// <summary> This class represents a connection between two nodes. </summary>
class Connection {
private:

	/// <summary> The default weight assigned to new connections in the graph. </summary>
	const int DEFAULT_WEIGHT = 0;

	/// <summary> The ending node of the connection. </summary>
	Node* connectedNode;

	/// <summary> The weight of the connection. </summary>
	float weight;
public:
	/// <summary> Creates a connection between two nodes. </summary>
	/// <param name="connectedNode">The first node in the connection.</param>
	/// <param name="weight">The weight of the connection.</param>
	Connection(Node* connectedNode, float weight);

	Connection(Node* connectedNode);

	Node* getConnectedNode();
};

#endif