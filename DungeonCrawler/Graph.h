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



		//the first [] operator returns a left return object
		LeftDeref operator[](const int index) {
			return LeftDeref(this, index); //pass the x array
		}

		
		std::shared_ptr<T> operator()(int x, int y) {
			return graph->nodes[x][y];
		}
		
		//graph[LEFTRETURN][RIGHTRETURN] = 
		
};


#endif
