#pragma once
#ifndef NODE_H
#define NODE_H

#include <string>

class Node {
private:
	/// <summary> The nodes x coordinate. </summary>
	float x;
	/// <summary> The nodes y coordinate. </summary>
	float y;
	/// <summary> This nodes distance from the starting node. </summary>
	float g;
	/// <summary> This nodes heuristic distance from the starting node. </summary>
	float h;
	/// <summary> The node score based on g and h. </summary>
	float f;
	/// <summary> The index in the graphs arrays this node is stored in. </summary>
	int index;
public:
	/// <summary> Creates a new node with the given x,y coordinates. </summary>
	/// <param name="x">x coordinate</param>
	/// <param name="y">y coordinate</param>
	/// <param name="index">the index of this node</param>
	Node(float x, float y, int index);

	/// <summary> The index of this node. </summary>
	/// <returns>The index of this node.</returns>
	int getIndex();

	/// <summary> Prints debug information about this node. </summary>
	void printDebug();
};

#endif
