#include "Selector.h"

Selector::Selector(std::initializer_list<Routine*> routines) : ConditionalRoutine(routines) {}

bool Selector::successCondition() {
    return oneSucceeded;
}

bool Selector::failureCondition() {
    return (allFinished && !oneSucceeded);
}

std::string Selector::getName() {
    return "Selector";
}