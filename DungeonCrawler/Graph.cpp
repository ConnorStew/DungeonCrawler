#include "stdafx.h"
#include "Graph.h"
#include <iostream>

Graph::Graph(int size) {
	//nodes.resize(size);
	adjacencyList.resize(size);
}

void Graph::addVertex(float x, float y) {
	Node* node = new Node(x, y, lastIndexAssigned);
	Key key = Key(x, y);
	std::pair<Key, Node*> pair = std::make_pair(key, node);
	nodes.insert(pair);
	lastIndexAssigned++;
}

void Graph::addConnection(Node* n1, Node* n2) {
	if (n1 == nullptr || n2 == nullptr)
		return;

	adjacencyList.at(n1->getIndex()).push_back(new Connection(n1, n2, DEFAULT_WEIGHT));
}

Node* Graph::getNode(int x, int y) {
	std::map<Key, Node*>::iterator it = nodes.find(Key(x, y));
		if (it != nodes.end())
			return it->second;

	return nullptr;
}
