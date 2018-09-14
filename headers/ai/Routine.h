#pragma once
#ifndef ROUTINE_H
#define ROUTINE_H

#include "stdafx.h"
#include "Graph.h"

class Entity;

/// <summary>The base class of of a behaviour in the behaviour tree. </summary>
class Routine {
public:
	enum RoutineState {
		SUCCESS,
		RUNNING,
		FAILURE
	};

	RoutineState getState() {
		return state;
	}

	void setState(RoutineState state) {
		this->state = state;
	}

	virtual void act(Entity &entity) = 0;

protected:
	RoutineState state;
};

#endif