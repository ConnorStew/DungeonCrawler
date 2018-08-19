#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include "stdafx.h"
#include "Node.h"
#include "Connection.h"
#include "Key.h"

/// <summary> A graph data structure used to map the connectiosn between nodes. </summary>
class Graph {
private:
	/// <summary> The last index assigned to a node. </summary>
	int lastIndexAssigned = 0;
	/// <summary> List of nodes in the graph. </summary>
	std::map<Key, Node*> nodes;
	/// <summary> List of nodes which are still being considered for the path. </summary>
	std::vector<Node*> openList;
	/// <summary> List of nodes which are not being considered for the path. </summary>
	std::vector<Node*> closedList;
	/// <summary> List of nodes which create the shortest path. </summary>
	std::vector<Node*> path;
	/// <summary>  Vector of lists containing connection between nodes. </summary>
	std::vector<std::vector<Connection*>> adjacencyList;
public:
	/// <summary> Creates a new graph. </summary>
	/// <param name="size">The amount of elements to store in the graph.</param>
	Graph(int size);

	/// <summary> Adds a new index to the graph. </summary>
	void addNode(Node * node);

	/// <summary> Adds a connection between two nodes. </summary>
	/// <param name="n1">the first node</param>
	/// <param name="n2">the second node</param>
	void addConnection(Node* n1, Node* n2);

	Node* getNode(int x, int y);
	std::vector<Connection*> getConnections(Node* node);
};

#endif