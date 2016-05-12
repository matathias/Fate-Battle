#ifndef SERVANTARCHERBOW_H
#define SERVANTARCHERBOW_H

#pragma once

#include "Servants/servantarcher.h"

class ServantArcherBow : public ServantArcher
{
public:
    ServantArcherBow(int as, Team t, Logger *l);

    virtual int doAction(int actionNum, vector<Servant *> defenders);
    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);

    int magicBow(vector<Servant*> defenders);

    // Re-define for the Certain Blow Noble Phantasm
    virtual int attack(vector<Servant *> defenders, bool counter);

    // Ballista
    virtual int activateNP1(vector<Servant *> defenders);
    // Rain of Arrows
    virtual int activateNP2(vector<Servant *> defenders);
};

#endif // SERVANTARCHERBOW_H
