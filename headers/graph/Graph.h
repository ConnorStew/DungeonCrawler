#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include "stdafx.h"
#include "node.h"

using std::shared_ptr;
using std::vector;
using std::pair;
using std::map;

/// <summary> A graph data structure used to map the connectiosn between nodes. </summary> 
template <class T>
class Graph {
	static_assert(std::is_base_of<Node, T>::value, "Class is not derived from Node.");

private:
	/// <summary> A list of nodes still being considered for the path. </summary>
	vector<shared_ptr<T>> openList;

	/// <summary> A list of nodes not being considered for the path. </summary>
	vector<shared_ptr<T>> closedList;

	/// <summary> A list of nodes on the path. </summary>
	vector<shared_ptr<T>> path;

	/// <summary> The nodes stored in the graph. </summary>
	map<pair<int, int>, shared_ptr<T>> nodes;

	map<pair<int, int>, vector<shared_ptr<T>>> adjList;

	class YDeref {
		private:
			int xIndex;
			int yIndex;
			Graph *graph;
		public:
			YDeref(Graph *graph, int xIndex, int yIndex) {
				this->xIndex = xIndex;
				this->yIndex = yIndex;
				this->graph = graph;
			}
			operator shared_ptr<T>() {
				typename std::map<pair<int, int>, shared_ptr<T>>::iterator it;
				it = graph->nodes.find(std::make_pair(xIndex, yIndex));
				if (it != graph->nodes.end())
					return it->second;
				else
					return nullptr;
			}

			bool operator==(T* rhs) {
				if (graph->nodes[std::make_pair(xIndex, yIndex)] == nullptr && rhs == nullptr)
					return true;

				if (std::make_pair(xIndex, yIndex) == std::make_pair(rhs->getGridX(), rhs->getGridY())) {
					return true;
				}

				return false;
			}

			bool operator==(YDeref* rhs) {
				if (graph->nodes[std::make_pair(xIndex, yIndex)] == nullptr && rhs == nullptr)
					return true;

				if (std::make_pair(xIndex, yIndex) == std::make_pair(rhs->getGridX(), rhs->getGridY())) {
					return true;
				}

				return (xIndex == rhs->xIndex) && (yIndex == rhs->yIndex);
			}


			/// <summary>
			/// Allows access to the nodes functions.
			/// </summary>
			/// <returns>The node contains at the given poistion.</returns>
			shared_ptr<T> operator->() {

				typename std::map<pair<int, int>, shared_ptr<T>>::iterator it;
				it = graph->nodes.find(std::make_pair(xIndex, yIndex));
				if (it != graph->nodes.end())
					return it->second;
				else
					return nullptr;
			}

			/// <summary>
			/// Insert a node into the graph.
			/// </summary>
			/// <param name="other">The node to insert. </param>
			void operator=(shared_ptr<T> other) {
				graph->nodes.insert(std::make_pair(std::make_pair(xIndex,yIndex), other));
			}
	};

	class XDeref {
		private:
			int xIndex;
			Graph *graph;
		public:
			XDeref(Graph *graph, int xIndex) {
				this->xIndex = xIndex;
				this->graph = graph;
			}

			YDeref operator[](const int index) {
				return YDeref(graph, xIndex, index);
			}
	};

	/// <summary>
	/// Connects tiles to the tile at x,y to the tile at x + xIncrease, y + yIncrease.
	/// </summary>
	/// <param name="x">X coordinate of node1.</param>
	/// <param name="y">Y coordinate of node1.</param>
	/// <param name="xIncrease">X distance of node2 away from node1.</param>
	/// <param name="yIncrease">Y distance of node2 away from node1.</param>
	void connectIfValid(int x, int y, int xIncrease, int yIncrease) {
		if (getNode(x,y) != nullptr && getNode(x + xIncrease, y + yIncrease) != nullptr) {
			if (!getNode(x,y)->getFilled() && !getNode(x + xIncrease,y + yIncrease)->getFilled()) {
				adj(x, y).push_back(getNode(x + xIncrease,y + yIncrease));
				adj(x + xIncrease, y + yIncrease).push_back(getNode(x,y));
			}
		}
	}

	/// <summary>
	/// Easy method to find nodes within the graph header.
	/// </summary>
	/// <param name="x">X position.</param>
	/// <param name="y">Y position.</param>
	/// <returns></returns>
	shared_ptr<T> getNode(int x, int y) {
		pair<int, int> myPair = std::make_pair(x,y);
		typename std::map<pair<int, int>, shared_ptr<T>>::iterator it;
		it = nodes.find(myPair);
		if (it != nodes.end())
			return it->second;
		else
			return nullptr;
	}

public:

	Graph() {
		std::cout << "here" << std::endl;
	}

	void addNode(int x, int y, shared_ptr<T> node) {
		nodes.insert(std::make_pair(std::make_pair(x,y), node));
	}

	/// <summary> Clears the graph. </summary>
	void clear() {
		openList.clear();
		closedList.clear();
		path.clear();
	}

	/// <summary>
	/// Adds connections for the given tile.
	/// </summary>
	/// <param name="tile">The tile to add connections to.</param>
	void addConnections(int x, int y, bool diagonal) {
		connectIfValid(x, y, 1, 0); //right
		connectIfValid(x, y, -1, 0); //left
		connectIfValid(x, y, 0, 1); //up
		connectIfValid(x, y, 0, -1); //down

		if (diagonal) {
			connectIfValid(x, y, 1, -1); //bottom right
			connectIfValid(x, y, 1, 1); //top right
			connectIfValid(x, y, -1, -1); //bottom left
			connectIfValid(x, y, -1, 1); //top left
		}
	}

	/// <summary>
	/// Clears the connections which involve the given tile.
	/// </summary>
	/// <param name="tile">The tile to remove the connections of.</param>
	void clearConnections(int x, int y) {
		shared_ptr<T> node = nullptr;
		auto it = nodes.find(std::make_pair(x, y));
		if (it != nodes.end())
			node = it->second;

		vector<shared_ptr<T>>& connectedTiles = adj(x,y);

		for (unsigned int i = 0; i < connectedTiles.size(); i++) {
			shared_ptr<T>& connectedTile = connectedTiles.at(i);
			//std::cout << *connectedTile << std::endl; //dereference pointer

			//remove the original tile from the connected tiles connections
			vector<shared_ptr<T>>& adjList = adj(connectedTile->getGridX(), connectedTile->getGridY());
			auto it = std::find(adjList.begin(), adjList.end(), node);
			if (it != adjList.end()) {
				adjList.erase(it);
			}
		}

		connectedTiles.clear();
	}

	/// <summary>
	/// Allows the access of the nodes through [x][y] syntax.
	/// </summary>
	/// <param name="index">The x index of the node.</param>
	/// <returns>An XDeref class which allows you to return the node at the y index.</returns>
	XDeref operator[](const int index) {
		return XDeref(this, index); //pass the x array
	}

	/// <summary>
	/// Allows you to access a list of adjacent nodes from the node at the x,y index.
	/// </summary>
	/// <param name="x">The nodes grid x coordinate.</param>
	/// <param name="y">The nodes grid y coordinate.</param>
	/// <returns>A list of nodes adjacent to the given grid position.</returns>
	vector<shared_ptr<T>>& adj(int x, int y) {
		return adjList[std::make_pair(x,y)];
	}

	/// <summary>
	/// Checks if a node is in the open list.
	/// </summary>
	/// <param name="node">The node to check.</param>
	/// <returns>Whether the node is in the list.</returns>
	bool inOpenList(shared_ptr<T> node) {
		return std::find(openList.begin(), openList.end(), node) != openList.end();
	}

	/// <summary>
	/// Checks if a node is in the closed list.
	/// </summary>
	/// <param name="node">The node to check.</param>
	/// <returns>Whether the node is in the list.</returns>
	bool inClosedList(shared_ptr<T> node) {
		return std::find(closedList.begin(), closedList.end(), node) != closedList.end();
	}

	const vector<shared_ptr<T>>& getOpenList() {
		return openList;
	}

	const vector<shared_ptr<T>>& getClosedList() {
		return closedList;
	}

	const vector<shared_ptr<T>>& getPathList() {
		return path;
	}

	/// <summary>
	/// Checks if a node is in the path list.
	/// </summary>
	/// <param name="node">The node to check.</param>
	/// <returns>Whether the node is in the list.</returns>
	bool inPathList(shared_ptr<T> node) {
		return std::find(path.begin(), path.end(), node) != path.end();
	}

	map<pair<int, int>, shared_ptr<T>> getNodes() {
		return nodes;
	}
	
	shared_ptr<T> at(int x, int y) {
		return getNode(x,y);
	}

	/// <summary>
	/// Runs the A* algorithm to get a path from the start to end node.
	/// </summary>
	/// <param name="start">Starting node.</param>
	/// <param name="end">Target node.</param>
	vector<shared_ptr<T>> aStar(int x1, int y1, int x2, int y2) {

		shared_ptr<T> start = getNode(x1, y1);
		shared_ptr<T> end = getNode(x2, y2);

		if (start == nullptr || end == nullptr) {
			std::cout << "Error: Tiles given are not on graph..." << std::endl;
			return vector<shared_ptr<T>>();
		}

		clear();
		openList.push_back(start);

		while (!openList.empty()) {

			if (inOpenList(end)) {
				shared_ptr<T> &current = end;

				while (current != start) {
					path.push_back(current);
					current = nodes[std::make_pair(current->getParentX(),current->getParentY())];
				}

				return path;
			}

			//get lowest nodes
			vector<shared_ptr<T>> lowestVertices = vector<shared_ptr<T>>();
			float lowestScore = std::numeric_limits<float>::max();

			for (shared_ptr<T> node : openList)
				if (node->getF() < lowestScore)
					lowestScore = node->getF();

			for (shared_ptr<T> node : openList)
				if (node->getF() == lowestScore)
					lowestVertices.push_back(node);

			for (shared_ptr<T> node : lowestVertices) {
				//std::lock_guard<std::mutex> lock(mutex);

				closedList.push_back(node);
				openList.erase(std::remove(openList.begin(), openList.end(), node), openList.end());

				for (shared_ptr<T> neighbour : adjList[std::make_pair(node->getGridX(),node->getGridY())]) {
					if (!inClosedList(neighbour)) {

						//the tile distance away
						float tentativeG = node->getG() + std::abs(node->getGridX() - neighbour->getGridX()) + std::abs(node->getGridY() - neighbour->getGridY());

						//hueristic distance the world distance away in pixels
						float tentativeH = std::abs(end->getWorldX() - neighbour->getWorldX()) + std::abs(end->getWorldY() - neighbour->getWorldY());

						//total the distance metrics
						float tentativeScore = (tentativeG + tentativeH);

						//add to open list and update score if not in open list
						if (!inOpenList(neighbour)) {
							openList.insert(openList.begin(), neighbour);
							neighbour->updateScore(node->getGridX(), node->getGridY(), tentativeScore, tentativeG, tentativeH);
						}

						//update the score of the neighbouring vertex
						if (tentativeScore < neighbour->getF()) {
							neighbour->updateScore(node->getGridX(), node->getGridY(), tentativeScore, tentativeG, tentativeH);
						}
					}
				}
			}
		}

		return path;
	}
};

#endif