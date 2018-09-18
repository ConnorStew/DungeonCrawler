#include "Sequence.h"

Sequence::Sequence(std::initializer_list<Routine*> routines) : ConditionalRoutine(routines) {}

bool Sequence::successCondition() {
    return allSucceeded;
}

bool Sequence::failureCondition() {
    return oneFailed;
}

std::string Sequence::getName() {
    return "Sequence";
}