#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include "stdafx.h"
#include "Node.h"

/*
/// <summary> A graph data structure used to map the connectiosn between nodes. </summary>
class Graph {
private:
	/// <summary> The last index assigned to a node. </summary>
	int lastIndexAssigned = 0;
	/// <summary> List of nodes in the graph. </summary>
	std::map<std::pair<int, int>, std::shared_ptr<Node>> nodes;
	/// <summary> List of nodes which are still being considered for the path. </summary>
	std::vector<std::shared_ptr<Node>> openList;
	/// <summary> List of nodes which are not being considered for the path. </summary>
	std::vector<std::shared_ptr<Node>> closedList;
	/// <summary> List of nodes which create the shortest path. </summary>
	std::vector<std::shared_ptr<Node>> path;
	/// <summary>  Vector of lists containing connection between nodes. </summary>
	std::vector<std::vector<std::shared_ptr<Connection>>> adjacencyList;

	std::shared_ptr<Node> startNode;
	std::shared_ptr<Node> endNode;
public:
	/// <summary> Creates a new graph. </summary>
	/// <param name="size">The amount of elements to store in the graph.</param>
	Graph(int size);

	/// <summary> Adds a new index to the graph. </summary>
	void addNode(std::shared_ptr<Node> node);

	/// <summary> Adds a connection between two nodes. </summary>
	/// <param name="n1">the first node</param>
	/// <param name="n2">the second node</param>
	void addConnection(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2);

	std::shared_ptr<Node> getNode(int x, int y);
	
	std::vector<std::shared_ptr<Connection>> getConnections(std::shared_ptr<Node> node);

	void aStar();

	void clear();

	const std::map<std::pair<int, int>, std::shared_ptr<Node>> getNodes();
	const std::vector<std::shared_ptr<Node>> getOpenList();
	const std::vector<std::shared_ptr<Node>> getClosedList();
	const std::vector<std::shared_ptr<Node>> getPath();
	const std::shared_ptr<Node> getStartNode();
	const std::shared_ptr<Node> getEndNode();
	void setStartNode(std::shared_ptr<Node> node);
	void setEndNode(std::shared_ptr<Node> node);

	void addToClosedList(std::shared_ptr<Node> node);
	void addToOpenList(std::shared_ptr<Node> node);
	void addToPathList(std::shared_ptr<Node> node);
};
*/


#endif