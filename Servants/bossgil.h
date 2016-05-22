#ifndef BOSSGIL_H
#define BOSSGIL_H

#pragma once

#include "Servants/servantarcher.h"

class BossGil : public ServantArcher
{
public:
    BossGil(int as, Team t, Logger *l);

    virtual void setPlayField(PlayField *f);

    virtual void subHP(int hp, DamageType dt);

    virtual int doAction(int actionNum, vector<Servant *> defenders);
    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);

    virtual int activateNP1(vector<Servant *> defenders);
    virtual int activateNP2(vector<Servant *> defenders);
    virtual int activateNP3(vector<Servant *> defenders);

private:
    bool chainsCooldown();
    bool gateCooldown();
    bool eaCooldown();
};

#endif // BOSSGIL_H
