#include "stdafx.h"
#include "Key.h"

Key::Key(int x, int y) {
	this->x = x;
	this->y = y;
}

bool operator<(const Key &lhs, const Key &rhs) {
	if (lhs.x < rhs.x) { //sort by left hand number first
		return true;
	} else if (lhs.x == rhs.x) { //if they are the same sort by right hand number
		return lhs.y < rhs.y;
	}

	return false;
}
