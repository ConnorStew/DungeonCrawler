 #include "ConditionalRoutine.h"

ConditionalRoutine::ConditionalRoutine(std::initializer_list<Routine*> routines) {
	this->routines = routines;
	state = RUNNING;
}

void ConditionalRoutine::act(Entity * entity) {
    if (state == RUNNING) {
		for (unsigned int i = 0; i < routines.size(); i++) {
			Routine* routine = routines.at(i);
            //std::cout << "Proccessing " << routine->getName() << std::endl;
            //std::cout << "State: " << routine->getState() << std::endl;
			if (routine->getState() != SUCCESS && routine->getState() != FAILURE) {
				allFinished = false;
				routine->act(entity);
                if (routine->getState() == SUCCESS)
                    oneSucceeded = true;
                    
                if (routine->getState() == FAILURE) {
                    oneFailed = true;
                    allSucceeded = false;
                }

                break; //do one routine at a time
			}

            allFinished = true;
		}

		if (successCondition()) {
            std::cout << "Conditional routine succeeded." << std::endl;
			state = SUCCESS;
            cleanup();
		}

        if (failureCondition()) {
            std::cout << "Conditional routine failed." << std::endl;
			state = FAILURE;
            cleanup();
		}
    }
}

void ConditionalRoutine::cleanup() {
    for (unsigned int i = 0; i < this->routines.size(); i++) {
        Routine* back = routines.back();
        routines.pop_back();
        delete back;
    }
}