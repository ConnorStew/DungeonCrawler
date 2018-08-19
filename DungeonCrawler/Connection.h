#pragma once
#ifndef CONNECTION_H
#define CONNECTION_H

#include "Node.h"

/// <summary> This class represents a connection between two nodes. </summary>
class Connection {
private:
	/// <summary> The starting node of the connection. </summary>
	Node* startNode;

	/// <summary> The ending node of the connection. </summary>
	Node* endNode;

	/// <summary> The weight of the connection. </summary>
	float weight;
public:
	/// <summary> Creates a connection between two nodes. </summary>
	/// <param name="startNode">The first node in the connection.</param>
	/// <param name="endNode">The second node in the connection.</param>
	/// <param name="weight">The weight of the connection.</param>
	Connection(Node* startNode, Node* endNode, float weight);
};

#endif