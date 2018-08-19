#pragma once
#ifndef NODE_H
#define NODE_H

#include "stdafx.h"

class Connection;

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

	/// <summary> A list of connections to this node. </summary>
	std::vector<Connection*> connections;

public:
	/// <summary> Creates a new node with the given x,y coordinates. </summary>
	/// <param name="x">x coordinate</param>
	/// <param name="y">y coordinate</param>
	Node(int x, int y);

	int getX();
	int getY();
	int getIndex();

	void setIndex(int index);

	/// <summary> Prints debug information about this node. </summary>
	void printDebug();
};

#endif
