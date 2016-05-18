#ifndef SERVANTAVENGERSCYTHE_H
#define SERVANTAVENGERSCYTHE_H

#pragma once

#include "Servants/servantavenger.h"

class ServantAvengerScythe : public ServantAvenger
{
public:
    ServantAvengerScythe(int as, Team t, Logger *l);

    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);
    virtual int doAction(int actionNum, vector<Servant *> defenders);

    virtual int attack(vector<Servant *> defenders, bool counter);

    // Soul Harvester
    virtual int activateNP1(vector<Servant *> defenders);
    // Death Blow
    virtual int activateNP2(vector<Servant *> defenders);

private:
    bool deathBlowActive();
};

#endif // SERVANTAVENGERSCYTHE_H
