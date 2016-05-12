#ifndef SERVANTARCHERPISTOLS_H
#define SERVANTARCHERPISTOLS_H

#pragma once

#include "Servants/servantarcher.h"

class ServantArcherPistols : public ServantArcher
{
public:
    ServantArcherPistols(int as, Team t, Logger *l);

    virtual int doAction(int actionNum, vector<Servant *> defenders);
    virtual int isActionNP(int action);
    virtual bool isActionRealityMarble(int action);
    virtual int getRealityMarbleMP();
    virtual vector<Coordinate> getActionRange(int action);

    // Re-define for the Skill: Rapid-Fire Mod
    virtual int attack(vector<Servant *> defenders, bool counter);

    // Sweeping Blows
    virtual int activateNP1(vector<Servant *> defenders);
    // 360 NO SCOPE
    virtual int activateNP2(vector<Servant *> defenders);
    // Castle Doctrine
    virtual int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTARCHERPISTOLS_H
