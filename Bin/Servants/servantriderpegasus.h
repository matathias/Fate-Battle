#ifndef SERVANTRIDERPEGASUS_H
#define SERVANTRIDERPEGASUS_H

#pragma once

#include "Servants/servantrider.h"

class ServantRiderPegasus : public ServantRider
{
public:
    ServantRiderPegasus(int as, Team t, Logger *l);

    int wingsOfIcarus();

    virtual void subHP(int hp, DamageType dt);
    virtual vector<int> getEvade();

    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);

    virtual int doAction(int actionNum, vector<Servant *> defenders);

    // Rare Foal
    virtual int activateNP1(vector<Servant *> defenders);
    // Legendary Stallion
    virtual int activateNP2(vector<Servant *> defenders);

private:
    bool highInTheSky();
};

#endif // SERVANTRIDERPEGASUS_H
