#include "stdafx.h"
#include "Graph.h"
#include <chrono>
#include <thread>
/*
Graph::Graph(int size) {
	adjacencyList.resize(size);
}

void Graph::addNode(std::shared_ptr<Node> node) {
	node->setIndex(lastIndexAssigned);
	lastIndexAssigned++;
	std::pair<int, int> std::pair<int, int>(node->getX(), node->getY());
	std::pair<std::pair<int, int>, std::shared_ptr<Node>> pair = std::make_pair(std::pair<int, int>, node);
	nodes.insert(pair);
}

void Graph::addConnection(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
	adjacencyList.at(n1->getIndex()).push_back(std::make_shared<Connection>(n2));
	adjacencyList.at(n2->getIndex()).push_back(std::make_shared<Connection>(n1));
}

std::shared_ptr<Node> Graph::getNode(int x, int y) {
	std::map<std::pair<int, int>, std::shared_ptr<Node>>::iterator it = nodes.find(std::pair<int, int>(x, y));
	if (it != nodes.end())
		return it->second;
}

std::vector<std::shared_ptr<Connection>> Graph::getConnections(std::shared_ptr<Node> node) {
	return adjacencyList.at(node->getIndex());
}

void Graph::aStar() {
	openList.clear();
	closedList.clear();
	path.clear();
	openList.push_back(startNode);

	while (openList.size() != 0) {
		std::cout << "in open loop" << std::endl;
		if (std::find(openList.begin(), openList.end(), endNode) != openList.end()) {
			path.push_back(endNode);

			std::shared_ptr<Node> current = endNode;
			while (current->getParent() != nullptr) {
				path.push_back(current->getParent());
				current = current->getParent();
			}

			std::cout << "in loop" << std::endl;
			return;
		}

		//get lowest nodes
		std::vector<std::shared_ptr<Node>> lowestVertices = std::vector<std::shared_ptr<Node>>();
		float lowestScore = std::numeric_limits<float>::max();

		for (std::shared_ptr<Node> node : openList)
			if (node->getF() < lowestScore)
				lowestScore = node->getF();

		for (std::shared_ptr<Node> node : openList)
			if (node->getF() == lowestScore)
				lowestVertices.push_back(node);

		for (std::shared_ptr<Node> node : lowestVertices) {
			closedList.push_back(node);
			openList.erase(std::remove(openList.begin(), openList.end(), node), openList.end());

			for (std::shared_ptr<Connection> connection : getConnections(node)) {
				std::shared_ptr<Node> neighbour = connection->getConnectedNode();
				if (!(std::find(openList.begin(), openList.end(), neighbour) != openList.end()) && !(std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())) {
					if (std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())
						continue;

					float tentativeG = node->getG() + std::abs(node->getX() - neighbour->getX()) + std::abs(node->getY() - neighbour->getY());
					float tentativeH = std::abs(endNode->getX() - neighbour->getX()) + std::abs(endNode->getY() - neighbour->getY());
					float tentativeScore = (tentativeG + tentativeH) * connection->getWeight();

					if (!(std::find(openList.begin(), openList.end(), neighbour) != openList.end())) {
						openList.push_back(neighbour);
						node->updateScore(neighbour, tentativeScore, tentativeG, tentativeH);
					}

					//update the score of the neighbouring vertex
					if (tentativeScore < neighbour->getF()) {
						node->updateScore(neighbour, tentativeScore, tentativeG, tentativeH);
					}
				}
			}
		}

		//std::this_thread::sleep_for(std::chrono::seconds(wait));
	}
	
}

void Graph::clear() {
	openList.clear();
	closedList.clear();
	path.clear();
}

const std::vector<std::shared_ptr<Node>> Graph::getOpenList() {
	return openList;
}

const std::vector<std::shared_ptr<Node>> Graph::getClosedList() {
	return closedList;
}

const std::vector<std::shared_ptr<Node>> Graph::getPath() {
	return path;
}

const std::shared_ptr<Node> Graph::getStartNode() {
	return startNode;
}

const std::shared_ptr<Node> Graph::getEndNode() {
	return endNode;
}

void Graph::setStartNode(std::shared_ptr<Node> start) {
	startNode = start;
}

void Graph::setEndNode(std::shared_ptr<Node> end) {
	endNode = end;
}

void Graph::addToClosedList(std::shared_ptr<Node> node) {
	closedList.push_back(node);
}

void Graph::addToOpenList(std::shared_ptr<Node> node) {
	openList.push_back(node);
}

void Graph::addToPathList(std::shared_ptr<Node> node) {
	path.push_back(node);
}

const std::map<std::pair<int, int>, std::shared_ptr<Node>> Graph::getNodes() {
	return nodes;
}

*/