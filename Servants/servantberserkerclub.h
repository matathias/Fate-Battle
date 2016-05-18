#ifndef SERVANTBERSERKERCLUB_H
#define SERVANTBERSERKERCLUB_H

#pragma once

#include "Servants/servantberserker.h"

class ServantBerserkerClub : public ServantBerserker
{
public:
    ServantBerserkerClub(int as, Team t, Logger *l);

    virtual void turnUpdate();

    virtual int attack(vector<Servant *> defenders, bool counter);

    virtual void subHP(int hp, DamageType dt);

protected:
    bool previouslyTargeted(Servant *target);

    bool damageFreeTurn;
    int increases;
    int strIncrement;
    int sklIncrement;

    vector<Servant*> previousTargets;
};

#endif // SERVANTBERSERKERCLUB_H
