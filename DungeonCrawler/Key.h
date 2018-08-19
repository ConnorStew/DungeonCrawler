#pragma once
#ifndef KEY_H
#define KEY_H

class Key {
private:
	int x;
	int y;

public:
	/// <summary> Creates a new key with the given x,y coordinates. </summary>
	/// <param name="x">x coordinate</param>
	/// <param name="y">y coordinate</param>
	Key(int x, int y);

	friend bool operator< (const Key &lhs, const Key &rhs);
};

#endif
