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

		int parentX;

		int parentY;

	public:

		/// <summary> Creates a new node with the given x,y coordinates. </summary>
		/// <param name="x">x coordinate</param>
		/// <param name="y">y coordinate</param>
		Node(int gridX, int gridY, bool filled);

		float getG();
		float getH();
		float getF();
		int getGridX();
		int getGridY();
		int getParentX();
		int getParentY();
		bool getFilled();

		void updateScore(int parentX, int parentY, float f, float g, float h);
		void setParent(int parentX, int parentY);

		void setFilled(bool set);

	protected:
		int gridX;
		int gridY;
	
};

#endif
