#pragma once
#ifndef NODE_H
#define NODE_H

#include "stdafx.h"

class Node {
private:
	
	/// <summary> This nodes distance from the starting node. </summary>
	float g;

	/// <summary> This nodes heuristic distance from the starting node. </summary>
	float h;

	/// <summary> The node score based on g and h. </summary>
	float f;

	bool filled;

	std::pair<int, int> parentLoc;

public:

	/// <summary> Creates a new node with the given x,y coordinates. </summary>
	/// <param name="x">x coordinate</param>
	/// <param name="y">y coordinate</param>
	Node(std::pair<int, int> gridLoc, bool filled);

	float getG();
	float getH();
	float getF();
	
	void updateScore(std::pair<int, int> parentLoc, float f, float g, float h);
	void setParent(std::pair<int, int> parentLoc);
	
	std::pair<int,int> getParent();
	bool getFilled();

	std::pair<int, int> getGridLoc();

	int gridX();
	int gridY();

	void setFilled(bool set);

protected:
	std::pair<int, int> gridLocation;
	
};

#endif
