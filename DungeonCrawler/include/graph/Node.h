#pragma once
#ifndef NODE_H
#define NODE_H

#include "stdafx.h"

using std::shared_ptr;
using std::pair;
using std::vector;

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

		float worldX;

		float worldY;

	public:

		Node(int gridX, int gridY, float worldX, float worldY, bool filled);

		float getG();
		float getH();
		float getF();
		int getGridX();
		int getGridY();
		int getParentX();
		int getParentY();
		bool getFilled();
		float getWorldX();
		float getWorldY();

		void updateScore(int parentX, int parentY, float f, float g, float h);
		void setParent(int parentX, int parentY);

		void setFilled(bool set);

	protected:
		int gridX;
		int gridY;
	
};

#endif
