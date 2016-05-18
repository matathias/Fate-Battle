#ifndef SERVANTAVENGERSAI_H
#define SERVANTAVENGERSAI_H

#pragma once

#include "Servants/servantavenger.h"

class ServantAvengerSai : public ServantAvenger
{
public:
    ServantAvengerSai(int as, Team t, Logger *l);

    virtual void subHP(int hp, DamageType dt);
    virtual vector<int> getEvade();

    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);
    virtual int doAction(int actionNum, vector<Servant *> defenders);

    virtual int attack(vector<Servant *> defenders, bool counter);

    // Casual Causality
    virtual int activateNP1(vector<Servant *> defenders);

private:
    bool casualCausalityActive();

    bool hasPermadead;
    int lastDamage;
};

#endif // SERVANTAVENGERSAI_H
