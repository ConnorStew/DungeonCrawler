#pragma once
#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "stdafx.h"
#include "Routine.h"

using std::vector;

/// <summary>
/// This a sequence class its used to define a set of routines that must all succeseed for it to successed.
///  </summary>
class Sequence : public Routine {
private:
	vector<Routine*> *routines;

public:
	Sequence(std::initializer_list<Routine*> routines) {
		this->routines = new vector<Routine*>;
		for (Routine* routine : routines)
			this->routines->push_back(routine);

		state = RUNNING;
	}

	virtual void act(Entity &entity) {
		if (state == RUNNING) {

			bool allSucceeded = true;
			for (unsigned int i = 0; i < routines->size(); i++) {
				Routine* routine = routines->at(i);

				if (routine->getState() != SUCCESS) {
					allSucceeded = false;
					routine->act(entity);

					if (routine->getState() == FAILURE) {
						delete routines;
						state = FAILURE;
						return;
					}
					break;
				}
			}

			
			//check for faliures and success


			if (allSucceeded) {
				state = SUCCESS;
				delete routines;
				return;
			}
		}
	}
};

#endif