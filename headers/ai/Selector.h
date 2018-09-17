#pragma once
#ifndef SELECTOR_H
#define SELECTOR_H

#include "stdafx.h"
#include "ConditionalRoutine.h"

class Selector : public ConditionalRoutine {
public:
    Selector(std::initializer_list<Routine*> routines);
    bool successCondition();
    bool failureCondition();
    std::string getName();
};

#endif