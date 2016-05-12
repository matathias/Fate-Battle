#ifndef SERVANTARCHERPROJECTILES_H
#define SERVANTARCHERPROJECTILES_H

#pragma once

#include "Servants/servantarcher.h"

class ServantArcherProjectiles : public ServantArcher
{
public:
    ServantArcherProjectiles(int as, Team t, Logger *l);

    int counterfeitLifeHPGain();
    int counterfeitLifeMPGain();

    virtual int doAction(int actionNum, vector<Servant *> defenders);
    virtual int isActionNP(int action);
    virtual bool isActionRealityMarble(int action);
    virtual int getRealityMarbleMP();
    virtual vector<Coordinate> getActionRange(int action);

    // Ultralimited Blade Works
    virtual int activateNP1(vector<Servant *> defenders);
    // Limited Blade Works
    virtual int activateNP2(vector<Servant *> defenders);
    // Unlimited Blade Works
    virtual int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTARCHERPROJECTILES_H
