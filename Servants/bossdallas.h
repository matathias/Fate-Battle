#ifndef BOSSDALLAS_H
#define BOSSDALLAS_H

#pragma once

#include "Servants/servantarcher.h"

class BossDallas : public ServantArcher
{
public:
    BossDallas(int as, Team t, Logger *l);

    virtual int doAction(int actionNum, vector<Servant *> defenders);
    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);

    virtual void setPlayField(PlayField *f);

    // Re-define for Master Duelist, Stand Your Ground, and Horse Gunner
    virtual int attack(vector<Servant *> defenders, bool counter);

    // Hidden Arsenal
    virtual int activateNP1(vector<Servant *> defenders);

private:
    bool hiddenArsenalCooldown();
    int hiddenArsenalCount;
};

#endif // BOSSDALLAS_H
