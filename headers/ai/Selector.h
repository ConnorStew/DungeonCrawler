#pragma once
#ifndef Selector_H
#define Selector_H

#include "stdafx.h"
#include "Routine.h"

using std::vector;

/// <summary>
/// This a Selector class its used to define a set of routines that only one routine must succeseed for it to successed.
///  </summary>
class Selector : public Routine {
private:
	vector<Routine*> routines;

public:
	Selector(std::initializer_list<Routine*> routines) {
        std::cout << getName() << " initialised." << std::endl;

		this->routines = routines;
		state = RUNNING;
	}

	virtual void act(Entity &entity) {
		if (state == RUNNING) {

            bool allFinished = true;
			for (unsigned int i = 0; i < routines.size(); i++) {
				Routine* routine = routines.at(i);

				if (routine->getState() != SUCCESS && routine->getState() != FAILURE) {
					allFinished = false;
					routine->act(entity);
                    break; //do one routine at a time
				}

                if (routine->getState() == SUCCESS) {
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

			//check for faliures and success
			if (allFinished == true) {
                std::cout << "Selector failed." << std::endl;
				state = FAILURE;
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
        return "Selector";
    }
};

#endif