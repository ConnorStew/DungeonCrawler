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
	vector<Routine*> routines;

public:
	Sequence(std::initializer_list<Routine*> routines) {
		std::cout << getName() + " initialised." << std::endl;
		this->routines = routines;
		state = RUNNING;
	}

	virtual void act(Entity &entity) {
		if (state == RUNNING) {

			bool allSucceeded = true;
			for (unsigned int i = 0; i < routines.size(); i++) {
				Routine* routine = routines.at(i);

				if (routine->getState() != SUCCESS) {
					allSucceeded = false;
					routine->act(entity);

					if (routine->getState() == FAILURE) {
						state = FAILURE;
						std::cout << getName() + " routines have all failed." << std::endl;
						for (unsigned int i = 0; i < this->routines.size(); i++) {
							Routine* back = routines.back();
							routines.pop_back();
							delete back;
						}
						return;
					}
					break;
				}
			}

			//check for faliures and success
			if (allSucceeded) {
				state = SUCCESS;
				std::cout << getName() + " has succeeded." << std::endl;
				for (unsigned int i = 0; i < this->routines.size(); i++) {
					Routine* back = routines.back();
					routines.pop_back();
					delete back;
				}
				return;
			}
		}
	}

	std::string getName() {
        return "Sequence";
    }
};

#endif