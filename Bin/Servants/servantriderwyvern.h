#ifndef SERVANTRIDERWYVERN_H
#define SERVANTRIDERWYVERN_H

#pragma once

#include "Servants/servantrider.h"

class ServantRiderWyvern : public ServantRider
{
public:
    ServantRiderWyvern(int as, Team t, Logger *l);

    int dragonborn();

    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);

    virtual int doAction(int actionNum, vector<Servant *> defenders);

    // Fus
    virtual int activateNP1(vector<Servant *> defenders);
    // Fus Ro
    virtual int activateNP2(vector<Servant *> defenders);
    // Fus Ro Dah
    virtual int activateNP3(vector<Servant *> defenders);

private:
    bool mountKilled;
};

#endif // SERVANTRIDERWYVERN_H
