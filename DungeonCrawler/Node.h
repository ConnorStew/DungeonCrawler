#pragma once
#ifndef NODE_H
#define NODE_H

#include "stdafx.h"

class Connection;

class Node {
private:
	

	/// <summary> This nodes distance from the starting node. </summary>
	float g;

	/// <summary> This nodes heuristic distance from the starting node. </summary>
	float h;

	/// <summary> The node score based on g and h. </summary>
	float f;

	/// <summary> The index in the graphs arrays this node is stored in. </summary>
	int index;

	std::shared_ptr<Node> parent;

public:
	/// <summary> Creates a new node with the given x,y coordinates. </summary>
	/// <param name="x">x coordinate</param>
	/// <param name="y">y coordinate</param>
	Node(std::pair<int, int> location, int index);

	int getX();
	int getY();
	float getG();
	float getH();
	float getF();
	
	void updateScore(std::shared_ptr<Node> parent, float f, float g, float h);

	void setParent(std::shared_ptr<Node> parent);

	int getIndex();
	
	std::shared_ptr<Node> getParent();

protected:
	std::pair<int, int> location;
	std::pair<int, int> getLocation();
};

#endif
