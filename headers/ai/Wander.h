#pragma once
#ifndef WANDER_H
#define WANDER_H

#include "stdafx.h"
#include "Routine.h"

class Wander : public Routine {
private:
	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> dist6;
	const int MOVEMENT_AMOUNT = 1;

	void validateMove(Entity * entity, int xIncrease, int yIncrease);
public:
	Wander();
	void act(Entity *entity);
	std::string getName();
};

#endif