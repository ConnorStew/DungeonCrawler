#pragma once
#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "stdafx.h"
#include "ConditionalRoutine.h"

class Sequence : public ConditionalRoutine {
public:
    Sequence(std::initializer_list<Routine*> routines);
    bool successCondition();
    bool failureCondition();
    std::string getName();
};

#endif