#pragma once
#ifndef ROUTINE_H
#define ROUTINE_H

#include "stdafx.h"
#include "Graph.h"
#include <typeinfo>

class Entity;

/** @brief The base class of of a behaviour in the behaviour tree. */
class Routine {
public:

	/** @brief The state of the routine. */
	enum RoutineState {
		UNINITIALISED,
		SUCCESS,
		RUNNING,
		FAILURE
	};

	RoutineState getState() {
		return state;
	}

	virtual void act(Entity *entity) = 0;

	virtual ~Routine() {}

	virtual std::string getName() = 0;

protected:
	RoutineState state = UNINITIALISED;

};

#endif