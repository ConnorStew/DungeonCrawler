#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include "stdafx.h"

/// <summary> A graph data structure used to map the connectiosn between nodes. </summary> 
template <class T, int size>
class Graph {
	private:
		std::vector<std::shared_ptr<T>> openList;
		std::vector<std::shared_ptr<T>> closedList;
		std::vector<std::shared_ptr<T>> path;
		std::vector<std::shared_ptr<T>> adjList[size][size];
		std::shared_ptr<T> nodes[size][size];
	public:

		class RightDeref {
			private:
				int leftIndex;
				int rightIndex;
				Graph *graph;

			public:

				RightDeref(Graph *graph, int leftIndex, int rightIndex) {
					this->leftIndex = leftIndex;
					this->rightIndex = rightIndex;
					this->graph = graph;
				}
			
				operator std::shared_ptr<T>() {
					return graph->nodes[leftIndex][rightIndex];
				}

				std::shared_ptr<T> operator->() const
				{
					return graph->nodes[leftIndex][rightIndex];
				}
			
				std::shared_ptr<T> operator=(const std::shared_ptr<T>& other) {
					return this->graph->nodes[leftIndex][rightIndex] = other;
				}
		};

		class LeftDeref {
			private:
				int index;
				Graph *graph;

			public:

				LeftDeref(Graph *graph, int index) {
					this->index = index;
					this->graph = graph;
				}

				//the first [] operator returns a left return object
				RightDeref operator[](const int index) {
					return RightDeref(graph, this->index, index); //pass the x array
				}
		};

		void clear() {
			openList.clear();
			closedList.clear();
			path.clear();
		}

		//the first [] operator returns a left return object
		LeftDeref operator[](const int index) {
			return LeftDeref(this, index); //pass the x array
		}

		
		std::shared_ptr<T> operator()(int x, int y) {
			return graph->nodes[x][y];
		}
		
		std::vector<std::shared_ptr<T>> * adj(int x, int y) {
			return &adjList[x][y];
		}

		bool inOpenList(std::shared_ptr<T> node) {
			return std::find(openList.begin(), openList.end(), node) != openList.end();
		}

		bool inClosedList(std::shared_ptr<T> node) {
			return std::find(closedList.begin(), closedList.end(), node) != closedList.end();
		}

		bool inPathList(std::shared_ptr<T> node) {
			return std::find(path.begin(), path.end(), node) != path.end();
		}

		void aStar(std::shared_ptr<T> start, std::shared_ptr<T> end) {
			clear();

			openList.push_back(start);

			while (!openList.empty()) {

				if (std::find(openList.begin(), openList.end(), end) != openList.end()) {

					std::shared_ptr<T> &current = end;
					path.push_back(current);

					do {
						std::this_thread::sleep_for(std::chrono::milliseconds(2));
						std::lock_guard<std::mutex> lock(mutex);
						current = nodes[current->getParentX()][current->getParentY()];
						path.push_back(current);
					} while (current != start);


					std::cout << "found goal node" << std::endl;
					return;
				}

				//get lowest nodes
				std::vector<std::shared_ptr<T>> lowestVertices = std::vector<std::shared_ptr<T>>();
				float lowestScore = std::numeric_limits<float>::max();

				for (std::shared_ptr<T> node : openList)
					if (node->getF() < lowestScore)
						lowestScore = node->getF();

				for (std::shared_ptr<T> node : openList)
					if (node->getF() == lowestScore)
						lowestVertices.push_back(node);

				for (std::shared_ptr<T> node : lowestVertices) {
					std::this_thread::sleep_for(std::chrono::milliseconds(2));
					std::lock_guard<std::mutex> lock(mutex);

					closedList.push_back(node);
					openList.erase(std::remove(openList.begin(), openList.end(), node), openList.end());

					for (std::shared_ptr<T> neighbour : adjList[node->getGridX()][node->getGridY()]) {
						if (!(std::find(openList.begin(), openList.end(), neighbour) != openList.end()) && !(std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())) {
							if (std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())
								continue;

							float tentativeG = node->getG() + std::abs(node->getGridX() - neighbour->getGridX()) + std::abs(node->getGridY() - neighbour->getGridY());
							float tentativeH = std::abs(end->getGridX() - neighbour->getGridX()) + std::abs(end->getGridY() - neighbour->getGridY());
							float tentativeScore = (tentativeG + tentativeH);

							if (!(std::find(openList.begin(), openList.end(), neighbour) != openList.end())) {
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
		}
};


#endif
