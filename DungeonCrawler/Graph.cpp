#include "stdafx.h"
#include "Graph.h"

Graph::Graph(int size) {
	//nodes.resize(size);
	adjacencyList.resize(size);
}

void Graph::addNode(Node* node) {
	Key key = Key(node->getX(), node->getY());
	std::pair<Key, Node*> pair = std::make_pair(key, node);
	nodes.insert(pair);
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
