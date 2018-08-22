#include "stdafx.h"
#include "Graph.h"

Graph::Graph(int size) {
	adjacencyList.resize(size);
}

void Graph::addNode(Node* node) {
	node->setIndex(lastIndexAssigned);
	lastIndexAssigned++;
	Key key = Key(node->getX(), node->getY());
	std::pair<Key, Node*> pair = std::make_pair(key, node);
	nodes.insert(pair);
}

void Graph::addConnection(Node* n1, Node* n2) {
	adjacencyList.at(n1->getIndex()).push_back(new Connection(n2));
	adjacencyList.at(n2->getIndex()).push_back(new Connection(n1));
}

Node* Graph::getNode(int x, int y) {
	std::map<Key, Node*>::iterator it = nodes.find(Key(x, y));
		if (it != nodes.end())
			return it->second;

	return nullptr;
}

std::vector<Connection*> Graph::getConnections(Node* node) {
	return adjacencyList.at(node->getIndex());
}

void Graph::aStar(Node * start, Node * end){
	
}

const std::vector<Node*> Graph::getOpenList() {
	return openList;
}

const std::vector<Node*> Graph::getClosedList() {
	return closedList;
}

const std::vector<Node*> Graph::getPath() {
	return path;
}

void Graph::addToClosedList(Node * node) {
	closedList.push_back(node);
}

void Graph::addToOpenList(Node * node) {
	openList.push_back(node);
}

void Graph::addToPathList(Node * node) {
	path.push_back(node);
}

const std::map<Key, Node*> Graph::getNodes() {
	return nodes;
}

