#pragma once
#ifndef CONDITIONAL_ROUTINE_H
#define CONDITIONAL_ROUTINE_H

#include "stdafx.h"
#include "routine.h"
#include "TileMap.h"

class Entity;

class ConditionalRoutine : public Routine {
public:
	ConditionalRoutine(std::initializer_list<Routine*> routines);
	void act(Entity *entity);
private:
	vector<Routine*> routines;
    virtual bool successCondition() = 0;
	virtual bool failureCondition() = 0;
	void cleanup();
	virtual std::string getName() = 0;
protected:
    bool allSucceeded = false;
    bool allFinished = false;
	bool oneSucceeded = false;
	bool oneFailed = false;
};

#endif